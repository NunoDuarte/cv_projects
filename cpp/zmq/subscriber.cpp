#include <zmq.hpp>
#include <string>
#include <iostream>
#include <msgpack.hpp>

int main()
{
    zmq::context_t context(1);
    std::cout << 1 << std::endl;
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://127.0.0.1:38101"); //43597
    std::cout << 2 << std::endl;
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "frame.world", 11);

    std::cout << 3 << std::endl;

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
        zmq::message_t env;
        subscriber.recv(&env,0);

        std::string env_str = std::string(static_cast<char*>(env.data()), env.size());
        std::cout << "Received envelope '" << env_str << "'" << std::endl;

 	std::cout << "" << std::endl;

        zmq::message_t msg;

        //  Process any waiting weather updates
        bool rc;
        do {
            zmq::message_t msg;
            if ((rc = subscriber.recv(&msg, ZMQ_DONTWAIT)) == true) {
                //  process weather update
		std::string msg_str = std::string(static_cast<char*>(msg.data()), msg.size());
		msgpack::object_handle oh = msgpack::unpack(msg_str.data(), msg_str.size());
		msgpack::object obj = oh.get();
		std::cout << obj << std::endl;

            }
        } while(rc == true);
        /*subscriber.recv(&msg);

	std::string msg_str = std::string(static_cast<char*>(msg.data()), msg.size());
	msgpack::object_handle oh = msgpack::unpack(msg_str.data(), msg_str.size());
	msgpack::object obj = oh.get();
	std::cout << obj << std::endl;
*/
    }


    for(int i=0; i<2; i++)
    {

        zmq::message_t env;
        subscriber.recv(&env,0);
        std::string env_str = std::string(static_cast<char*>(env.data()), env.size());
        //std::cout << "Received envelope '" << env_str << "'" << std::endl;
        zmq::message_t msg;
        subscriber.recv(&msg, 0);
        std::string msg_str = std::string(static_cast<char*>(msg.data()), msg.size());
        //std::cout << "Received '" << msg_str << "'" << std::endl;

	msgpack::unpacker pac;
	pac.reserve_buffer( env_str.size() );
	std::copy( env_str.begin(), env_str.end(), pac.buffer() );
	pac.buffer_consumed( env_str.size() );

    	malloc(900000);

	msgpack::object_handle oh = msgpack::unpack(env_str.data(), env_str.size());;
	int count = 0;
	while ( pac.next(oh) ) {
		msgpack::object msg = oh.get();
		std::cout << msg << " ";
		count++;
	}

        std::cout << "Received " << count << " counts" << std::endl;

	/*msgpack::unpacked upd;
	msgpack::unpack(upd, msg_str.data(), msg_str.size());
	//msgpack::object obj = oh.get();
	std::cout << upd.get() << std::endl;
*/


    }
    return 0;
}
