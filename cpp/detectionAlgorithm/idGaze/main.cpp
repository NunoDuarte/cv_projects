#include <string>
#include <iostream>
#include <thread>
#include "opencv2/opencv.hpp"
#include "opencv2/cudafilters.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp" 	// for cascade classifier
#include "include/lsl_cpp.h"
#include <msgpack.hpp>
#include <zmq.hpp>
#include "zhelper.hpp"
#include <fstream>

using namespace cv;
using namespace std;

Mat imgLines;
Mat imgOriginal;
Mat imgOriginalTotal;
Mat imgThresholded;

int iLowHb;
int iHighHb;
int iLowSb; 
int iHighSb;
int iLowVb;
int iHighVb;
int iLowHd;
int iHighHd;
int iLowSd; 
int iHighSd;
int iLowVd;
int iHighVd;

int iLastX; 
int iLastY;

/** Function Headers */
void detectAndDisplay( Mat frame );

/** Global variables */
String face_cascade_name = "cascade-icub-60v60.xml";
String eyes_cascade_name = "haarcascade_eye.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_name = "Capture - Face detection";
RNG rng(12345);

void image(Mat &imgOriginal, Mat &imgLines, Mat &imgThresholded, int iLowHb, int iLowSb, int iLowVb, int iHighHb, int iHighSb, int iHighVb, int iLowHd, int iLowSd, int iLowVd, int iHighHd, int iHighSd, int iHighVd)
{

	Mat imgHSV;
	Mat imgThresholded_bright;
	Mat imgThresholded_dark;

	//Convert the captured frame from BGR to HSV
	cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); 	

	//Threshold the image
	inRange(imgHSV, Scalar(iLowHb, iLowSb, iLowVb), Scalar(iHighHb, iHighSb, iHighVb), imgThresholded_bright); 
	inRange(imgHSV, Scalar(iLowHd, iLowSd, iLowVd), Scalar(iHighHd, iHighSd, iHighVd), imgThresholded_dark); 

	cv::cuda::addWeighted(imgThresholded_bright, 1.0, imgThresholded_dark, 1.0, 0.0, imgThresholded);

	//morphological opening (removes small objects from the foreground)
	// CUDA erode
	/*cuda::GpuMat d_img0(imgThresholded);
	Mat element = cv::getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	cv::Ptr<cv::cuda::Filter> erodeFilter = cv::cuda::createMorphologyFilter(cv::MORPH_ERODE, d_img0.type(), element);
	erodeFilter->apply(d_img0, d_img0);*/
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	// CUDA dilate
	/*Ptr<cuda::Filter> dilateFilter = cv::cuda::createMorphologyFilter(MORPH_DILATE, d_img0.type(), element);
	dilateFilter->apply(d_img0, d_img0);

	//morphological closing (removes small holes from the foreground)
	// CUDA dilate
	dilateFilter->apply(d_img0, d_img0);*/
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

	// CUDA erode
	/*erodeFilter->apply(d_img0, d_img0);
	d_img0.download(imgThresholded);*/
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

	//Calculate the moments of the thresholded image
	Moments oMoments = moments(imgThresholded);

	double dM01 = oMoments.m01;
	double dM10 = oMoments.m10;
	double dArea = oMoments.m00;

	// if the area <= 10000, I consider that the there are no 
	// object in the image and it's because of the noise, the area is not zero 
	if (dArea > 10000)
	{
		//calculate the position of the ball
		int posX = dM10 / dArea;
		int posY = dM01 / dArea;        

		if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
		{
			//Draw a red line from the previous point to the current point
			line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);
		}

		iLastX = posX;
		iLastY = posY;
	}

}

// The function we want to execute on the new thread.
void task1(string msg, int iLowHb, int iLowSb, int iLowVb, int iHighHb, int iHighSb, int iHighVb, int iLowHd, int iLowSd, int iLowVd, int iHighHd, int iHighSd, int iHighVd)
{
	image(imgOriginal, imgLines, imgThresholded, iLowHb, iLowSb, iLowVb, iHighHb, iHighSb, iHighVb, iLowHd, iLowSd, iLowVd, iHighHd, iHighSd, iHighVd);
}

// The function we want to execute on the new thread.
void task2(string msg)
{
	detectAndDisplay(imgOriginal);
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
	std::vector<Rect> faces;
	Mat gray, blurred, thresh;

	// cv2 - convert RGB to Gray scale
	cvtColor( frame, gray, COLOR_BGR2GRAY );
	// cv2 - blur the gray image
	GaussianBlur( gray, blurred, Size( 31,31), 0);
	// cv2 - Threshold the blurred image
	threshold( blurred, thresh, 127, 255, THRESH_TOZERO);

	//cvtColor( frame, frame_gray, CV_BGR2GRAY );
	//equalizeHist( frame_gray, frame_gray );

	//-- Detect faces
	face_cascade.detectMultiScale( gray, faces, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE);
	bool detected = false;

	for( size_t i = 0; i < faces.size(); i++ )
	{
		Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
		//ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

		Mat faceROI = gray( faces[i] );
		std::vector<Rect> eyes;

		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale( faceROI, eyes);

		if (eyes.size() > 1 and eyes.size() < 3)
		{
			for( size_t j = 0; j < eyes.size(); j++ )
			{
				ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
				Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
				int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
				circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
			}
			detected = true;
			break;
		}
		if (detected) break;
	}
	frame = imgOriginal;
}

int main(int argc, char** argv)
{

	cout << "Now resolving streams..." << endl;
	cout << "looking for an NormPose2IP stream..." << endl;
	vector<lsl::stream_info> results = lsl::resolve_stream("name", "NormPose2IP");

	cout << "Here is what was resolved: " << endl;
	cout << results[0].as_xml() << endl;

	// make an inlet to get data from it
	cout << "Now creating the inlet..." << endl;
	lsl::stream_inlet inlet(results[0]);

	// make a new stream_info (GazePose) and open an outlet with it
	lsl::stream_info info("GazePose", "NormPose2IP", 3, 100, lsl::cf_float32, "myuid34234");
	info.desc().append_child_value("manufacturer", "VisLab");
	lsl::stream_outlet outlet(info);

	// start receiving & displaying the data
	cout << "Now pulling samples..." << endl;
	zmq::context_t context(1);
	zmq::socket_t subscriber(context, ZMQ_SUB);

	std::string ip = "tcp://127.0.0.1:";
	// request reply client for SUB_PORT
	zmq::socket_t requester(context, ZMQ_REQ);
	requester.connect("tcp://127.0.0.1:50020");

    	int request;
        s_send (requester, "SUB_PORT");
        std::string req_sub = s_recv (requester);
	std::ostringstream oss;
	oss << ip << req_sub;

	subscriber.connect(oss.str()); // connect SUB_PORT IP 
	subscriber.setsockopt(ZMQ_SUBSCRIBE, "frame.world", 11);

	vector<Mat> channels;
	Mat fin_img;

	int width;
	int height;

	iLowHb = 0;
	iHighHb = 10;

	iLowSb = 100; 
	iHighSb = 255;

	iLowVb = 100;
	iHighVb = 255;

	iLowHd = 160;
	iHighHd = 179;

	iLowSd = 100; 
	iHighSd = 255;

	iLowVd = 100;
	iHighVd = 255;

	iLastX = -1; 
	iLastY = -1;

	int i = 0;
	while (true)
	{
		int line = 0;
		while (1) {

			//  Process all parts of the message
			zmq::message_t message;
			subscriber.recv(&message);

			//  Dump the message as text or binary
			int size = message.size();
			std::string data(static_cast<char*>(message.data()), size);
			//msgpack::object_handle oh = msgpack::unpack(data.data(), data.size());
			//msgpack::object obj = oh.get();

			int char_nbr;
			int countL = 0;
			int countC = 0;
			//std::cout << "[" << std::setfill('0') << std::setw(3) << size << "]";
			if (line==1){

				std::size_t pos = data.find("width");
				std::string str1 = data.substr(pos+5);  //
				msgpack::object_handle oh = msgpack::unpack(str1.data(), str1.size());
				msgpack::object obj = oh.get();
				//std::cout << obj << std::endl;
				width = obj.as<int>();

				std::size_t pos1 = data.find("height");
				std::string str2 = data.substr(pos1+6);  //

				oh = msgpack::unpack(str2.data(), str2.size());
				obj = oh.get();
				//std::cout << obj << std::endl;
				height = obj.as<int>();

			}
			// check if we have passed the first two messages
			if (line==2){

				Mat A1 = Mat(height,width, CV_8UC1);
				for (char_nbr = 0; char_nbr < size; char_nbr+=3) {
					if (countC < height){
						if (countL < width-1) {
							//R << (int) data [char_nbr] << " ";
							countL++;
						}else{
							//R << (int) data [char_nbr] << ";\n";
							countC++;
							countL = 0;
						}
					}
					A1.data[A1.channels()*(A1.cols*countC + countL) + 1] = (int) data [char_nbr];
				}
				channels.push_back(A1);
				Mat A2 = Mat(height,width, CV_8UC1);
				countL = 0;
				countC = 0;
				for (char_nbr = 1; char_nbr < size; char_nbr+=3) {
					if (countC < height){
						if (countL < width-1) {
							countL++;
						}else{
							countC++;
							countL = 0;
						}
					}
					A2.data[A2.channels()*(A2.cols*countC + countL) + 1] = (int) data [char_nbr];
				}
				channels.push_back(A2);
				Mat A3 = Mat(height,width, CV_8UC1);
				countL = 0;
				countC = 0;
				for (char_nbr = 2; char_nbr < size; char_nbr+=3) {
					if (countC < height){
						if (countL < width-1) {
							countL++;
						}else{
							countC++;
							countL = 0;
						}
					}
					A3.data[A3.channels()*(A3.cols*countC + countL) + 2] = (int) data [char_nbr];
				}
				channels.push_back(A3);
			}

			int more = 0;           //  Multipart detection
			size_t more_size = sizeof (more);
			subscriber.getsockopt (ZMQ_RCVMORE, &more, &more_size);
			//getchar();
			if (!more) break; //  Break when there is no more message
			line++;
		}
		merge(channels, fin_img);

		// Press  ESC on keyboard to  exit
		char c = (char)	waitKey(1);
		if( c == 27 ) break;

		channels.clear();

		//// Read the Different Objects
		// save the original frame
		imgOriginal = fin_img;		
		imgOriginalTotal = fin_img;

		//Capture a temporary image from the camera
		Mat imgTmp = fin_img;

		//Create a black image with the size as the camera output
		//cv::cuda::multiply(imgLines, Mat::zeros( imgTmp.size(), CV_8UC3 ), imgLines);
		imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );

		// red Object
		thread t1(task1, "Red Object", 0, 100, 100, 10, 255, 255, 160, 100, 100, 179, 255, 255);
		t1.join();

		// add to the original frame the location of the red Object
		imgOriginalTotal = imgOriginalTotal + imgLines;
		//imshow("Thresholded Red", imgThresholded); //show the thresholded image

		imgThresholded = Mat::zeros( imgTmp.size(), CV_8UC3 );
		imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );

		// green Object
	     	thread t2(task1, "Green Object", 44, 54, 63, 71, 255, 255, 65, 60, 160, 71, 255, 255);
		t2.join();

		// add to the original frame the location of the red Object
		imgOriginalTotal = imgOriginalTotal + imgLines;
		//imshow("Thresholded Green", imgThresholded); //show the thresholded image

		imgThresholded = Mat::zeros( imgTmp.size(), CV_8UC3 );
		imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );

		// blue Object
		thread t3(task1, "Blue Object", 90, 130, 60, 140, 255, 255, 100, 170, 80, 140, 255, 255);
		t3.join();

		imgOriginalTotal = imgOriginalTotal + imgLines;
		//imshow("Thresholded Blue", imgThresholded); //show the thresholded image

		// getting sample from inlet
		std::vector<std::vector<float>> chunk_nested_vector;
		double ts;
		// get the sample and timestamp
		if (ts = inlet.pull_chunk(chunk_nested_vector)){

			float pos_x = chunk_nested_vector[0][1];
			float pos_y = chunk_nested_vector[0][2];

			// Draw a circle 
			circle(imgOriginalTotal,Point(int(pos_x*(width)), int(height) - int(pos_y*int(width))), 10, Scalar(0,255, 1), 5, 8);
		}

		// show the location of all the objects in the original frame
		imshow("Original", imgOriginalTotal); //show the original image
	}


}


