# CMake

## 教程

- [bustub](https://github.com/cmu-db/bustub) **优先参考这个工程的写法。**
- [CMake Primer](https://llvm.org/docs/CMakePrimer.html)
- [**CMake Tutorial**](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- [**cmake_example**](#cmake-example)  这个是常用的示例
- [CMake入门实战](https://github.com/wzpan/cmake-demo)

### cmake example

- [github address](https://github.com/ttroy50/cmake-examples/tree/v2.0.0)
- [backup address](https://1drv.ms/u/s!AiOHW4QDJaFOgQBuv3-Kj6sTYVMH?e=GGfDUw)
- [cmake example readme](./cmake_example.md)

## cmake基础

### 基本语法特性

- 指定(参数1  参数2 ...)

  - 参数使用括弧括起。
  - 参数之间使用空格。
- 指令是大小写无关的，参数和变量是大小写相关的。
- 变量使用${}方式取值，但是在IF控制语句中是直接使用变量名。

### 重要指令

- cmake_minimum_required 指定CMake的最小版本要求。

  ```cmake
  cmake_minimum_required(VERSION 3)
  ```
- project定义工程名称，并可指定工程支持的语言。

  ```cmake
  project(HELLOWORLD)
  ```
- set显示的定义变量

  `set(VAR [VALUE] [CACHE TYPE DOCSTRING [FORCE]])`

  ```cmake
  set(SRC hello.cpp main.cpp)
  ```
- include_directories 向工程添加多个特定的头文件搜索路径

  `include_directories([AFTER|BEFORE] [SYSTEM] dir1 dir2)`
- 
- link_directories 向工程众添加多个特定的库文件搜索路径。

  `link_directories(dir1 dir2)`

  ```cmake
  link_directories(./lib)
  ```
- add_library 生成库文件

  `add_library(libname [SHARE|STATIC|MODULE] [EXCLUDE_FROM_ALL] source1 source2 ... sourceN)`

  ```cmake
  # libhello.so
  set(SRC hello.cpp main.cpp)
  add_library(hello SHARED ${SRC})
  ```
- add_compile_options 添加编译参数

  ```cmake
  add_compile_options(-Wall -std=c++11 -O2)
  ```
- add_executable生成可执行文件

  ```cmake
  add_executable(hello ${SRC})
  ```
- target_link_libraries 为target添加需要链接的库

  target_link_libraries(target library1 library2 ...)
- add_subdirectory 向当前工程添加子目录
- aux_source_directory 发现一个目录下所有的源代码文件并将列表存储在一个变量中。

  `aux_source_directory(dir VARIBLE) `

  ```cmake
  aux_source_directory(. SRC)
  add_executable(main ${SRC})
  ```
- add_definitions 定义编译宏

  ```cmake
  # c++
  #ifdef DEBUG
  std::cout << "debug...\n";
  #endif

  # cmake 
  add_definitions(-DDEBUG)
  ```

### 常用变量

- g++编译选项

  ```cmake
  # 在CMAKE_CXX_FLAGS编译选项后追加-std=c++11
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  ```
- CMAKE_C_COMPILER：指定C编译器
- CMAKE_CXX_COMPILER：指定C++编译器
- CMAKE_BUILD_TYPE

  ```cmake
  # Debug/Release
  set(CMAKE_BUILD_TYPE Debug)
  ```

## note

### windows

- 设置源码编码/字符集

  ```md
  if(MSVC)
     add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
     add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
     add_definitions(-DUNICODE -D_UNICODE)
  endif()
  ```
- [MT/MTD](https://stackoverflow.com/a/60844772/8330816)

  ```cmake
  if(MSVC)
   add_compile_options(
       $<$<CONFIG:>:/MT> #---------|
       $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
       $<$<CONFIG:Release>:/MT> #--|
   )
   endif()
  ```
- windows设置动态库设置导出库

  ```cmake
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
  ```

### 进阶语法

- 打印日志message

  ```cmake
  set(var_name var1)
  message("var_name= ${var_name}")

  set(${var_name} foo) # same as "set(var1 foo)"
  message("var1= ${var1}")

  set(${${var_name}}_var bar) # same as "set(foo_var bar)"
  message("foo_var= ${foo_var}")
  ```
- list

  ```cmake
  set(my_list a b c d)
  set(a 1 2 3)
  set(b 4 5 6)
  set(c 7 8 9)
  foreach(list_name IN LISTS my_list)
      foreach(val IN LISTS ${list_name})
          message("${val} ")
      endforeach()
  endforeach()

  foreach(list_name IN LISTS my_list)
      message("${${list_name}}")
  endforeach()

  foreach(var IN ITEMS foo bar baz)
     message(${var})
  endforeach()

  foreach(var IN LISTS my_list ITEMS out_of_bounds)
  message(${var})
  endforeach()
  ```
- range

  ```cmake
  foreach(val RANGE 0 5 2)
     message("${val}")
   endforeach()
  ```
- function

  ```cmake
  function(add_deps target)
    message("argn=${ARGN}\n")

    foreach(val IN ITEMS ${ARGV})
      message("${val}")
    endforeach()

    if(<condition>)
      message("do stuff")
    elseif(<condition>)
      message("do other stuff")
    else()
      message("do other other stuff")
    endif()

  endfunction()
  ```
- 遍历变量

  ```cmake
  get_cmake_property(_variableNames VARIABLES)
  foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
  endforeach()
  ```
- initial cache and force

  如果定义了大量缓存变量，使用-D不太方便，往往使用-C

  ```shell
  # cache.cmake

  set(A "123" CACHE STRING "")
  set(B "456" CACHE STRING "")
  set(C "789" CACHE STRING "")

  # CMakeLists.txt
  cmake_minimum_required(VERSION 2.8)
  project(foo NONE)

  message("A: ${A}")
  message("B: ${B}")
  message("C: ${C}")

  [usage-of-variables]> rm -rf _builds
  [usage-of-variables]> cmake -C initial-cache/cache.cmake -Hinitial-cache -B_builds
  loading initial cache file initial-cache/cache.cmake
  A: 123
  B: 456
  C: 789
  -- Configuring done
  -- Generating done
  -- Build files have been written to: /.../usage-of-variables/_builds
  ```

  force: 被认为是设计不良的cmake 代码

  If you want to set cache variable even if it’s already present in cache file you can add `FORCE`:

  ```shell
  cmake_minimum_required(VERSION 2.8)
  project(foo NONE)

  set(A "123" CACHE STRING "" FORCE)
  message("A: ${A}")

  [usage-of-variables]> rm -rf _builds
  [usage-of-variables]> cmake -DA=456 -Hforce -B_builds
  A: 123
  -- Configuring done
  -- Generating done
  -- Build files have been written to: /.../usage-of-variables/_builds
  ```
- 使用环境变量做判断，交叉编译时使用。

  ```cmake
  if("$ENV{PLATFORM}" MATCHES "amd64")
     message(STATUS ---arm64---")
  elseif("$ENV{PLATFORM}" MATCHES "arm64")
     message(STATUS "---arm64---")
  elseif("$ENV{PLATFORM}" MATCHES "mips64")
     message(STATUS "---mips64---")
  elseif("$ENV{PLATFORM}" MATCHES "loongarch64")
     message(STATUS "---loongarch64---")
  else()
     message(STATUS "unknown arch")
  endif()
  ```

## 生成平台相关的编译

1. windows:

   ```cmd
   mkdir build
   cd build
   cmake -H.. -B. -G "Visual Studio 14 2015 Win64" -T v140_xp
   cmake --build . --config Release --verbose
   ```

   说明：

   **不指定--config 默认是debug。**

   **-H"source_path" -B"Build path"**，可以简写成 `cmake .. -G "Visual Studio 14 2015 Win64" -T v140_xp`。

   ```txt
   cmake --help
   The following generators are available on this platform (* marks default):
   * Visual Studio 16 2019        = Generates Visual Studio 2019 project files.
                                    Use -A option to specify architecture.
   Visual Studio 15 2017 [arch] = Generates Visual Studio 2017 project files.
                                    Optional [arch] can be "Win64" or "ARM".
   Visual Studio 14 2015 [arch] = Generates Visual Studio 2015 project files.
                                    Optional [arch] can be "Win64" or "ARM".
   Visual Studio 12 2013 [arch] = Generates Visual Studio 2013 project files.
                                    Optional [arch] can be "Win64" or "ARM".
   Visual Studio 11 2012 [arch] = Generates Visual Studio 2012 project files.
                                    Optional [arch] can be "Win64" or "ARM".
   Visual Studio 10 2010 [arch] = Generates Visual Studio 2010 project files.
                                    Optional [arch] can be "Win64" or "IA64".
   Visual Studio 9 2008 [arch]  = Generates Visual Studio 2008 project files.
                                    Optional [arch] can be "Win64" or "IA64".
   Borland Makefiles            = Generates Borland makefiles.
   NMake Makefiles              = Generates NMake makefiles.
   NMake Makefiles JOM          = Generates JOM makefiles.
   MSYS Makefiles               = Generates MSYS makefiles.
   MinGW Makefiles              = Generates a make file for use with
                                    mingw32-make.
   ...
   ```
2. linux

   ```shell
   mkdir build
   cd build
   cmake .. -DCMAKE_BUILD_TYPE=Debug
   make
   ```

   默认是Release。
3. 指定[rpath](https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling)

   ```CMakeLists.txt
   # use, i.e. don't skip the full RPATH for the build tree
   set(CMAKE_SKIP_BUILD_RPATH TRUE)

   # -Wl,--fatal-warnings            = Treat warnings as errors
   set(CMAKE_CXX_FLAGS "-Wl,--fatal-warnings")

   # -Wl,-rpath,.                    = Set rpath so that libraries can be placed next to the executable
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--disable-new-dtags -Wl,-rpath,'$ORIGIN':'$ORIGIN'/lib")
   ```

使用make VERBOSE=1 可以查看详细编译过程。

## Other

[Useful Variables](https://gitlab.kitware.com/cmake/community/wikis/doc/cmake/Useful-Variables)

[cmake 添加头文件目录，链接动态、静态库](https://www.cnblogs.com/binbinjx/p/5626916.html)

[How do I activate C++ 11 in CMake?](https://stackoverflow.com/questions/10851247/how-do-i-activate-c-11-in-cmake)
