#include <iostream>
#include "BoundedBlockingQueueIPC.h"

int main() {
  peanut::MPMCQueue q(10, false, "peanutTest");

  for (int i = 0; i < 10; ++i) {
    std::cout << q.take() << std::endl;
  }
  return 0;
}
