// getdiskinfo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include "hardware.h"
#include <vector>
#include <thread>
#include <mutex>

std::mutex g4stdout;

void test()
{

  //DiskId
  std::vector<std::string> diskId;
  peanut::HdInfo::getInstance().getDiskId(diskId);
  //sort(diskId.begin(), diskId.end());
  //for (auto auTemp : diskId)
 
  if(diskId.size() > 0) {
    std::lock_guard<std::mutex> lock(g4stdout);
    std::cout << std::this_thread::get_id() << "diskId = " << diskId[0] << "," << diskId[1]   << std::endl;
  }
    
  //cpuId
  std::string cpuId = "";
  peanut::HdInfo::getInstance().getCpuId(cpuId);
  {
    std::lock_guard<std::mutex> lock(g4stdout);
    std::cout << std::this_thread::get_id() << "cpuId = " << cpuId << std::endl;
  }
  

  //SystemId
  std::string systemId = "";
  peanut::HdInfo::getInstance().getSystemId(systemId);
  {
    std::lock_guard<std::mutex> lock(g4stdout);
    std::cout << std::this_thread::get_id() << "systemId = " << systemId << std::endl;;
  }

}

int main() {

  int times = 10;
  int num = 10;

  for (int m = 0; m < times; ++m) {
    std::vector<std::thread> vec;

    for (int i = 0; i < num; ++i) {
      vec.emplace_back(std::move(std::thread(test)));
    }


    for (int i = 0; i < num; ++i) {
      vec[i].join();
    }
  
  }
  

  //peanut::hdflag::GetInstance().uninitialize();

  std::cout << "done....\n";

  return 0;
}
