# spdlog

最开始使用工作中一个log库（windows平台），考虑到跨平台开发，采用了glog。后来又参与国产平台的开发，为了减少依赖、简单无依赖，使用了spdlog。

主要使用：debug info error 三个级别的日志，默认输出info日志。调试时可以使用

1. linux shell中 
   ```shell
   # 1
   SPDLOG_LEVEL=debug ./example
   # 2
   export SPDLOG_LEVEL=debug
   ./example
   ```
2. windows cmd中 
   ```cmd
   set SPDLOG_LEVEL=debug
   example.exe
   ```

下面是示例。

```c++
#include "spdlog/cfg/env.h" // support for loading levels from the environment variable
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"

void init_glog();

int main(const int, const char **) {
  init_glog();

  spdlog::info("Welcome to spdlog!");
  spdlog::error("Some error message with arg: {}", 1);

  spdlog::warn("Easy padding in numbers like {:08d}", 12);
  spdlog::critical(
      "Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
  spdlog::info("Support for floats {:03.2f}", 1.23456);
  spdlog::info("Positional args are {1} {0}..", "too", "supported");
  spdlog::info("{:<30}", "left aligned");

  spdlog::debug("This message should be displayed..");

  return 0;
}

void init_glog() {
  std::string logpath = "./peanut.log";
  spdlog::set_level(spdlog::level::info);
  spdlog::cfg::load_env_levels();
  spdlog::set_pattern("[%Y-%m-%d] [%H:%M:%S.%f %z] [%^%l%$] [thread %t] %v");
  // Create a file rotating logger with 5mb size max and 3 rotated files.
  auto rotating_logger =
      spdlog::rotating_logger_mt("some_logger_name", logpath, 1048576 * 5, 3);
  spdlog::set_default_logger(rotating_logger);
  spdlog::flush_on(spdlog::level::debug);
  spdlog::info("spdlog version {}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR,
               SPDLOG_VER_PATCH);
  spdlog::debug("enable debug model.");
}
```

