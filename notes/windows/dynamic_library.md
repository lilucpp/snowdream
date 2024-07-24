# 动态库

## __stdcall vs __cdecl
在于清理栈的方式不同，优先使用__cdecl，win32相关的函数使用__stdcall。

```c++
#if definded(_WIN32) && !defined(_WIN64)
# define STDCALL __stdcall
#else 
# define STDCALL 
#endif
```

## 符号导出

有四种符号导出方法，推荐顺序如下：
1. 在源码中使用关键词 __declspec(dllexport)
2. 在.def文件中使用EXPORT
3. link命令中的/EXPORT
4. #pragma comment(linker, "/export: definition ")

个人使用1或2，3、4没有使用过。

**如果导出函数声明包含 __stdcall,想导出c接口的函数名字必须使用.def文件。**

- cmake中添加def文件：直接按常规的cpp文件添加即可。
- qmake中添加def文件：DEF_FILE  += export.def

## 其他

### 导出.lib文件

You're going to need [Microsoft Visual C++ 2010 Express](http://www.microsoft.com/visualstudio/eng/downloads) (or any other source of MSVC command line tools), and your DLL.

Steps:

1. `dumpbin /EXPORTS yourfile.dll > yourfile.exports`
2. Paste the names of the needed functions from `yourfile.exports` into a new `yourfile.def` file. Add a line with the word `EXPORTS` at the top of this file.
3. Run the following commands from `VC\bin` directory (the one where `lib.exe` and other compile tools reside).

 

```cpp
 vcvars32.bat

 lib /def:yourfile.def /out:yourfile.lib
```

or for x64 builds

```cpp
 lib /def:yourfile.def /machine:x64 /out:yourfile64.lib
```

You should get two files generated: `yourfile.lib` and `yourfile.exp`

[Share](https://stackoverflow.com/a/16127548/8330816)


### 示例1：
```c++
#ifdef TESTDLL_EXPORTS
#define TESTDLL_API __declspec(dllexport)
#else
#define TESTDLL_API __declspec(dllimport)
#endif

//...

extern TESTDLL_API int ntestdll;

TESTDLL_API int fntestdll(void);
```
然后在动态库工程 c/c++ 预处理器  预处理器定义中定义宏TESTDLL_EXPORTS。

隐式使用动态库时，包含头文件，链接时添加到出库libranam.lib即可。

显示使用动态库时，使用GetProcAddress()获取符号地址时提示找不到。原因是符号会被编译器修复名字。使用dumpbin /export testdll.dll可以查看符号名。
```bat
P:\tmp\testdll\Debug>dumpbin /exports testdll.dll
Microsoft (R) COFF/PE Dumper Version 14.00.24215.1
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file testdll.dll

File Type: DLL

  Section contains the following exports for testdll.dll

    00000000 characteristics
    602F1DB1 time date stamp Fri Feb 19 10:08:49 2021
        0.00 version
           1 ordinal base
           7 number of functions
           7 number of names

    ordinal hint RVA      name
...
          5    4 00011087 ?fntestdll@@YAHXZ = @ILT+130(?fntestdll@@YAHXZ)
          6    5 00018138 ?ntestdll@@3HA = ?ntestdll@@3HA (int ntestdll)
          7    6 000110D2 ?test@Ctestdll@@QAEXXZ = @ILT+205(?test@Ctestdll@@QAEXXZ)

  Summary

        1000 .data
        1000 .idata
        2000 .rdata
        1000 .reloc
        1000 .rsrc
        5000 .text
       10000 .textbss
```
符号fntestdll变成了?fntestdll@@YAHXZ。

可以使用extern "C" {} 把要导出的符号包含起来。
```bat
...
 ordinal hint RVA      name
 ...
          6    5 0001116D fntestdll = @ILT+360(_fntestdll)
          7    6 00017138 ntestdll = _ntestdll
...
```
### 示例2
需要在 项目>属性>链接器>输入>模块定义文件 输入def文件的名称。
```c++
int fntestdll(void);

extern int ntestdll;

```
.def 文件
```def
; testdefdll.def : 声明 DLL 的模块参数。
LIBRARY      "testdefdll"

EXPORTS
    ; 此处可以是显式导出
    fntestdll @1
    ntestdll @2
```

```bat
 ordinal hint RVA      name

          1    0 000112BC fntestdll = @ILT+695(?fntestdll@@YAHXZ)
          2    1 00023328 ntestdll = ?ntestdll@@3HA (int ntestdll)
```
### 示例3
类导出
```c++
// testdll.h
...
extern "C" {
class TESTDLL_API Ctestdll {
public:
    Ctestdll(void);
    // 如果不适用需要，将执行静态链接，提示找到不到符号。
    virtual void test();
};

TESTDLL_API Ctestdll* create_object();
TESTDLL_API void destroy_object( Ctestdll* object );
...

// testdll.cpp
...
Ctestdll* create_object() {
	return new Ctestdll;
}

void destroy_object( Ctestdll* object ) {
	delete object;
}

Ctestdll::Ctestdll() {
	return;
}

void Ctestdll::test() {
	printf("Ctestdll::test\n");
}

// main.cpp
...
typedef Ctestdll* (*my_create_object)();
typedef void (*my_destroy_object)(Ctestdll*);

my_create_object cr = (my_create_object) GetProcAddress(hlib, "create_object");
if(NULL == cr) {
    printf("load address create_object fail:%lu\n", GetLastError());
    return;
}

my_destroy_object des = (my_destroy_object) GetProcAddress(hlib, "destroy_object");
if(NULL == des) {
    printf("load address destroy_object fail:%lu\n", GetLastError());
    return;
}

Ctestdll* testdll = cr();
testdll->test();
des(testdll);

...
```

参考资料：

[1] https://docs.microsoft.com/en-us/cpp/build/reference/exports?view=msvc-160

[2] https://stackoverflow.com/a/497158/8330816
