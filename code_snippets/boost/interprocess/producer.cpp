#include <chrono>
#include <thread>
#include "BoundedBlockingQueueIPC.h"

int main() {
  peanut::MPMCQueue q(10, true, "peanutTest");

  for (int i = 0; i < 100; ++i) {
    q.put(std::to_string(i));
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  getchar();
  return 0;
}
