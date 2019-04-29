#include <zmq.hpp>
#include <string>
#include <iostream>
#include <msgpack.hpp>
#include <fstream>
#include "zhelper.hpp"
#include <sstream>
#include <opencv2/opencv.hpp>
#include "opencv2/core/cuda.hpp"
#include <opencv2/core/ocl.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/cudaobjdetect.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/cudawarping.hpp"
#include "cuda_runtime_api.h"
#include "device_launch_parameters.h"


using namespace cv;
using namespace std;
using namespace cv::cuda;

int main(){

	/*if (!cv::ocl::haveOpenCL()){
		cout << "OpenCL is not avaiable..." << endl;
		return -1;
	}
	cv::ocl::Context context1;
	if (!context1.create(cv::ocl::Device::TYPE_GPU)){
		cout << "Failed creating the context..." << endl;
		return -1;
	}

	// In OpenCV 3.0.0 beta, only a single device is detected.
	cout << context1.ndevices() << " GPU devices are detected." << endl;



	cuda::GpuMat gI;

	getchar();*/
	cuda::GpuMat gI;


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

	Mat A = Mat::zeros(720,1280, CV_8UC1);
	vector<Mat> channels;
	Mat fin_img;

	int count = 0;
	while(1){
		
		int line = 0;
			//		Mat A = Mat::zeros(720,1280, CV_8UC1);	
		Mat A1 = Mat::zeros(720,1280, CV_8UC1);
		while (1) {

			//  Process all parts of the message
			zmq::message_t message;
			subscriber.recv(&message);

			//  Dump the message as text or binary
			int size = message.size();
			std::string data(static_cast<char*>(message.data()), size);
			msgpack::object_handle oh = msgpack::unpack(data.data(), data.size());
			msgpack::object obj = oh.get();

			bool is_text = true;
			int char_nbr;
			unsigned char byte;
			for (char_nbr = 0; char_nbr < size; char_nbr++) {
			    byte = data [char_nbr];
			    if (byte < 32 || byte > 127)
				is_text = false;
			}
			int countL = 0;
			int countC = 0;
			//std::cout << "[" << std::setfill('0') << std::setw(3) << size << "]";

			// check if we have passed the first two messages
			if (line==2){
				
				for (char_nbr = 0; char_nbr < size; char_nbr+=3) {
 					if (countC < 720){
						if (countL < 1279) {
							//R << (int) data [char_nbr] << " ";
							countL++;
						}else{
							//R << (int) data [char_nbr] << ";\n";
							countC++;
							countL = 0;
						}
					}
					A1.data[A1.channels()*(A1.cols*countC + countL) + 0] = (int) data [char_nbr];
					//std::count << A.data[0] << std::endl;
					//std::cout << A.data[A.channels()*(A.cols*countC + countL) + 0] << std::endl;
				}
				channels.push_back(A1);
				Mat A2 = Mat::zeros(720,1280, CV_8UC1);
				countL = 0;
				countC = 0;
				for (char_nbr = 1; char_nbr < size; char_nbr+=3) {
 					if (countC < 720){
						if (countL < 1279) {
							countL++;
						}else{
							countC++;
							countL = 0;
						}
					}
					A2.data[A2.channels()*(A2.cols*countC + countL) + 1] = (int) data [char_nbr];
				}
				channels.push_back(A2);
				Mat A3 = Mat::zeros(720,1280, CV_8UC1);
				countL = 0;
				countC = 0;
				for (char_nbr = 2; char_nbr < size; char_nbr+=3) {
 					if (countC < 720){
						if (countL < 1279) {
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
			
			if (line==2) std::cout << "Received One Frame" << std::endl;

			int more = 0;           //  Multipart detection
			size_t more_size = sizeof (more);
			subscriber.getsockopt (ZMQ_RCVMORE, &more, &more_size);

			if (!more) break; //  Break when there is no more message
			//getchar();
			line++;
    		}
			merge(channels, fin_img);
			gI.upload(A1);
			if (count % 4 ==0 ) imshow("threshold",fin_img);
			// Press  ESC on keyboard to  exit
			char c = (char)waitKey(1);
			if( c == 27 ) break;
			channels.clear();
	count++;

   	}
	return 0;
}


