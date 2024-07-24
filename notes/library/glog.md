# glog 使用

## 资料

1. [How To Use Google Logging Library](http://rpg.ifi.uzh.ch/docs/glog.html)
2. [How to use glog](https://manutdzou.github.io/2016/09/14/how-to-use-glog.html)
3. [glog使用文档](http://www.yeolar.com/note/2014/12/20/glog/)
4. [官网](https://github.com/google/glog#google-style-perror)

## Introduction

**Google glog** is a library that implements application-level logging. This library provides logging APIs based on C++-style streams and various helper macros. You can log a message by simply streaming things to LOG(<a particular [severity level](#severity)>), e.g.

```c++
#include <glog/logging.h>

int main(int argc, char* argv[]) {
  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);

  // ...
  LOG(INFO) << "Found " << num_cookies << " cookies";
}
```

Google glog defines a series of macros that simplify many common logging tasks. You can log messages by severity level, control logging behavior from the command line, log based on conditionals, abort the program when expected conditions are not met, introduce your own verbose logging levels, and more.

## Compiler

 gflags: cmake -DBUILD_SHARED_LIBS=ON -DINSTALL_HEADERS=ON -DINSTALL_SHARED_LIBS=ON 
 glog: ./autogen.sh && ./configure && make && make install

## Severity Level

You can specify one of the following severity levels (in increasing order of severity): `INFO`, `WARNING`, `ERROR`, and `FATAL`. Logging a `FATAL` message terminates the program (after the message is logged).

Unless otherwise specified, glog writes to the filename "/tmp/<program name>.<hostname>.<user name>.log.<severity level>.<date>.<time>.<pid>" (e.g., "/tmp/hello_world.example.com.hamaji.log.INFO.20080709-222411.10474"). By default, glog copies the log messages of severity level `ERROR` or `FATAL` to standard error (stderr) in addition to log files.

## Setting Flags

Several flags influence glog's output behavior. 

```c++
gflags::ParseCommandLineFlags(&argc, &argv, true);
```

For example, if you want to turn the flag `--logtostderr` on, you can start your application with the following command line:

```shell
./your_application --logtostderr=1
```

The following flags are most commonly used:

- `logtostderr` (`bool`, default=`false`)

  Log messages to stderr instead of logfiles. Note: you can set binary flags to `true` by specifying `1`, `true`, or `yes` (case insensitive). Also, you can set binary flags to `false` by specifying `0`, `false`, or `no` (again, case insensitive).

- `stderrthreshold` (`int`, default=2, which is `ERROR`)

  Copy log messages at or above this level to stderr in addition to logfiles. The numbers of severity levels `INFO`, `WARNING`, `ERROR`, and `FATAL` are 0, 1, 2, and 3, respectively.

- `minloglevel` (`int`, default=0, which is `INFO`)

  Log messages at or above this level. Again, the numbers of severity levels `INFO`, `WARNING`, `ERROR`, and `FATAL` are 0, 1, 2, and 3, respectively.

- `log_dir` (`string`, default="")

  If specified, logfiles are written into this directory instead of the default logging directory.

- `v` (`int`, default=0)

  Show all `VLOG(m)` messages for `m` less or equal the value of this flag. Overridable by --vmodule. See [the section about verbose logging](#verbose) for more detail.

You can also modify flag values in your program by modifying global variables `FLAGS_*` . Most settings start working immediately after you update `FLAGS_*` . The exceptions are the flags related to destination files. For example, you might want to set `FLAGS_log_dir` before calling `google::InitGoogleLogging` . Here is an example:

```c++
   LOG(INFO) << "file";
   // Most flags work immediately after updating values.
   FLAGS_logtostderr = 1;
   LOG(INFO) << "stderr";
   FLAGS_logtostderr = 0;
   // This won't change the log destination. If you want to set this
   // value, you should do this before google::InitGoogleLogging .
   FLAGS_log_dir = "/some/log/directory";
   LOG(INFO) << "the same file";
```

my configure:

```c++
    FLAGS_log_dir = "./log/";
    FLAGS_stderrthreshold = 0;
    FLAGS_colorlogtostderr = 1;
    FLAGS_minloglevel = 0; 
```

## Conditional / Occasional Logging

Sometimes, you may only want to log a message under certain conditions. You can use the following macros to perform conditional logging:

```c++
   LOG_IF(INFO, num_cookies > 10) << "Got lots of cookies";
```

The follow line outputs a log messages on the 1st, 11th, 21st, ... times it is executed.
```c++
  LOG_EVERY_N(INFO, 10) << "Got the " << google::COUNTER << "th cookie";
```

You can combine conditional and occasional logging with the following macro.

```c++
   LOG_IF_EVERY_N(INFO, (size > 1024), 10) << "Got the " << google::COUNTER
                                           << "th big cookie";
```

Instead of outputting a message every nth time, you can also limit the output to the first n occurrences:

```c++
   LOG_FIRST_N(INFO, 20) << "Got the " << google::COUNTER << "th cookie";
```

## Debug Mode Support

Special "debug mode" logging macros only have an effect in debug mode and are compiled away to nothing for non-debug mode compiles. Use these macros to avoid slowing down your production application due to excessive logging.

```c++
   DLOG(INFO) << "Found cookies";

   DLOG_IF(INFO, num_cookies > 10) << "Got lots of cookies";

   DLOG_EVERY_N(INFO, 10) << "Got the " << google::COUNTER << "th cookie";
```

## CHECK Macros

`CHECK` aborts the application if a condition is not true. Unlike `assert`, it is *not* controlled by `NDEBUG`, so the check will be executed regardless of compilation mode. Therefore, `fp->Write(x)` in the following example is always executed:

```c++
   CHECK(fp->Write(x) == 4) << "Write failed!";
```

There are various helper macros for equality/inequality checks - `CHECK_EQ`, `CHECK_NE`, `CHECK_LE`, `CHECK_LT`, `CHECK_GE`, and `CHECK_GT`. They compare two values, and log a `FATAL` message including the two values when the result is not as expected. The values must have `operator<<(ostream, ...)` defined.

## Verbose Logging

When you are chasing difficult bugs, thorough log messages are very useful.

The `--v` command line option controls which verbose messages are logged:

```c++
   VLOG(1) << "I'm printed when you run the program with --v=1 or higher";
   VLOG(2) << "I'm printed when you run the program with --v=2 or higher";
```

if `--v==1`, `VLOG(1)` will log, but `VLOG(2)` will not log.

## Notes for Windows users

Google glog defines a severity level `ERROR`, which is also defined in `windows.h` . You can make glog not define `INFO`, `WARNING`, `ERROR`, and `FATAL` by defining `GLOG_NO_ABBREVIATED_SEVERITIES` before including `glog/logging.h` . Even with this macro, you can still use the iostream like logging facilities:

```c++
  #define GLOG_NO_ABBREVIATED_SEVERITIES
  #include <windows.h>
  #include <glog/logging.h>
```

## Automatically Remove Old Logs

To enable the log cleaner:

```c++
google::EnableLogCleaner(3); // keep your logs for 3 days
```

And then glog will check if there are overdue logs whenever a flush is performed. In this example, any log file from your project whose last modified time is greater than 3 days will be unlink()ed.

This feature can be disabled at any time (if it has been enabled)

```c++
google::DisableLogCleaner();
```

## example

```c++
#include <glog/logging.h>
#include "../Dll/Header.h" // add

void init_log() {
  FLAGS_log_dir = "./log/";
  FLAGS_stderrthreshold = 0;
  FLAGS_colorlogtostderr = 1;
  FLAGS_minloglevel = 0;
  FLAGS_logbufsecs = 0;// 立即输出
  google::EnableLogCleaner(7);
}

int main(int argc, char* argv[]) {
  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  init_log();

  LOG(INFO) << "INFO " << 100 << " cookies";
  LOG(WARNING) << "WARNING " << 100 << " cookies";
  LOG(ERROR) << "ERROR " << 100 << " cookies";
  // LOG(FATAL) << "FATAL " << 100 << " cookies";

  LOG(INFO) << "add(1,3)=" << add(1, 3);

  return 0;
}
```

