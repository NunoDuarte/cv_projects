#include <zmq.hpp>
#include <string>
#include <iostream>
#include <msgpack.hpp>
#include <fstream>
#include "zhelper.hpp"
#include <sstream>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

int main(){

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

	ofstream R;
	ofstream G;
	ofstream B;
	R.open ("april25r.txt");
	G.open ("april25g.txt");
	B.open ("april25b.txt");


Mat A = Mat::zeros(720,1280, CV_8UC1);
vector<Mat> channels;
Mat fin_img;

	for(int i=0; i<1; i++){

		int line = 0;
			//		Mat A = Mat::zeros(720,1280, CV_8UC1);	
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
				Mat A1 = Mat::zeros(720,1280, CV_8UC1);
				for (char_nbr = 0; char_nbr < size; char_nbr+=3) {
 					if (countC < 720){
						if (countL < 1279) {
							R << (int) data [char_nbr] << " ";
							//G << (int) data [char_nbr + 1] << " ";
							//B << (int) data [char_nbr + 2] << " ";
							countL++;
						}else{
							R << (int) data [char_nbr] << ";\n";
							//G << ";\n";
							//B << ";\n";
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
							G << (int) data [char_nbr] << " ";
							//G << (int) data [char_nbr + 1] << " ";
							//B << (int) data [char_nbr + 2] << " ";
							countL++;
						}else{
							G << (int) data [char_nbr] << ";\n";
							//G << ";\n";
							//B << ";\n";
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
							B << (int) data [char_nbr] << " ";
							//G << (int) data [char_nbr + 1] << " ";
							//B << (int) data [char_nbr + 2] << " ";
							countL++;
						}else{
							B << (int) data [char_nbr] << ";\n";
							//G << ";\n";
							//B << ";\n";
							countC++;
							countL = 0;
						}
					}
					A3.data[A3.channels()*(A3.cols*countC + countL) + 2] = (int) data [char_nbr];
				}
				channels.push_back(A3);


					/*else if (countC >= 1440 and countC < 2160){
						if (countL < 1280) {
							myfile << (int) data [char_nbr] << " ";
						}else{
							myfile << (int) data [char_nbr] << "\n";
							countL = 0;
							countC++;
						}
						countL++;
					}*/

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
			imshow("threshold",fin_img);
			waitKey(0);

	R.close();
	G.close();
	B.close();

   	}
	return 0;
}


