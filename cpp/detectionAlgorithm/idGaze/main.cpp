#include <string>
#include <iostream>
#include <thread>
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

	int count = 0;

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

		//if (count % 1 ==0 ) imshow("threshold",fin_img);

		// Press  ESC on keyboard to  exit
		char c = (char)	waitKey(1);
		if( c == 27 ) break;

		channels.clear();
		count++;

		//// Read the Different Objects
		// save the original frame
		imgOriginal = fin_img;		
		imgOriginalTotal = fin_img;

		//Capture a temporary image from the camera
		Mat imgTmp = fin_img;

		//Create a black image with the size as the camera output
		imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );

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


