#include <zmq.hpp>
#include <string>
#include <iostream>
#include <msgpack.hpp>
#include <fstream>
#include "zhelper.hpp"
#include <sstream>


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

	ofstream myfile;
	myfile.open ("example.txt");

	for(int i=0; i<100; i++){

		int line = 0;
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

				for (char_nbr = 0; char_nbr <= size; char_nbr++) {	
					if (countC < 1280){
						if (countL < 720) {
							//myfile << (int) data[char_nbr] <<" ";
						}else{
							//myfile << (int) data[char_nbr] <<"\n";
							countL = 0;
							countC++;
						}
						countL++;
					}
					else if (countC >= 1280 and countC < 2560){
						if (countL < 720) {
							//myfile << (int) data[char_nbr] <<" ";
						}else{
							//myfile << (int) data[char_nbr] <<"\n";
							countL = 0;
							countC++;
						}
						countL++;
					}
					else if (countC >= 2560 and countC <= 3840){
						if (countL < 720) {
							myfile << (int) data[char_nbr] <<" ";
						}else{
							myfile << (int) data[char_nbr] <<"\n";
							countL = 0;
							countC++;
						}
						countL++;
					}
				}
			}
			
			if (line==2) std::cout << "Received One Frame" << std::endl;

			int more = 0;           //  Multipart detection
			size_t more_size = sizeof (more);
			subscriber.getsockopt (ZMQ_RCVMORE, &more, &more_size);

			if (!more) break; //  Break when there is no more message
			//getchar();
			line++;
    		}

	myfile.close();

   	}
	return 0;
}


