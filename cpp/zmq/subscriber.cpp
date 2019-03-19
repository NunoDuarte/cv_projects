#include <zmq.hpp>
#include <string>
#include <iostream>
#include <msgpack.hpp>
#include <fstream>
#include "zhelper.hpp"

using namespace std;

int main()
{
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
//    void *ctx = zmq_ctx_new ();
//    void *dealer = zmq_socket (ctx, ZMQ_DEALER);
    subscriber.connect("tcp://127.0.0.1:44643"); //43597
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "frame.world", 11);

  ofstream myfile;
  myfile.open ("example.txt");

    for(int i=0; i<10; i++)
    {

  /*      zmq::message_t env;
        subscriber.recv(&env,0);
        std::string env_str = std::string(static_cast<char*>(env.data()), env.size());
        std::cout << "Received '" << env_str << "'" << std::endl;

	msgpack::unpacker pac;
	pac.reserve_buffer( env_str.size() );
	std::copy( env_str.begin(), env_str.end(), pac.buffer() );
	pac.buffer_consumed( env_str.size() );

	msgpack::object_handle oh = msgpack::unpack(env_str.data(), env_str.size());;
	int count = 0;
	while ( pac.next(oh) ) {
		msgpack::object msg = oh.get();
		myfile << msg << " ";
		std::cout << msg << " ";
		count++;
	}

	myfile.close();
        std::cout << "Received " << count << " ENV counts" << std::endl;


	bool rc;
	    zmq::message_t msg;
	    if ((rc = subscriber.recv(&msg, ZMQ_DONTWAIT)) == true) {
		//  process update
		std::string msg_str = std::string(static_cast<char*>(msg.data()), msg.size());
		msgpack::object_handle oh = msgpack::unpack(msg_str.data(), msg_str.size());
		//msgpack::object obj = oh.get();
		//std::cout << obj << std::endl;

		msgpack::unpacker pac1;
		pac1.reserve_buffer( msg_str.size() );
		std::copy( msg_str.begin(), msg_str.end(), pac1.buffer() );
		pac1.buffer_consumed( msg_str.size() );

		int count1 = 0;
		while ( pac1.next(oh) ) {
			msgpack::object msg = oh.get();
			myfile << msg << " ";
			//std::cout << msg << " ";
			count1++;
		}

		std::cout << "Received " << count1 << " ENV counts" << std::endl;
	    }
*/
        //  Process 
       /* bool rc;
        do {
	    //std::cout << "haay!!! " << i << std::endl;
            zmq::message_t msg;
            if ((rc = subscriber.recv(&msg, ZMQ_DONTWAIT)) == true) {
                //  process update
		std::string msg_str = std::string(static_cast<char*>(msg.data()), msg.size());
		msgpack::object_handle oh = msgpack::unpack(msg_str.data(), msg_str.size());
		//msgpack::object obj = oh.get();
		//std::cout << obj << std::endl;

		msgpack::unpacker pac1;
		pac1.reserve_buffer( msg_str.size() );
		std::copy( msg_str.begin(), msg_str.end(), pac1.buffer() );
		pac1.buffer_consumed( msg_str.size() );

		int count1 = 0;
		while ( pac1.next(oh) ) {
			msgpack::object msg = oh.get();
			myfile << msg << " ";
			//std::cout << msg << " ";
			count1++;
		}

		std::cout << "Received " << count1 << " ENV counts" << std::endl;

            }
        } while(rc == true);	
	std::cout << "NEW FRAME" << std::endl;
	std::cout << " " << std::endl;      
 
*/
    


 /*       zmq::message_t env;
        subscriber.recv(&env,0);
        std::string env_str = std::string(static_cast<char*>(env.data()), env.size());
	//std::cout << "Received '" << env_str << "'" << std::endl;

        //zmq::message_t msg;
        //subscriber.recv(&msg, 0);
        //std::string msg_str = std::string(static_cast<char*>(msg.data()), msg.size());
        //std::cout << "Received '" << msg_str << "'" << std::endl;

	msgpack::unpacker pac;
	pac.reserve_buffer( env_str.size() );
	std::copy( env_str.begin(), env_str.end(), pac.buffer() );
	pac.buffer_consumed( env_str.size() );

	msgpack::object_handle oh = msgpack::unpack(env_str.data(), env_str.size());;
	int count = 0;
	while ( pac.next(oh)) {
		msgpack::object msg = oh.get();
		if (i != 2) std::cout << msg << " ";
		count++;
	}
        std::cout << "Received " << count << " counts" << std::endl;

	getchar();
*/

    while (1) {
        //  Process all parts of the message
        zmq::message_t message;
        subscriber.recv(&message);

        //  Dump the message as text or binary
        int size = message.size();
        std::string data(static_cast<char*>(message.data()), size);
	msgpack::object_handle oh = msgpack::unpack(data.data(), data.size());
	msgpack::object obj = oh.get();
	std::cout << obj << std::endl;

        bool is_text = true;

        int char_nbr;
        unsigned char byte;
        for (char_nbr = 0; char_nbr < size; char_nbr++) {
            byte = data [char_nbr];
            if (byte < 32 || byte > 127)
                is_text = false;
        }
       /* std::cout << "[" << std::setfill('0') << std::setw(3) << size << "]";
        for (char_nbr = 0; char_nbr < size; char_nbr++) {
            if (is_text)
                std::cout << (char)data [char_nbr] << " " ;
            else
                std::cout << std::setfill('0') << std::setw(2)
                   << std::hex << (unsigned int) data [char_nbr] << " ";
        }
        std::cout << std::endl;*/
	std::cout << "Received " << size << " counts" << std::endl;

        int more = 0;           //  Multipart detection
        size_t more_size = sizeof (more);
        subscriber.getsockopt (ZMQ_RCVMORE, &more, &more_size);
        if (!more){
     	    std::cout << "It broke!" << std::endl;
            break;              //  Last message part
	}
    }
        //  Process 
      /*  bool rc;
	int count = 0;
        do {
	    //std::cout << "haay!!! " << i << std::endl;
            zmq::message_t env;
            if ((rc = subscriber.recv(&env)) == true and (count-1)%3 == 0) {
                //  process update
		std::string env_str = std::string(static_cast<char*>(env.data()), env.size());
		msgpack::object_handle oh = msgpack::unpack(env_str.data(), env_str.size());
		msgpack::object obj = oh.get();
		std::cout << obj << std::endl;

		int more = 0; 
		size_t more_size = sizeof (more);
		subscriber.getsockopt(ZMQ_RCVMORE, &more, &more_size);
		while (more) {
			std::cout << "hello" << std::endl;
	
			msgpack::unpacker pac;
			pac.reserve_buffer( env_str.size() );
			std::copy( env_str.begin(), env_str.end(), pac.buffer() );
			pac.buffer_consumed( env_str.size() );

			env_str = std::string(static_cast<char*>(env.data()), env.size());
			oh = msgpack::unpack(env_str.data(), env_str.size());
			while ( pac.next(oh)) {
				msgpack::object msg = oh.get();
			  	myfile << msg << " ";
				count++;
			}
			std::cout << "Received " << count << " counts" << std::endl;
		}
			

  		myfile.close();
		
		getchar();

            }
		
		count++;
        } while(rc == true);	
	std::cout << " " << std::endl;     */  

    }
    return 0;
}
