#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>


#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <limits.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#endif

namespace psimpl {
inline std::string getAppDir() {
  std::string path;
#ifdef _WIN32
  char buf[MAX_PATH] = {0};
  GetModuleFileNameA(NULL, buf, MAX_PATH);
  path = buf;
  if (!path.empty()) {
    path = path.substr(0, path.rfind("\\"));
  }

#else
  char buf[PATH_MAX];
  ssize_t len = ::readlink("/proc/self/exe", buf, sizeof(buf) - 1);
  if (len != -1) {
    buf[len] = '\0';
    path = buf;

    path = path.substr(0, path.rfind("/"));
  }

#endif
  return path;
}
}  // namespace psimpl

namespace Helper {
class ProcessSingleton {
 private:
  std::string uniqueName_;
#ifdef _WIN32
  std::shared_ptr<void> mutexHandle_;
#else
  int lockFileDescriptor_{-1};
  bool isLocked_{false};
#endif

 public:
  ProcessSingleton(const std::string &name) {
#ifdef _WIN32
    uniqueName_ = "Global\\" + name;
    mutexHandle_ = std::shared_ptr<void>(CreateMutexA(NULL, TRUE, uniqueName_.c_str()), CloseHandle);
#else
    uniqueName_ = psimpl::getAppDir() + "/" + name;
#endif
  }

  ~ProcessSingleton() {
#ifdef _WIN32
    // if (mutexHandle_) {
    // CloseHandle(mutexHandle_.get());
    // }
#else
    if (lockFileDescriptor_ != -1) {
      close(lockFileDescriptor_);
      if (isLocked_) {
        flock(lockFileDescriptor_, LOCK_UN);
        unlink(uniqueName_.c_str());
      }
    }
#endif
  }

  bool create() {
#ifdef _WIN32
    return (mutexHandle_ && GetLastError() != ERROR_ALREADY_EXISTS);
#else
    umask(0);  // 设置 umask 为 0，禁止掩码掉任何权限位
    lockFileDescriptor_ = open(uniqueName_.c_str(), O_RDWR | O_CREAT, 0666);
    if (lockFileDescriptor_ == -1) return false;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    int nret = fcntl(lockFileDescriptor_, F_SETLK, &lock);
    if (nret == -1) {
      isLocked_ = false;
    } else {
      isLocked_ = true;
    }
    return isLocked_;
#endif
  }
};

}  // namespace Helper
