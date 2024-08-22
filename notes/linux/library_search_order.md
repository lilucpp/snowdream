# 库搜索目录

## RPATH

What is RPATH?

   If an executable "foo" links to the shared library "bar", the library "bar" has to be found and loaded when the executable "foo" is executed. This is the job of the linker, under Linux this is usually ld.so. The linker searches a set of directories for the library bar, which will under most UNIXes have the name libbar.so. The linker will search the libraries in the following directories in the given order:

- RPATH - a list of directories which is linked into the executable, supported on most UNIX systems. It is ignored if RUNPATH is present.
- LD_LIBRARY_PATH - an environment variable which holds a list of directories
- RUNPATH - same as RPATH, but searched after LD_LIBRARY_PATH, supported only on most recent UNIX systems, e.g. on most current Linux systems
- /etc/ld.so.conf - configuration file for ld.so which lists additional library directories
- builtin directories - basically /lib and /usr/lib

## 工程中rpath设置

### qt

```txt
unix:{
    # add your own with quoting gyrations to make sure $ORIGIN gets to the command line unexpanded
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\':\'\$$ORIGIN\'/lib"
}
```

### cmake

```CMakeLists.txt
# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH TRUE)

# -Wl,--fatal-warnings            = Treat warnings as errors
set(CMAKE_CXX_FLAGS "-Wl,--fatal-warnings")

# -Wl,-rpath,.                    = Set rpath so that libraries can be placed next to the executable
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--disable-new-dtags -Wl,-rpath,'$ORIGIN':'$ORIGIN'/lib")
```

使用make VERBOSE=1 可以查看详细编译过程。

### makefile

-Wl,-rpath,'$$ORIGIN'

### configure文件

```
./configure --with-openssl  --prefix=${outputBuild}/${output}/ --host=${BUILDARCH} LDFLAGS="-Wl,-rpath=\\\$\$ORIGIN  -ldl -pthread ${LDFLAGS} -L${output}/lib"
```

## rpath相关的命令

关于动态库调试，rpath修改相关的命令。`<br>`

- chrpath： 可以更改、删除rpath路径。
- patchelf：可以添加rpath路径。`<br>`
  patchelf --set-rpath "\$ORIGIN"  ./*.so. `<br>`
  **注意点：在mips64上如果一开始没有rpath，使用命令添加，会破坏二进制文件，导致不可用。**

## 工程实践

LD_DEBUG=libs  progname：可以查看动态库搜索过程，一般把输出打印到文件，方便查看。 `LD_DEBUG=libs  progname &> 1.txt` 。  

qt中加载插件也有个类似的环境变了， export QT_DEBUG_PLUGINS=1   然后执行qt进程，可以打印出插件加载过程。  

**如果a.out 依赖 a.so ，a.so依赖b.so，可以为a.so增加rpath。**

## rpath vs runpath

1. 搜索顺寻：rpath > LD_LIBRARY_PATH > runpath `<br>`
   设置LD_LIBRARY_PATH后，如果不生效。可以使用下面命令，因为系统不知道该目录中有哪些库可用。[https://askubuntu.com/a/302900](https://askubuntu.com/a/302900)。
   ```shell
   sudo ldconfig /usr/local/lib
   ```
2. [编译指定](https://stackoverflow.com/a/52020177/8330816)
   ```txt
   -Wl,--disable-new-dtags to tell the new linker to use the old behavior, i.e. RPATH.
   Use -Wl,--enable-new-dtags to tell the old linker to use the new behavior, i.e. RUNPATH.
   readelf -d prog | grep PATH：可以查看当前是RPATH还是RUNPATH.
   ```
3. 验证参考此链接：https://medium.com/nataraj-raghavendra/rpath-vs-runpath-883029b17c45

## linux上常见的加载顺序

- rpath 列表(不包含runpath列表)
- LD_LIBRARY_PATH中的路径列表（OSX上是DYLD_LIBRARY_PATH）
- 可执行文件中的runpath
- 检查/etc/ld.so.cache文件以确认是否包含了库相关的条目
- 系统搜索目录如/lib、/usr/lib、/usr/local/lib

注意事项：

- 如果子依赖也设置了rpath，那么子依赖的rpath优先查找。这样会导致一个后果，如果主进程依赖一个库，子依赖也依赖这个库，两个版本不一致时容易出问题。  如主进程依赖当前目录下的一个libssl.so， 子依赖也依赖libssl.so， 如果这是子依赖设置的rpath路径是系统的某个目录，就会使用系统libssl.so。可能导致主进程无法启动。
  可以尝试使用LD_LIBRARY_PATH的形式，或者LD_PRELOAD=/xx.so   a.out

## preload hook

一种是环境变量配置(LD_PRELOAD)；另一种是文件配置：(/etc/ld.so.preload)。`<br>`

- 若使用命令行指定LD_PRELOAD则只影响该新进程及子进程；若写入全局环境变量则LD_PRELOAD对所有新进程生效；父进程可以控制子进程的环境变量从而取消preload
- 文件preload方式影响所有新进程且无法被取消
- 可以拦截到系统调用和普通库函数
- 实现和操作最为简单，只需要编写同名系统调用函数即可实现hook
- 可以使用动态调用方式或自定义实现方式绕过

进程在启动后，会按照一定顺序加载动态库：

- 加载环境变量LD_PRELOAD指定的动态库
- 加载文件/etc/ld.so.preload指定的动态库
- 搜索环境变量LD_LIBRARY_PATH指定的动态库搜索路径

## 参考资料

linux上共享库所涉及的机制可以参考下面的资料

- linux/unix系统编程手册（下） 第42章 共享库基础
- [shared_library.md](../linux/shared_library.md)
- [dynamic loader manual](https://man7.org/linux/man-pages/man8/ld.so.8.html)
- [shared library HOWTO](https://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html)
- [preload](https://www.jianshu.com/p/f78b16bd8905)
