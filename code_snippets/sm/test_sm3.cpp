#include <string.h>

#include <iostream>
#include <sstream>
#include <string>

#include "sm3.h"

std::string arr2hex(unsigned char* hash, int len) {
  std::stringstream ss;
  for (int i = 0; i < len; ++i) ss << std::hex << (int)hash[i];
  return ss.str();
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage:" << argv[0] << " string\n";
    return 0;
  }
  unsigned char hash[32] = {};
  sm3((unsigned char*)argv[1], strlen(argv[1]), hash);
  std::cout << "sm3(" << argv[1] << ")=" << arr2hex(hash, sizeof(hash))
            << std::endl;

  return 0;
}
