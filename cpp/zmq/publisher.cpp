#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/format.hpp>
#include <zmq.hpp>
#include <string>
#include <iostream>

int main()
{
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_REP);
    publisher.bind("tcp://*:50020");

    while (true) {
        zmq::message_t request;

        //  Wait for next request from client
        publisher.recv (&request);
        std::cout << "Received Hello" << std::endl;

        //  Do some 'work'
        sleep (1);

        //  Send reply back to client
        zmq::message_t reply (5);
        memcpy ((void *) reply.data (), "World", 5);
        publisher.send (reply);
    }

/*    for(int n = 0; n < 3; n++) {
	//getchar();
	
        zmq::message_t env1(1);
        memcpy(env1.data(), "A", 1);
        std::string msg1_str = (boost::format("Hello-%i") % (n + 1)).str();
        zmq::message_t msg1(msg1_str.size());
        memcpy(msg1.data(), msg1_str.c_str(), msg1_str.size());
        std::cout << "Sending '" << msg1_str << "' on topic A" << std::endl;
        publisher.send(env1, ZMQ_SNDMORE);
        publisher.send(msg1);

        zmq::message_t env2(1);
        memcpy(env2.data(), "B", 1);
        std::string msg2_str = (boost::format("World-%i") % (n + 1)).str();
        zmq::message_t msg2(msg2_str.size());
        memcpy(msg2.data(), msg2_str.c_str(), msg2_str.size());
        std::cout << "Sending '" << msg2_str << "' on topic B" << std::endl;
        publisher.send(env2, ZMQ_SNDMORE);
        publisher.send(msg2);

        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    }*/
    return 0;
}
