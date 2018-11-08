#include <string>
#include <iostream>
#include <thread>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp" 	// for cascade classifier
#include "include/lsl_cpp.h"

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

double t = 0.0;
const double fps1 = 50.0;
const double fps2 = 10.0;

const double t1 = 1.0 / fps1;
const double t2 = 1.0 / fps2;

// true - drop the frame
// false - do NOT drop the frame
bool NextTick()
{
  t += t1;
  if ( t > t2 )
  {
    t -= t2;
    return false;
  }
  return true;
}


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

	addWeighted(imgThresholded_bright, 1.0, imgThresholded_dark, 1.0, 0.0, imgThresholded);

	//morphological opening (removes small objects from the foreground)
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

	//morphological closing (removes small holes from the foreground)
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
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

	// start receiving & displaying the data
	cout << "Now pulling samples..." << endl;


	VideoCapture cap(0); //capture the video from webcam

	if ( !cap.isOpened() )  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	// set camera resolution
	//cout << "Resolution" << cap.get(CV_CAP_PROP_FRAME_WIDTH) << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;

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

	//Capture a temporary image from the camera
	Mat imgTmp;
	cap.read(imgTmp); 

	//Create a black image with the size as the camera output
	imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;
 

	//-- 1. Load the cascades
	if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
	if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

	int i = 0;
	while (true)
	{
		bool bSuccess = cap.grab(); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}
		if (NextTick()){
		  	//std::cout<<"dropping the frame"<<std::endl;
		}		
		else
		{
			//std::cout<<"display the frame"<<std::endl;
			bool bSuccess = cap.retrieve(imgOriginal);
			//// Read the Different Objects
			// save the original frame		
			imgOriginalTotal = imgOriginal;

			// red Object
			thread t1(task1, "Red Object", 0, 100, 100, 10, 255, 255, 160, 100, 100, 179, 255, 255);
			t1.join();

			// add to the original frame the location of the red Object
			imgOriginalTotal = imgOriginalTotal + imgLines;
			//imshow("Thresholded Red", imgThresholded); //show the thresholded image

			imgThresholded = Mat::zeros( imgTmp.size(), CV_8UC3 );;
			imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;

			// green Object
		     	thread t2(task1, "Green Object", 65, 60, 60, 80, 255, 255, 65, 60, 160, 80, 255, 179);
			t2.join();

			// add to the original frame the location of the red Object
			imgOriginalTotal = imgOriginalTotal + imgLines;
			//imshow("Thresholded Green", imgThresholded); //show the thresholded image

			// blue Object
			thread t3(task1, "Blue Object", 90, 130, 60, 140, 255, 255, 100, 170, 80, 140, 255, 255);
			t3.join();
			imgOriginal = imgOriginal + imgLines;
			//imshow("Thresholded Blue", imgThresholded); //show the thresholded image

			// show the location of all the objects in the original frame
			//imshow("Original", imgOriginalTotal); //show the original image

			// Face Detection
			thread t4(task2, "Face");

			t4.join();
			//-- Show what you got
			imshow("Faces", imgOriginal);
	

		}
		//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		if (waitKey(30) == 27) 
		{
			cout << "esc key is pressed by user" << endl;
			break; 
		}

	}

}


