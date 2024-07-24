#pragma once

#include <vector>
#include <string>

#ifdef _WIN32
#include "hardware_win_impl.h"
#endif

#ifdef _LINUX
//todo
#endif

// 为了在主线程中做com初始化，声明了一个全局变量，做了妥协。
extern peanut::HardwareInfo gHardwareInfo;

namespace peanut {

  class HdInfo {
  public:
    static HdInfo& getInstance()
    {
      static HdInfo instance;
      return instance;
    }

    bool getDiskId(std::vector<std::string>& diskIds)
    {
      return gHardwareInfo.GetDiskIds(diskIds);
    }

    bool getCpuId(std::string& cpuId)
    {
      return gHardwareInfo.GetCpuId(cpuId);
    }

    bool getSystemId(std::string& systemId)
    {
      return gHardwareInfo.GetSystemId(systemId);
    }

  private:
    HdInfo() = default;
    ~HdInfo() = default;

    HdInfo(const HdInfo&) = delete;
    HdInfo& operator=(const HdInfo&) = delete;
  };

} // namespace peanut
