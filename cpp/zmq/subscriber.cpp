#include <zmq.hpp>
#include <string>
#include <iostream>
#include <msgpack.hpp>

int main()
{
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
//    void *ctx = zmq_ctx_new ();
//    void *dealer = zmq_socket (ctx, ZMQ_DEALER);
    subscriber.connect("tcp://127.0.0.1:44231"); //43597
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "frame.world", 11);

    for(int i=0; i<2; i++)
    {

        zmq::message_t env;
        subscriber.recv(&env,0);
        std::string env_str = std::string(static_cast<char*>(env.data()), env.size());
        std::cout << "Received '" << env_str << "'" << std::endl;

	msgpack::unpacker pac;
	pac.reserve_buffer( env_str.size() );
	std::copy( env_str.begin(), env_str.end(), pac.buffer() );
	pac.buffer_consumed( env_str.size() );

	msgpack::object_handle oh1 = msgpack::unpack(env_str.data(), env_str.size());;
	int count = 0;
	while ( pac.next(oh1) ) {
		msgpack::object msg = oh1.get();
		std::cout << msg << " ";
		count++;
	}

        std::cout << "Received " << count << " ENV counts" << std::endl;

        //  Process 
        bool rc;
        do {
	    //std::cout << "haay!!! " << i << std::endl;
            zmq::message_t msg;
            if ((rc = subscriber.recv(&msg, ZMQ_DONTWAIT)) == true) {
                //  process update
		std::string msg_str = std::string(static_cast<char*>(msg.data()), msg.size());
		msgpack::object_handle oh = msgpack::unpack(msg_str.data(), msg_str.size());
		msgpack::object obj = oh.get();
		std::cout << obj << std::endl;

            }
        } while(rc == true);	
	std::cout << " " << std::endl;        
    }

/*
    for(int i=0; i<10; i++)
    {

        zmq::message_t env;
        subscriber.recv(&env,0);
        std::string env_str = std::string(static_cast<char*>(env.data()), env.size());

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
	while ( pac.next(oh) ) {
		msgpack::object msg = oh.get();
		std::cout << msg << " ";
		count++;
	}
        std::cout << "Received " << count << " counts" << std::endl;
     }
*/
    
    return 0;
}
