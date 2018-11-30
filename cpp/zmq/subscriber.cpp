#include <zmq.hpp>
#include <string>
#include <iostream>
#include <msgpack.hpp>

int main()
{
    zmq::context_t context(1);
    std::cout << 1 << std::endl;
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://127.0.0.1:35607"); //43597
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
        subscriber.recv(&msg);
/*        std::string msg_str = std::string(static_cast<char*>(msg.data()), msg.size());
        std::cout << "Received '" << msg_str << "'" << std::endl;

        // serializes this object.
        std::vector<std::string> vec;
        vec.push_back(msg_str); */
/*        vec.push_back("MessagePack");

        // serialize it into simple buffer.
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, vec);;
*/

/*
	msgpack::unpacked unpacked_body;
	msgpack::unpack(&unpacked_body, static_cast<const char*>(env.data()), env.size());
	unpacked_body.get().convert(&data);
*/

	std::string msg_str = std::string(static_cast<char*>(msg.data()), msg.size());

	msgpack::object_handle oh = msgpack::unpack(msg_str.data(), msg_str.size());
	msgpack::object obj = oh.get();
	std::cout << obj << std::endl;

/*        // deserialize it.
        msgpack::object_handle oh = msgpack::unpack(sbuf.data(), sbuf.size());

        // print the deserialized object.
        msgpack::object obj = oh.get();
        std::cout << obj << std::endl;  //=> ["Hello", "MessagePack"]
*/
}
    for(int i=0; i<100; i++)
    {

        zmq::message_t msg;
        subscriber.recv(&msg);
	std::string msg_str = std::string(static_cast<char*>(msg.data()), msg.size());

	msgpack::object_handle oh = msgpack::unpack(msg_str.data(), msg_str.size());
	msgpack::object obj = oh.get();
	std::cout << obj << std::endl;

        subscriber.recv(&msg);
	msg_str = std::string(static_cast<char*>(msg.data()), msg.size());
	oh = msgpack::unpack(msg_str.data(), msg_str.size());
	obj = oh.get();
	std::cout << obj << std::endl;

        std::cout << "Received" << std::endl;

    }
    return 0;
}
