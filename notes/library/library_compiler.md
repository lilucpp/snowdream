# 常见库编译
## 依赖工具安装
安装 ActivePerl-5.26.1.2601-MSWin32-x64-404865.exe

## openssl 静态库编译
- 打开vs 命令行
- 配置项目

```bat
cd G:\project\openssl-OpenSSL_1_0_2o
# 32位编译
perl Configure VC-WIN32 no-asm no-shared --prefix="G:\project\openssl-OpenSSL_1_0_2o\openssl-static-out"
ms\do_nt.bat

# 64位编译
perl Configure VC-WIN64A no-asm no-shared --prefix="G:\project\openssl-OpenSSL_1_0_2o\openssl-static-x64-out"
# a: amd64或英特尔系列
ms\do_win64a.bat
```

- 编译  

```sh
# 清理之前的编译
nmake -f ms\nt.mak clean

# MT 链接
nmake -f ms\nt.mak
nmake -f ms\nt.mak install

# MD 链接
nmake -f ms\ntdll.mak
nmake -f ms\ntdll.mak install
```

## curl 编译

- 配置项目

```sh
curl-7_40_0\buildconf.bat  
curl-7_40_0\projects\generate.bat  
```

- 打开 curl-7_40_0\projects\Windows\VC9\curl.sln

> curlsrc 、libcurl 两个项目做同样的配置
```
配置                       LIB Release - LIB OpenSSL | Win32
C/C++ > 常规 > 附加包含目录   openssl-static-out\include
链接器 > 常规 > 附加库目录     openssl-static-out\lib
C/C++ > 代码生成 > 运行库      多线程 (/MT)
```

- 编译

> 输出目录：curl-7_40_0\build\Win32\VC9\LIB Release - LIB OpenSSL\

### curl 使用方法

- 查看帮助文档
  ```sh
  $ curl.exe -h al
  # 以下是部分帮助信息，显示了 TLS 版本参数
  -2, --sslv2         Use SSLv2 (SSL)
  -3, --sslv3         Use SSLv3 (SSL)
  -1, --tlsv1         Use => TLSv1 (SSL)
      --tlsv1.0       Use TLSv1.0 (SSL)
      --tlsv1.1       Use TLSv1.1 (SSL)
      --tlsv1.2       Use TLSv1.2 (SSL)
  ```
- 上传文件
  ```sh
  curl.exe -k -g -T "1.txt" https://user:passwd@127.0.0.1:666/webdav/1.txt
  ```

## sqlcipher编译

修改Makefile.msc文件

```txt
# Flags controlling use of the in memory btree implementation
#
# SQLITE_TEMP_STORE is 0 to force temporary tables to be in a file, 1 to
# default to file, 2 to default to memory, and 3 to force temporary
# tables to always be in memory.
#
TCC = $(TCC) -DSQLITE_TEMP_STORE=1
RCC = $(RCC) -DSQLITE_TEMP_STORE=1
```

替换为

```txt
# Flags controlling use of the in memory btree implementation
#
# SQLITE_TEMP_STORE is 0 to force temporary tables to be in a file, 1 to
# default to file, 2 to default to memory, and 3 to force temporary
# tables to always be in memory.
#
TCC = $(TCC) -DSQLITE_TEMP_STORE=2
RCC = $(RCC) -DSQLITE_TEMP_STORE=2

TCC = $(TCC) -DSQLITE_HAS_CODEC
RCC = $(RCC) -DSQLITE_HAS_CODEC

!IF "$(PLATFORM)"=="x64"
TCC = $(TCC) -I"G:\project\openssl-OpenSSL_1_0_2o\openssl-static-x64-out\include"
RCC = $(RCC) -I"G:\project\openssl-OpenSSL_1_0_2o\openssl-static-x64-out\include"
!ELSEIF "$(PLATFORM)"=="x86"
TCC = $(TCC) -I"G:\project\openssl-OpenSSL_1_0_2o\openssl-static-out\include"
RCC = $(RCC) -I"G:\project\openssl-OpenSSL_1_0_2o\openssl-static-out\include"
!ENDIF

!IF "$(PLATFORM)"=="x64"
LTLIBPATHS = $(LTLIBPATHS) /LIBPATH:"G:\project\openssl-OpenSSL_1_0_2o\openssl-static-x64-out\lib"
LTLIBS = $(LTLIBS) libeay32.lib ssleay32.lib
!ELSEIF "$(PLATFORM)"=="x86"
LTLIBPATHS = $(LTLIBPATHS) /LIBPATH:"G:\project\openssl-OpenSSL_1_0_2o\openssl-static-out\lib"
LTLIBS = $(LTLIBS) libeay32.lib ssleay32.lib
!ENDIF

LTLIBS = $(LTLIBS) WS2_32.Lib Gdi32.Lib AdvAPI32.Lib Crypt32.Lib User32.Lib
```

### 86编译

- 打开vs 的x86命令行
- set PLATFORM=x86
- nmake /f Makefile.msc

### 64编译

如果64位编译想修改输出的动态库名字，需要修改SQLITE3DLL、SQLITE3LIB、SQLITE3EXE、SQLITE3EXEPDB。

```txt
SQLITE3DLL = sqlite3_x64.dll
SQLITE3LIB = sqlite3_x64.lib
SQLITE3EXE = sqlite3_x64.exe
SQLITE3EXEPDB = /pdb:sqlite3sh_x64.pdb
```

- 打开vs的x64命令行
- set PLATFORM=x64
- nmake /f Makefile.msc

## wxsqlite3编译

source code: https://sourceforge.net/projects/wxcode/files/

编译工具：https://github.com/premake/premake-core/releases/tag/4.3

```txt
# 生成vs工程
premake4 --os=linux --platform=x64 vs2008
```

编译器编译即可
