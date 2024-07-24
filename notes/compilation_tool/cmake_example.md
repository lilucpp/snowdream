# CMake Examples

Table of Contents

- [Introduction](https://github.com/ttroy50/cmake-examples/tree/v2.0.0#introduction)
- [Requirements](https://github.com/ttroy50/cmake-examples/tree/v2.0.0#requirements)
- [Other Links](https://github.com/ttroy50/cmake-examples/tree/v2.0.0#other-links)

# Introduction

[CMake](https://cmake.org/) is a cross-platform open-source meta-build system which can build, test and package software. It can be used to support multiple native build environments including make, Apple’s xcode and Microsoft Visual Studio.

This repository includes some example CMake configurations which I have picked up when exploring it’s usage for various projects. The examples are laid out in a tutorial like format. The first examples are very basic and slowly increase in complexity drawing on previous examples to show more complex use cases.

These examples have been tested on Ubuntu 14.04 but should work under any Linux system that supports CMake.

# Requirements

The basic requirements for most examples are:

- CMake
- A c++ compiler (defaults to gcc)
- make

The easiest way to install the above on Ubuntu is as follows

```
$ sudo apt-get install build-essential
$ sudo apt-get install cmake
```

Some specific examples may require other tools including:

- [boost](http://www.boost.org/)

  ```
  $ sudo apt-get libboost-all-dev
  ```

- [protobuf](https://github.com/google/protobuf)

  ```
  $ sudo apt-get install libprotobuf-dev
  $ sudo apt-get install protobuf-compiler
  ```

- [cppcheck](http://cppcheck.sourceforge.net/)

  ```
  $ sudo apt-get install cppcheck
  ```

- [clang](http://clang.llvm.org/)

  ```
  $ sudo apt-get install clang-3.6
  ```

- [ninja](https://ninja-build.org/)

  ```
  $ sudo apt-get install ninja-build
  ```

# Other Links

There are many CMake tutorials and examples online. The list below includes links to some of these which I have found helpful in my CMake journey.

- [Official CMake Tutorial](https://cmake.org/cmake-tutorial/)
- [Official CMake Wiki](https://cmake.org/Wiki/Main_Page)
- [CMake Useful Variables](https://cmake.org/Wiki/CMake_Useful_Variables)
- [Derek Molloy - Intro to CMake](http://derekmolloy.ie/hello-world-introductions-to-cmake/)
- [Modular C++ Projects](http://techminded.net/blog/modular-c-projects-with-cmake.html)
- [Common CMake Anti-Patterns](http://voices.canonical.com/jussi.pakkanen/2013/03/26/a-list-of-common-cmake-antipatterns/)
- [Using clang static analyser with CMake](http://baptiste-wicht.com/posts/2014/04/install-use-clang-static-analyzer-cmake.html)
- [Static Analysis with CDash](https://cmake.org/pipermail/cmake/2011-April/043709.html) - Includes some info about using CppCheck with CMake
- [CppCheck Targets](https://www.openfoundry.org/svn/cms/trunk/cmake/CppcheckTargets.cmake)
- [CMake Tips](https://samthursfield.wordpress.com/2015/10/20/some-cmake-tips/)
- [John Lamp - CMake Tutorial](https://www.johnlamp.net/cmake-tutorial.html)