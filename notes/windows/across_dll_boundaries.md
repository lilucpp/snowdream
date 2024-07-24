# Potential Errors Passing CRT Objects Across DLL Boundaries

[reference](https://docs.microsoft.com/en-us/cpp/c-runtime-library/potential-errors-passing-crt-objects-across-dll-boundaries?redirectedfrom=MSDN&view=vs-2019)

When you pass C Run-time (CRT) objects such as file handles, locales, and environment variables into or out of a DLL (function calls across the DLL boundary), unexpected behavior can occur if the DLL, as well as the files calling into the DLL, use different copies of the CRT libraries.

A related problem can occur when you allocate memory (either explicitly with `new` or `malloc`, or implicitly with `strdup`, `strstreambuf::str`, and so on) and then pass a pointer across a DLL boundary to be freed. This can cause a memory access violation or heap corruption if the DLL and its users use different copies of the CRT libraries.

示例代码：snowdream/code_snippets/Across_DLL_Boundaries



## 摘录资料

A.exe中加载B.dll.  在A.exe中用new申请了一片内存，在B.dll中执行delete导致程序崩溃。

> 原因：
>
> 跨模块内存管理不一致导致。A.exe是MD链接，B.dll是MT链接，有2个C++运行库运行在同一个地址空间上，有可能读写同样内存区域，导致程序崩溃。
>
> 解决办法：
>
> ​        每个模块自己管理内存。不要跨模块申请/释放内存.
>
> 1. A.exe中调用B.dll中的对应的函数，由其分配内存，然后再调用B.dll中的函数去释放。
>
> 2. A申请的内存，由A来释放。
>3. B.dll使用MD链接，而且B编译时使用和A编译时相同版本的运行时库。

> 详解：
>
> 在Windows系统中，进程和dll的内存管理是由Runtime Library实现的，而MT和MD影响了Runtim Library的链接方式，从而导致了此问题。
>
> l MT 是多线程静态链接运行时库。
>
> l MD是多线程动态链接运行时库。
>
> 在《》中专门讲述了该问题。我这里简单摘录一下：
>
> If you choose to link with the **static runtime library**, then your module has its **own private copy of the C/C++ runtime.** When your module calls `new` or `malloc`, the memory can only be freed by your module calling `delete` or `free`. **If another module calls** `delete` **or** `free`**, that will use the C/C++ runtime of that other module which is not the same as yours**. Indeed, even if you choose to link with the DLL version of the C/C++ runtime library, you still have to agree which version of the C/C++ runtime to use. If your DLL uses `MSVCRT20.DLL` to allocate memory, then anybody who wants to free that memory must also use `MSVCRT20.DLL`.
>
> 另外在MSDN《》也提到：
>
> **If you do choose to mix CRT libraries, remember that you have two separate copies of the CRT**, with separate and distinct states, so you must be careful about what you try to do across a CRT-boundary. There are many ways to get into trouble with two CRTs. Here are just a few:
>
> - **There are two separate heaps**. You cannot allocate (explicitly with new, malloc, or so on -- or implicitly with strdup, strstreambuf::str, or so on), and then pass the pointer across a CRT-boundary to be freed.
> - You cannot pass a FILE* or file handle across a CRT-boundary and expect the "stdio low-level IO" to work.
> - You cannot set the locale in one and expect the other's locale to be set.
>
> 简单点说，当使用静态库链接时，会有多份运行时库，而且每份库都拷贝一份自己的内存管理。而使用动态链接后，由于都是链接的同一个运行时库，这样就保证内存管理只用一份了。但是动态链接时必须两个模块使用同样版本的运行时库，不同版本的依旧会有多份。
>
> 在该案例中，由于A.exe是MD动态链接，B.dll是MT静态链接，导致有2份运行时库存在，在进行内存管理时就出错了，从而导致程序崩溃。
>
> 所以跨模块的内存管理，**最好是由每个模块提供自己的分配和销毁接口函数，然后在模块外部通过这些接口的调用来控制对象的生命期**，而不是在外部 new/delete。如果实在要用，可以使用微软提供了GlobalAlloc/GlobalFree这样的全局内存API，用它们的话跨模块也没有问题的。

> 模块连接C/C++运行库的静态版本和动态版本不同的疑问
>
> windows核心编程中提到，模块中存在如下代码C/C++ codeVoid EXEFunc(){PVOID pv = DLLFunc();free(pv);}PVOID DLLFunc(){return (malloc(100));} 
>在连接到C/C++运行库的静态版本时，free调用就会失败。为什么调用 malloc就不会失败呢？



跨模块内存管理的陷阱

许式伟
2004年6月21日

由于编译器、编译模式的不同，不同模块的内存结构与管理程序往往并不相同。因此，如果我们在一个模块申请内存，而在另一个模块中释放，这是一个不安全的做法。因为模块在释放内存的时候，并不会预料到需要释放的内存并非是自己管理的。

 

直观的说，假设我们有两个模块：Module1，Module2。它们有函数Module1.alloc，Module1.free；Module2.alloc, Module2.free。虽然同为alloc和free，但是你不能假设Module1.alloc/free和Module2.alloc/free是同一份代码。因此，Module1.alloc申请的资源，只有由Module1.free去释放才是确保安全的。

  

展开来讲，我们通常有以下跨模块调用约定： 

1)    不能够在一个模块的引出函数、接口中申请内存并且返回出去，让另一模块释放它。

如果有此需求，尝试用以下几种解决方案:

1. 考虑提供一个函数，让外部模块获得所需的内存大小，让外部模块申请内存并传入。这是一个比较典型的解决方案，Windows的API均采用此解决方案。
   
2. 考虑使用一个接口包装此内存的访问，让外部模块获得接口指针，并以此访问内存。内存是通过接口的Release()函数释放的。这样就保证了内存释放的正确性。
   
3. 考虑使用CoTaskMemAlloc/CoTaskMemFree申请、释放内存。因为内存的申请、释放由系统完成，故可以保证其一致性。
   
4. 作为条目3. 的特殊情形，如果返回的是字符串，可考虑用BSTR。此时资源管理由系统调用SysAllocString/SysFreeString 实现。
   
5. 仍然在内部申请内存并返回出去，同时将该内存的释放函数也作为引出函数引出去。外部模块使用完该内存后，用我们引出的释放函数释放它。这是可行的方案，虽然比较少见。你可以认为其实CoTaskMemAlloc/CoTaskMemFree、SysAllocString/SysFreeString也是基于这条规则提供的，只不过它没有特定目的而已。

◆注意◆

有时候出于某种考虑（例如检测内存资源泄漏），我们可能提供一个自己实现的Win32 API版本来取代Windows的系统调用。

我们知道，如果你使用CoTaskMemAlloc/CoTaskMemFree、SysAllocString/SysFreeString来申请、释放内存，那么哪怕存在内存泄漏，我们在《[最快速度找到内存泄漏](http://blog.csdn.net/xushiweizh/archive/2006/12/20/1451083.aspx)》中介绍的方法并不能检测出来。

除了使用一些系统资源泄漏的检测工具（其实它们的方法和我们这里介绍的肯定也类似）外，一种方法，就是提供这些API的替换版本。这些替换版本中，我们提供了泄漏检测的能力。

我们这里并不准备详细讨论这个技巧。但是请注意，这里存在的潜在危险是，有可能出现这样的情形：设想我们的某DLL使用了替换版本的SysAllocString，其中申请了一个BSTR返回给另一DLL，而该DLL并不使用替换版本的SysFreeString，而是调用系统的SysFreeString释放这个BSTR。这里存在的问题是显然的，因为系统并没有分配过这样一个BSTR。

2)    不能够在参数列表或返回值中用到类。

这是因为:

1. 同一个类，相同的声明，在**不同的编译器**、甚至**不同的编译模式**下，会有不同的内存布局。也就是说，看似是同一个类，但是其实在不同的模块中，理解上根本不同。例如，你用VC++写一个DLL，该DLL返回一个std::string，而DLL的客户程序是C++ Builder写的。你能够保证C++ Builder的std::string与VC++的内存布局一致吗？
   
2. 类存在成员函数（特别是构造、析构），这些成员函数对我们来说是个黑箱操作。对他们的调用同样容易产生这样的情况，就是在我们的模块中申请了内存，而在外部模块中（由析构函数）释放。仍然以std::string为例。我们往往为了方便返回一个字符串，而将函数声明为:
       ⑴ std::string getXXX();
   或者:
       ⑵ getXXX(std::string& str);
   这种方式在同一模块中是可行的，而且是相对比较高效的方式。但是如果用于跨模块的字符串传递，则存在风险（并不一定会出问题，关于什么时候不出问题，我们下一回讨论）。 

遇到这种要用类的情形。请尝试采用以下方案:

1.  考虑采用**纯结构体**。
2. 考虑使用一个接口包装该类，将该类实现为COM组件。
3. 如果返回的是字符串，考虑用BSTR。 
   

纯结构体

所谓“纯结构体”，是指该结构体:

1. 没有任何虚拟的成分。如虚函数、虚拟继承等。
   
2. 它的所有成员变量，均为简单数据类型（C标准数据类型，不包括指针），或者是另一个“纯结构体”。
   
3. 如果成员变量是一个指针，那么要么作为输入参数，指向的内容是一个纯结构体或C标准字符串；要么作为输出参数，指向的内容是一系统分配的资源。

总的说来一句话，就是“纯结构体”成员的类型要求，完全等同模块的引出函数参数类型的要求。

 

纯结构体在接口定义中比较广泛，往往用于取代在接口使用类的需求。对于我们规范中的“不允许使用类”，有一个误区是，使用了一个struct关键字定义的，本质上还是类的东西。例如：

struct AStruct
{
   std::string strA;
   std::string strB;
};

这个struct有构造、析构（尽管没有显式写出，编译器帮你生成的），析构中有内存释放操作，是一个标标准准的“类”。

 

另外，结构体需要显式指定字节对齐方式。例如：

\#pragma pack(push, 1)
struct XXXX
{
    ...
};
\#pragam pack(pop)

附加说明：

对“内存管理”相关的技术感兴趣？这里可以看到我的[所有关于内存管理的文章](http://blog.csdn.net/xushiweizh/category/265099.aspx)。