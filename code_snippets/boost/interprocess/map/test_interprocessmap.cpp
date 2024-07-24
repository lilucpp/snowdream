// test_interprocessmap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "BoundedMapIPC.h"
#include <iostream>
#include <thread>
#include <map>

void travel(const std::multimap<std::string, std::string> &mapwarp) {
  std::cout << "map=";
  for (auto itr = mapwarp.begin(); itr != mapwarp.end(); ++itr) {
    std::cout << "\t" << itr->first << "=" << itr->second << "\n";
  }

}

int main(int argc, char** argv) {
  if (argc < 2) {
	return 0;
  }
  namespace bi = boost::interprocess;
  if (argc == 2) {
    bi::shared_memory_object::remove("peanutTestMultiMap");
  }


  Helper::BI::IPMapWarp pid_sic("peanutTestMultiMap3", 2048, true);
  Helper::BI::IPMapWarp sid_pid("peanutTestMultiMap2", 2048, true);
  Helper::BI::IPMapWarp imap("peanutTestMultiMap2", 2048, true);
  int num = atoi(argv[1]);
  while (true) {
    imap.clear();
    for (int i = 0; i < atoi(argv[1]); ++i) {
      imap.insert(std::to_string(i), std::to_string(num++));
      imap.insert(std::to_string(i), std::to_string(num++));
      auto c = imap.equal_range(std::to_string(i));
      for (auto &val : c) {
        std::cout<< "\t" << "val=" << val << std::endl;
	  }
    }
    //std::multimap<std::string, std::string> temp({imap.begin(), imap.end()});
    //travel(temp);
    std::cout << "map size=" << imap.size() << std::endl;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
  

  getchar();
  return 0;
}
#include <boost/interprocess/managed_shared_memory.hpp>
#include <cstdlib>  //std::system
#include <sstream>
#include <boost/interprocess/permissions.hpp>

int main22(int argc, char *argv[]) {

  using namespace boost::interprocess;
  if (argc == 1) {  // Parent process
    // Remove shared memory on construction and destruction
    struct shm_remove {
      shm_remove() { shared_memory_object::remove("MySharedMemory"); }
      ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
    } remover;

    // Create a managed shared memory segment
    managed_shared_memory segment(create_only, "MySharedMemory", 65536);

    // Allocate a portion of the segment (raw memory)
    managed_shared_memory::size_type free_memory = segment.get_free_memory();
    void *shptr = segment.allocate(1024 /*bytes to allocate*/);
    strncpy((char*)shptr, "hello, world!~", 1024);

    // Check invariant
    if (free_memory <= segment.get_free_memory()) return 1;

    // An handle from the base address can identify any byte of the shared
    // memory segment even if it is mapped in different base addresses
    managed_shared_memory::handle_t handle =
        segment.get_handle_from_address(shptr);
    std::stringstream s;
    s << argv[0] << " " << handle;
    s << std::ends;
    // Launch child process
    if (0 != std::system(s.str().c_str())) return 1;
    // Check memory has been freed
    if (free_memory != segment.get_free_memory()) return 1;
  } else {
    // Open managed segment
    managed_shared_memory segment(open_only, "MySharedMemory");

    // An handle from the base address can identify any byte of the shared
    // memory segment even if it is mapped in different base addresses
    managed_shared_memory::handle_t handle = 0;

    // Obtain handle value
    std::stringstream s;
    s << argv[1];
    s >> handle;

    // Get buffer local address from handle
    void *msg = segment.get_address_from_handle(handle);
    std::cout << "msg="
              << (char*) msg << std::endl;

    // Deallocate previously allocated memory
    segment.deallocate(msg);
  }
  return 0;
}
