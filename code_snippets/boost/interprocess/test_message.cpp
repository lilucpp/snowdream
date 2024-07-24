// test_message.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "boost/interprocess/ipc/message_queue.hpp"
#include <chrono>
#include <thread>
#include <map>
int main(int argc, char** argv)
{
	std::map<std::string, std::string> map;
	namespace bi = boost::interprocess;
  if (argc == 1) {
	// parent process
    bi::message_queue::remove("test_message");
    bi::message_queue mq(bi::open_or_create, "test_message", 10, sizeof(int));
    for (int i = 0; i < 100; ++i) {
      mq.send(&i, sizeof(i), 0);
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
   
	getchar();
  }
  else{
    bi::message_queue mq(bi::open_only, "test_message");
    unsigned int priority;
    bi::message_queue::size_type recvd_size;
    while (true) {
	  int number;
      mq.receive(&number, sizeof(number), recvd_size, priority);
      std::cout << "revice: " << number << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
  }

  return 0;
}

