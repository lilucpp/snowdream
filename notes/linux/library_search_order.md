# 库搜索目录

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

关于动态库调试，rpath修改相关的命令。<br>
- chrpath： 可以更改、删除rpath路径。
- patchelf：可以添加rpath路径。<br>
  patchelf --set-rpath "\$ORIGIN"  ./*.so. <br>
  **注意点：在mips64上如果一开始没有rpath，使用命令添加，会破坏二进制文件，导致不可用。**

## 工程实践

LD_DEBUG=libs  progname：可以查看动态库搜索过程。<br>

**如果a.out 依赖 a.so ，a.so依赖b.so，可以为a.so增加rpath。**

##  rpath vs runpath
1. 搜索顺寻：rpath > LD_LIBRARY_PATH > runpath <br>
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

## preload hook
一种是环境变量配置(LD_PRELOAD)；另一种是文件配置：(/etc/ld.so.preload)。<br>
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

