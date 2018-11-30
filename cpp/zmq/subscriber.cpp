#include <zmq.hpp>
#include <string>
#include <iostream>

int main()
{
    zmq::context_t context(1);
    std::cout << 1 << std::endl;
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://127.0.0.1:36273"); //43597
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

    while(true)
    {
        std::cout << 4 << std::endl;
        zmq::message_t env;
        std::cout << 5 << std::endl;
        subscriber.recv(&env,0);
        std::cout << 6 << std::endl;
        std::string env_str = std::string(static_cast<char*>(env.data()), env.size());
        std::cout << "Received envelope '" << env_str << "'" << std::endl;

 	std::cout << "" << std::endl;

        zmq::message_t msg;
        subscriber.recv(&msg, 0);
        std::string msg_str = std::string(static_cast<char*>(msg.data()), msg.size());
        std::cout << "Received '" << msg_str << "'" << std::endl;

	break;
    }
    return 0;
}
