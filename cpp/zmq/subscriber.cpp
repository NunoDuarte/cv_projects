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
#include <jpeglib.h>


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

	Mat gI;

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

	int count = 0;
	while(1){
		
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
				msgpack::object_handle oh = msgpack::unpack(data.data(), data.size());
				msgpack::object obj = oh.get();
				std::cout << obj << std::endl;

				std::cout << data.data() << std::endl;

				std::size_t pos = data.find("width");
				std::cout << data.find("width") << std::endl;

				std::string str3 = data.substr(pos-1);  // this gets me width
				std::cout << data.substr(pos-1) << std::endl;

				std::string str4 = data.substr(pos+5);  //
				std::cout << data.substr(pos+5) << std::endl;

				std::size_t pos1 = data.find("height");
				std::cout << data.find("height") << std::endl;

				std::string str5 = data.substr(pos1-1);  // this gets me height
				std::cout << data.substr(pos1-1) << std::endl;

				std::string str6 = data.substr(pos1+6);  //
				std::cout << data.substr(pos1+6) << std::endl;

				oh = msgpack::unpack(str6.data(), str6.size());
				obj = oh.get();
				std::cout << obj << std::endl;

				getchar();
			        /*msgpack::sbuffer buffer;
				// deserializes these objects using msgpack::unpacker.
				msgpack::unpacker pac;
			
				// feeds the buffer.
				pac.reserve_buffer(data.size());
				memcpy(pac.buffer(), data.data(), data.size());
				pac.buffer_consumed(data.size());

				// now starts streaming deserialization.
				msgpack::object_handle oh;
				while(pac.next(oh)) {
					std::cout << oh.get() << std::endl;
				}		
				//msgpack::object_handle oh = msgpack::unpack(data.data(), data.size());	*/
				//msgpack::object obj = oh.get();
				//std::cout << obj << std::endl;

			}
			// check if we have passed the first two messages
			if (line==2){

				Mat A1 = Mat(720,1280, CV_8UC1);
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
					A1.data[A1.channels()*(A1.cols*countC + countL) + 1] = (int) data [char_nbr];
				}
				channels.push_back(A1);
				Mat A2 = Mat(720,1280, CV_8UC1);
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
				Mat A3 = Mat(720,1280, CV_8UC1);
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

			int more = 0;           //  Multipart detection
			size_t more_size = sizeof (more);
			subscriber.getsockopt (ZMQ_RCVMORE, &more, &more_size);
			//getchar();
			if (!more) break; //  Break when there is no more message
			line++;
    		}
		merge(channels, fin_img);
		//fin_img.download(gI);

		if (count % 1 ==0 ) imshow("threshold",fin_img);

		// Press  ESC on keyboard to  exit
		char c = (char)	waitKey(1);
		if( c == 27 ) break;

		channels.clear();

		count++;

   	}
	return 0;
}


