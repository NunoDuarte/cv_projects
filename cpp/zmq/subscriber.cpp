#include <zmq.hpp>
#include <string>
#include <iostream>
#include <msgpack.hpp>

int main()
{
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://127.0.0.1:37775"); //43597
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "frame.world", 11);

/*    int request_nbr;
    for (request_nbr = 0; request_nbr != 10; request_nbr++) {
      zmq::message_t request((void*)"SUB_PORT", 8, NULL);
	//        zmq::msg_init_size (&request, 5);
      //        memcpy (zmq::msg_data (&request), "Hello", 5);
      printf ("Sending Hello %d…\n", request_nbr);
      std::cout << 4 << std::endl;
      char *text = "SUB_PORT";
      send(subscriber, text);
      subscriber.send ( &request, 0);
      std::cout << 5 << std::endl;
      //zmq::msg_close (&request);

      std::cout << 6 << std::endl;
      zmq::message_t reply;
      std::cout << 7 << std::endl;
      subscriber.recv ( &reply);
      printf ("Received World %d\n", request_nbr);
    }
*/


/*zmq::message_t FirstFrame(FirstBuffer.size());
memcpy(FirstFrame.data(), FirstBuffer.c_str(), FirstBuffer.size());

msgpack::sbuffer SecondBuf;
msgpack::pack(SecondBuf, DetectStruct.mode);

zmq::message_t SecondFrame(SecondBuf.size());
memcpy(SecondFrame.data(), SecondBuf.data(), SecondBuf.size());

zmq::multipart_t multipart;

multipart.add(std::move(FirstFrame));
multipart.add(std::move(SecondFrame));

multipart.send(*Sub2DSocket);
*/
    for(int i=0; i<1; i++)
    {
        /*zmq::message_t env;
        subscriber.recv(&env,0);

        std::string env_str = std::string(static_cast<char*>(env.data()), env.size());
        std::cout << "Received envelope '" << env_str << "'" << std::endl;

 	std::cout << "" << std::endl;
*/
        //  Process any waiting weather updates
        bool rc;
	int k = 0;
        do {
            zmq::message_t msg;
            if ((rc = subscriber.recv(&msg, 0)) == true ) {
                //  process update
		std::string msg_str = std::string(static_cast<char*>(msg.data()), msg.size());
        	//std::cout << "Received envelope '" << msg_str << "'" << std::endl;

		msgpack::unpacker pac1;
		pac1.reserve_buffer( msg_str.size() );
		std::copy( msg_str.begin(), msg_str.end(), pac1.buffer() );
		pac1.buffer_consumed( msg_str.size() );

		msgpack::object_handle oh1 = msgpack::unpack(msg_str.data(), msg_str.size());;
		int count = 0;
		int j = 0;
		while ( pac1.next(oh1) and j < 20) {
			msgpack::object msg = oh1.get();
			std::cout << msg << " ";
			count++;
			j++;
		}
		std::cout << " " << "count " << count << std::endl;

		msgpack::unpacker pac;
		pac.reserve_buffer( msg_str.size() );
		std::copy( msg_str.begin(), msg_str.end(), pac.buffer() );
		pac.buffer_consumed( msg_str.size() );

		//std::cout << " " << "count " << count << std::endl;
		k++;
            }

        } while(rc == true and k < 20);

        /*zmq::message_t msg;
        subscriber.recv(&msg, ZMQ_DONTWAIT);
        std::string msg_str = std::string(static_cast<char*>(msg.data()), msg.size());
        std::cout << "Received '" << msg_str << "'" << std::endl;
	msgpack::object_handle oh = msgpack::unpack(msg_str.data(), msg_str.size());
	msgpack::object obj = oh.get();
	std::cout << obj << std::endl;
*/

        //  Process any waiting weather updates
/*        bool rc;
        do {
            zmq::message_t msg;
            if ((rc = subscriber.recv(&msg, ZMQ_DONTWAIT)) == true) {
                //  process update
		std::string msg_str = std::string(static_cast<char*>(msg.data()), msg.size());
		msgpack::object_handle oh = msgpack::unpack(msg_str.data(), msg_str.size());
		msgpack::object obj = oh.get();
		std::cout << obj << std::endl;

		msgpack::unpacker pac;
		pac.reserve_buffer( msg_str.size() );
		std::copy( msg_str.begin(), msg_str.end(), pac.buffer() );
		pac.buffer_consumed( msg_str.size() );

		//std::cout << " " << "count " << count << std::endl;

            }
        } while(rc == true);

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

        std::cout << "Received " << count << " counts" << std::endl;
*/        
/*subscriber.recv(&msg);
	std::string msg_str = std::string(static_cast<char*>(msg.data()), msg.size());
	msgpack::object_handle oh = msgpack::unpack(msg_str.data(), msg_str.size());
	msgpack::object obj = oh.get();
	std::cout << obj << std::endl;
*/
    }


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
/*
	msgpack::object_handle oh = msgpack::unpack(env_str.data(), env_str.size());;
	int count = 0;
	while ( pac.next(oh) ) {
		msgpack::object msg = oh.get();
		std::cout << msg << " ";
		count++;
	}
        std::cout << "Received " << count << " counts" << std::endl;
*/
     }
    
    return 0;
}
