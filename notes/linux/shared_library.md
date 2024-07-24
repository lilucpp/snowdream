# Shared Library

```shell
# gcc -Wall -Wextra -g -fPIC mylib.c  -Wl,-soname,libpeanut.so -shared -o mylib.so
gcc -g -c -fPIC -Wall -Wextra mylib.c
gcc -g -shared -Wl,-soname,libpeanut.so -o libmylib.so mylib.o 
```

## 真实名称
libname.so.major-id.minor-id

共享库的每个不兼容版本是通过一个唯一的主要版本标识符来区分的，这个主要版本标识符是共享库的真实名称的一部分。根据惯例，主要版本标识符由**一个数字**构成，这个数字随着库的每个不兼容版本的发布而顺序递增。 

次要版本标识符，它用来区分库的主要版本中兼容的次要版本。次要版本可以是任意字符串。但根据惯例，它要么是一个数字，要么是两个由点分割的数字，其中第一个数字标识出了次要版本，第二个数字表示该次要版本中的补丁号或修订号。
```txt
libdemo.so.1.0.1
libdemo.so.1.0.2
libdemo.so.2.0.0
libfoo.so.5.0
```
## soname
使用soname的目的是提供一个间接层，使得可执行程序能够在**运行**时使用与**链接**时使用的库不同的（但兼容的）共享库。

共享库的soname包括相应的真实名称中的主要版本标识符，但不包括次要版本标识符。因此soname的形式为libname.so.major-id。

通常，会将soname创建为包含真实名称的目录中的一个ie对应的符号连接。

```txt
libdemo.so.1        -> libdemo.so.1.0.2
libdemo.so.2        -> libdemo.so.2.0.0
libfoo.so.5         -> libfoo.so.5.0
```
通常，每个库的主要版本的soname会指向在主要版本中最新的次要版本（如上面的libdemo.so.1）。由于一个库的不同的不要版本的soname不同，因此他们能够和平地共存并且被需要他们的程序访问。

## 连接器名称
将可执行文件与共享库链接起来时会用到这个名称。连接器名称是一个只包含库名称同时不包含主要或次要版本标识符的符号链接，因此其形式为libname.so。有了链接器名称之后就可以构建能够自动使用共享库的正确版本（最新版本）的独立于版本的链接命令了。

链接器可以连接到真实名称，也可以链接到库的最新主要版本的soname。通常，最好使用指向soname的链接。

如果需要将一个程序与共享库的一个较老的主要版本链接起来，就不能使用连接器名称。相反，在链接命令中需要通过指定具体的真实名称或soname来标示出所需要的版本（主要版本）。
```txt
libdemo.so          -> libdemo.so.2
libfoo.so           -> libfoo.so.5
```
## LD_LIBRARY_PATH VS LIBRARY_PATH
LIBRARY_PATH 在编译前由gcc使用，以搜索包含需要链接到程序的静态库和共享库的目录。

LD_LIBRARY_PATH成功编译和链接后，程序将使用该程序搜索包含共享库的目录。

## 运行时符号解析

1. 主程序中全局符号的定义覆盖库中相应的定义。
2. 如果一个全局符号在多个库中定义，那么对该符号的引用会被绑定到在扫描库时找到的第一个定义，其中扫描顺序是按照这些库在静态链接命令行中列出时从左到右的顺序。
3. -Bsymbolic 链接器选项指定了共享库中对全局符号的引用应该优先绑定到库中的相应的定义上（如果存在的话）。（注意不管是否使用了这个选项，在主程序中调用func总是会调用主程序中定义的func）
   ```shell
   g++ -shared -fPIC -Wl,-Bsymbolic -o libapple.so apple.cpp
   ```
4. 使用dlopen动态加载一个共享库时，会优先使用动态库里的函数，如果要使用主程序中的符号，可在链接程序时使用-Wl,--export-dynamic（或-export-dynamic）选项.

## 静态库
```shell
gcc -c peanut.cpp
ar rcs libpeanut.a peanut.o
```
静态库和动态库同时存在时，程序链接时会优先使用静态库。  
查看静态库架构：`readelf -h ./xx.a`

强制使用静态库：
1. gcc命令行中指定静态库路径名（全民，包括.a扩展）
2. gcc 命令行中使用-static
3. 使用-Wl,-Bstatic和-Wl,-Bdynamic 选项显示地指定连接器选择共享库还是静态库。

## 使用静态链接
-static

## 动态加载
- dlopen()函数打开一个共享库，返回一个供后续使用的句柄
- dlsym()搜索一个符号（函数或变量的字符串）并返回其地址
- dlclose()关闭之前由dlopen()打开的库
- dlerror()返回个错误消息字符串
链接时加上-ldl选项以便与libdl库连接起来。

```c++
#include <stdio.h>
#include <dlfcn.h>
#include <thread>
#include <unistd.h>

void (*my_xyz)();

int main() {
	void* dyhandle = dlopen("./mydylib.so", RTLD_LAZY);
	if(dyhandle == NULL) {
		printf("dlopen fail: %s\n", dlerror());
		return 1;
	}
	
	*(void**)(&my_xyz) = dlsym(dyhandle, "xyz");
	dlerror();
	if(my_xyz == NULL) {
		printf("not found sym xyz\n");
		const char* err = dlerror();
		if(err != NULL) {
			printf("load sym error: %s\n", err);
		}
		return 2;
	}
	Dl_info info;

	if(dladdr("xyz", &info) == 0) {
		printf("dladdr xyz fail: %s\n", dlerror());
		return 3;
	}

	printf("fli_fname=%s\n fli_fbase=%0x\n sli_sname=%s\n dli_asddr=%0x\n", info.dli_fname,
	info.dli_fbase, info.dli_sname, info.dli_saddr);

	std::thread t(my_xyz);

	usleep(3000*1000);
   // 关闭句柄后，使用中的动态库函数会崩溃。
	//if(dlclose(dyhandle) != 0) {
	//	printf("close library error: %s\n", dlerror());
	//}

	t.join();

	return 0;
}
```
## 符号的可见性
1. 在程序中可以使用static关键词使得一个符号私有于一个源代码模块，从而使得它无法被其他目标文件绑定。
2. gcc提供了特有的特性声明
  ```c++
  void __attribute__ ((visibility("hidden"))) func(void) {
     /* Code */
  }
  ```
  static关键词将一个符号的可见性限制在单个源代码文件中，而hidden特性使得一个符号对构成共享库的所有源代码文件都可见，但对库之外的文件不可见。
3. 版本脚本可以用来精确控制符号的可见性以及选择将一个引用绑定到符号的那个版本。

## 连接器版本脚本

要使用版本脚本必须要指定--version-script连接器选项。
```shell
$ gcc -Wl,--version-script,myscriptfile.map ...
```
版本脚本的后缀通常（但不统一）是.map。在版本脚本中，#开启了一段注释。

符号版本化允许一个共享库提供同一个函数的多个版本。

```shell
peanut@peanut:~/demo/symbol_visual$ cat sv_v1.map 
VER_1{
	global:
		xyz;
	local:
		*;
};
peanut@peanut:~/demo/symbol_visual$ cat sv_v2.map 
VER_1 {
	global: 
		xyz;
	local:
		*;
};

VER_2 {
	global:
		pqr;
} VER_1;

peanut@peanut:~/demo/symbol_visual$ cat sv_lib_v1.cpp 
#include <stdio.h>

extern "C" {

	void xyz(void) { printf("v1 xyz\n"); }
}
peanut@peanut:~/demo/symbol_visual$ cat sv_lib_v2.cpp 
#include <stdio.h>

__asm__(".symver xyz_old,xyz@VER_1");
__asm__(".symver xyz_new,xyz@@VER_2");

extern "C" {
void xyz_old(void) { printf("v1 xyz\n"); }

void xyz_new(void) { printf("v2 xyz\n"); }

void pqr(void) { printf("v2 pqr\n"); }

}
peanut@peanut:~/demo/symbol_visual$ cat main.cpp 
#include <stdlib.h>

extern "C" {
	void xyz(void);
}

int main() {
	xyz();

	return 0;
}

g++ -fPIC -shared -o libsv.so -Wl,--version-script,sv_v1.map sv_lib_v1.cpp  
g++ -o p1 main.cpp  libsv.so
g++ -fPIC -shared -o libsv.so -Wl,--version-script,sv_v2.map sv_lib_v2.cpp 
g++ -o p2 main.cpp  libsv.so
LD_LIBRARY_PATH=. ./p1
v1 xyz

LD_LIBRARY_PATH=. ./p2
v2 xyz
```

## 初始化和终止函数
可以定义一个或多个在共享库被加载和卸载时自动执行的函数，这样在使用共享库时就能够完成一些初始化和终止工作了。不管是自动加载还是dlopen接口显示加载，初始化和终止函数都会被执行。
```c++
void __attribute__ ((constructor)) some_name_load(void) { 
   //  initialization code 
}

void __attribute__ ((destructor)) some_naem_unload(void) {
   // finalization code
}
```

_init()和_fini()

较早的技术。如果创建了_init()和_fini()函数，那么在构建时需要指定gcc -nostartfiles选项以防止连接器加入这些函数的默认实现。
如果需要的话可以使用-Wl,--init和-Wl,--fini连接器选项来指定函数的名字。自从有了gcc的constructor和destructor特性之后已经不建议
使用_init()和_fini()函数了。

## 预加载共享库
出于测试目的，有些时候可以有选择的覆盖一些正常情况下会被连接器按照规则找出的函数。可以使用环境变量LD_PRELOAD,其值由在加载
其他共享库之前需要加载的共享库名称构成，其中共享库名称之间用空格或冒号分隔。

出于安全原因，set-user-ID和set-group-ID程序忽略了LD_PRELOAD.

## 监控动态连接器：LD_DEBUG
有些时候需要监控动态链接器的操作以弄清楚它在搜索那些库，可以使用LD_DEBUG环境变量来完成。
LD_DEBUG=libs ./a.out

出于安全考虑，set-user-ID和set-group-ID程序忽略了LD_DEBUG.

## 导出函数
摘录：https://blog.csdn.net/zdragon2002/article/details/6061962  
1. 在方法声明定义时，加修饰：__attribute__((visibility("hidden")))
就是说将不公开的函数都加上这个属性，没加的就是可见的  
2. gcc 在链接时设置 -fvisibility=hidden，则不加 visibility声明的都默认为hidden; gcc默认设置 -fvisibility=default，即全部可见；  
在gcc中加了这个设置之后表示所有的函数都是对外不可见了，然后在代码里面对于想公开的函数加上 __attribute__((visibility("default")))

3. 使用export map，gcc -Wl,--version-script=export.map, 在export.map中指定

   {

   global:export_func;

   local:*;

   };  
   则除了export_func外，全部为内部可见；  
   实验通过export map的方法,通过readelf总是发现函数仍然可见，后来搜索发现：
需同时使用ld --retain-symbols-file --version-script 两个选项方能达到 使得elf文件中的.dynsym和.symtab中的符号表都得到控制；其中--retain-symbols-file控制静态符号表，--version-script则对应dynamic符号表；
4. 使用libtool的export-symbols选项，没试过，略；

以上方法，对于gcc的visibility功能需要 gcc 4以上版本，

## 动态库中c++类导出
个人的一些原则：
- 尽量使用extern "C" + c函数的方式；  
- 不使用stl容器，只使用基本类型；
- 如果非要使用c++ 类和stl，库和使用者的编译器保持一致。

### 如果使用类可以用c函数的形式到处类。  
定义
```c++
#if defined(__cplusplus)
  // Code being compiled with C++ compiler 
  #define MATHTOOLS_EXTERN_C extern "C"
#else 
  // Code being compiled with C compiler (Not C++ compiler)
  #define MATHTOOLS_EXTERN_C 
#endif 

#if defined(_WIN32)
  // MS Windows DLLs (*.dll)
  #define MATHTOOLS_EXPORT_C MATHTOOLS_EXTERN_C __declspec(dllexport)
#else 
  // Unix-like Shared Object (.so) operating systems and GCC.
  #define MATHTOOLS_EXPORT_C MATHTOOLS_EXTERN_C __attribute__ ((visibility ("default")))
#endif 

```
c++类
```c++
class Plotter{
  struct impl; 
  std::unique_ptr<impl> m_pimpl;    
public: 
  Plotter();
  Plotter(double width, double height);
  Plotter(Plotter const& rhs) = default;
 ~Plotter() = default;

 void addPoint(double x, double y);
 void clear();
};
```

使用c的方式实现
```c
// C-interface for class default constructor.
MATHTOOLS_EXPORT_C
Plotter* mathtools_Plotter_new() 
{
   return new (std::nothrow) Plotter();
}

// C-interface for class copy constructor.
MATHTOOLS_EXPORT_C
Plotter* mathtools_Plotter_new_copy(Plotter* hPlotter) 
{
   return new (std::nothrow) Plotter(*hPlotter);
}

// C-interface for class destructor.
MATHTOOLS_EXPORT_C
void mathtools_Plotter_delete(Plotter* hPlotter) 
{
  delete hPlotter;
}

// C-interface for class ordinary method.
// Schema: <Library Name>_<Class Name>_<method>
MATHTOOLS_EXPORT_C
void mathtools_Plotter_addPoint(Plotter* hPlotter, double x, double y)
{
   hPlotter->addPoint(x, y);
}

// Error handling for C-interface functions or C-APIs
MATHTOOLS_EXPORT_C
void mathtools_Plotter_addPoint(Plotter* hPlotter, double x, double y, int* ErrorCode)
{
    *ErrorCode = ErrorEnum::OK;
    try { 
       // Method addPoint throws exceptions 
       hPlotter->addPoint(x, y);
    } catch(Exception1 const& ex){
       *ErrorCode = ErrorEnum::NO_SPACE_ERROR; // Set with value of some enum 
    } catch(Exception2 const& ex){
       *ErrorCode = ErrorEnum::SOME_ERROR_ENUM;
   }
}
```

### Passing std::string STL Object across a DLL boundary
```c++
Type synonym:

// h prefix stands for handle 
using hString = std::string*;
C-interface for std::string default constructor.

LIBRARY_EXPORT_C  
hString libprefix_string_new()
{
   // std::nothrow => Do not throw std::bad_alloc, just return 
   // a null pointer on failure.
   return new (std::nothrow) std::string();   
}

// C-interface for std::string copy-constructor.
LIBRARY_EXPORT_C  
hString libprefix_string_new_copy(hString hstr)
{
   // std::nothrow => Do not throw std::bad_alloc, just return 
   // a null pointer on failure.
   return new (std::nothrow) std::string(*hstr);   
}

// C-interface for std::string overloaded constructor.
LIBRARY_EXPORT_C  
hString libprefix_string_new2(const char* text)
{
   return new (std::nothrow) std::string(text);   
}

// C-interface for std::string destructor.
LIBRARY_EXPORT_C 
hString libprefix_string_delete(hString hstr)
{
   delete hstr;
}
```

### Passing std::vector<double> across a DLL boundary
```c++
// Handle type:
using hVectorD = std::vector<double>*;

// Constructors:
LIBRARY_EXPORT_C 
hVectorD libprefix_vectorD_new()
{
  return new (std::nothrow) std::vector<double>();
}

LIBRARY_EXPORT_C 
hVectorD libprefix_vectorD_new2(size_t n, double x)
{
  return new (std::nothrow) std::vector<double>(n, x);
}

LIBRARY_EXPORT_C 
hVectorD libprefix_vectorD_new3(size_t n, double [] array)
{
  // Iterator-range constructor 
  return new (std::nothrow) std::vector<double>(array, array + n);
}

// Destructor:
LIBRARY_EXPORT_C 
void libprefix_vectorD_delete(hVectorD hvec)
{
  delete hvec;
}

// Get size:
LIBRARY_EXPORT_C 
size_t libprefix_vectorD_size(hVectorD hvec)
{
  hvec->size();
}

// Getter:
LIBRARY_EXPORT_C 
double libprefix_vectorD_get(hVectorD hvec, size_t n)
{
  return hvec->operator[](n);
}

// Setter:
LIBRARY_EXPORT_C 
void libprefix_vectorD_set(hVectorD hvec, size_t n, double x)
{
  hvec->operator[](n) = x;
}
```

## 获取冬天库路径
```c++
#include <dlfcn.h>
//定义该函数为了dladdr获取符号信息
void fun1()
{
   Dl_info info;
   //dladdr获取某个地址的符号信息
   int rc = dladdr((void*)fun1, &info);  //(void *)转化是关键步骤
   char dir[100];
   strcpy(dir, info.dli_fname); //info.dli_fname存的是包含文件名的路径.
}
```
dladdr的原型:

int dladdr(void *address, Dl_info *dlip); //这个dladdr这个函数只对动态链接程序起作用。 

dlip 是指向Dl_info 结构的指针。该结构必须由用户分配。如果指定的address 在其中一个加载模块的范围内，则
结构成员由dladdr() 设置。Dl_info 结构包含下列成员：
```c++
struct {
const char *dli_fname;　　//这个路径是包含这个共享对象地址的文件路径，以我的理解就是调用这个函数的文件
void *dli_fbase;
const char *dli_sname;
void *dli_saddr;
size_t dli_size; /* ELF only */
int dli_bind; /* ELF only */
int dli_type;
};
```
参考链接:
1.动态链接库加载拾遗&dladdr函数使用:https://www.jianshu.com/p/1ef4460b63db
2.https://blog.csdn.net/dragon101788/article/details/18673323



- [1] [Shared Libraries - Binary Components](https://caiorss.github.io/C-Cpp-Notes/DLL-Binary-Components-SharedLibraries.html)