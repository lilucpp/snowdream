# CMake

## Guide
- [bustub](https://github.com/cmu-db/bustub) **优先参考这个工程的写法。**
- [CMake Primer](https://llvm.org/docs/CMakePrimer.html)
- [**CMake Tutorial**](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- [**cmake_example**](#cmake-example)  这个是常用的示例
- [CMake入门实战](https://github.com/wzpan/cmake-demo)



### cmake example

- [github address](https://github.com/ttroy50/cmake-examples/tree/v2.0.0)

- [backup address](https://1drv.ms/u/s!AiOHW4QDJaFOgQBuv3-Kj6sTYVMH?e=GGfDUw)

[cmake example readme](./cmake_example.md)

### note
- 设置头文件/库目录
    ```md
    include_directories(C:/PeanutLibV140/include)
    link_directories(C:/PeanutLibV140/lib)
    ```
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
-  打印日志message
    ```cmake
    set(var_name var1)
    message("var_name= ${var_name}")

    set(${var_name} foo) # same as "set(var1 foo)"
    message("var1= ${var1}")

    set(${${var_name}}_var bar) # same as "set(foo_var bar)"
    message("foo_var= ${foo_var}")
    ```
-  list
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
-  range
   ```cmake
   foreach(val RANGE 0 5 2)
      message("${val}")
    endforeach()
   ```
-  function
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
-  遍历变量
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

### 生成平台相关的编译

1. windows:

   ```cmd
   mkdir build
   cd build
   cmake -H.. -B. -G "Visual Studio 14 2015 Win64" -T v140_xp
   cmake --build . --config Release --verbose
   ```

   说明：

   **不指定--config 默认是debug。**

   **-H"source_path" -B"Build path"**，可以简写成`cmake .. -G "Visual Studio 14 2015 Win64" -T v140_xp`。

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


   What is RPATH?

   If an executable "foo" links to the shared library "bar", the library "bar" has to be found and loaded when the executable "foo" is executed. This is the job of the linker, under Linux this is usually ld.so. The linker searches a set of directories for the library bar, which will under most UNIXes have the name libbar.so. The linker will search the libraries in the following directories in the given order:

   - RPATH - a list of directories which is linked into the executable, supported on most UNIX systems. It is ignored if RUNPATH is present.
   - LD_LIBRARY_PATH - an environment variable which holds a list of directories
   - RUNPATH - same as RPATH, but searched after LD_LIBRARY_PATH, supported only on most recent UNIX systems, e.g. on most current Linux systems
   - /etc/ld.so.conf - configuration file for ld.so which lists additional library directories
   - builtin directories - basically /lib and /usr/lib
  
  

### Other

[Useful Variables](https://gitlab.kitware.com/cmake/community/wikis/doc/cmake/Useful-Variables)

[cmake 添加头文件目录，链接动态、静态库](https://www.cnblogs.com/binbinjx/p/5626916.html)

[How do I activate C++ 11 in CMake?](https://stackoverflow.com/questions/10851247/how-do-i-activate-c-11-in-cmake)