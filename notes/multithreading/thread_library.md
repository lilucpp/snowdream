# Thread library

## Future

标准库提供了一些工具来获取异步任务（即在单独的线程中启动的函数）的返回值，并捕捉其所抛出的异常。

定义于头文件  <future>

|                                                              |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [promise](https://zh.cppreference.com/w/cpp/thread/promise)(C++11) | 存储一个值以进行异步获取 (类模板)                            |
| [packaged_task](https://zh.cppreference.com/w/cpp/thread/packaged_task)(C++11) | 打包一个函数，存储其返回值以进行异步获取 (类模板)            |
| [future](https://zh.cppreference.com/w/cpp/thread/future)(C++11) | 等待被异步设置的值 (类模板)                                  |
| [shared_future](https://zh.cppreference.com/w/cpp/thread/shared_future)(C++11) | 等待被异步设置的值（可能为其他 future 所引用） (类模板)      |
| [async](https://zh.cppreference.com/w/cpp/thread/async)(C++11) | 异步运行一个函数（有可能在新线程中执行），并返回保有其结果的 [std::future](https://zh.cppreference.com/w/cpp/thread/future) (函数模板) |

具体用法参考

1. https://zh.cppreference.com/w/cpp/thread

## ThreadPool

使用boost： [threadpool](../code_snippets/thread/threadpool_boost)

使用cpp11: [threadpool](../code_snippets/thread/threadpool_cpp11) 

其中使用cpp11的threadpool是[progschj的复本](https://github.com/progschj/ThreadPool)

## BlockingQueue

[blockingqueue](../code_snippets/thread/BlockingQueue.h)

此实现参考陈硕的muduo库。

## Counter

[计数器](../code_snippets/thread/Counter)

此实现参考陈硕的muduo库

## taskflow

github: https://github.com/taskflow/taskflow

Taskflow helps you quickly write parallel tasks programs in modern C++

## Async++

github: https://github.com/Amanieu/asyncplusplus

Async++ is a lightweight concurrency framework for C++11. The concept was inspired by the [Microsoft PPL library](http://msdn.microsoft.com/en-us/library/dd492418.aspx) and the [N3428](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3428.pdf) C++ standard proposal.

## thread: pause, interrupt...

The class `working_thread` is cross-platform (Windows/Linux) implementation of the pause/resume .

地址：https://www.codeproject.com/Articles/570769/Data-Processing-Thread-with-the-Pause-Resume-Funct

bak addr: https://1drv.ms/u/s!AiOHW4QDJaFOhBeLYReQ5fDjvexj?e=fVNvCL


## Threadsafe vs re-entrant
[wikipedia:](https://en.wikipedia.org/wiki/Reentrancy_(computing))
若一个程序或子程序可以“在任意时刻被中断然后操作系统调度执行另外一段代码，这段代码又调用了该子程序不会出错”，则称其为可重入（reentrant或re-entrant）的。即当该子程序正在运行时，执行线程可以再次进入并执行它，仍然获得符合设计时预期的结果。与多线程并发执行的线程安全不同，可重入强调对单个线程执行时重新进入同一个子程序仍然是安全的。
若一个函数是可重入的，则该函数应当满足下述条件：

不能含有静态（全局）非常量数据。
不能返回静态（全局）非常量数据的地址。
只能处理由调用者提供的数据。
不能依赖于单实例模式资源的锁。
调用(call)的函数也必需是可重入的。

可重入函数未必是线程安全的；线程安全函数未必是可重入的。
例如，一个函数打开某个文件并读入数据。这个函数是可重入的，因为它的多个实例同时执行不会造成冲突；但它不是线程安全的，因为在它读入文件时可能有别的线程正在修改该文件，为了线程安全必须对文件加“同步锁”。
另一个例子，函数在它的函数体内部访问共享资源使用了加锁、解锁操作，所以它是线程安全的，但是却不可重入。因为若该函数一个实例运行到已经执行加锁但未执行解锁时被停下来，系统又启动该函数的另外一个实例，则新的实例在加锁处将转入等待。如果该函数是一个中断处理服务，在中断处理时又发生新的中断将导致资源死锁。fprintf函数就是线程安全但不可重入。
下述例子，是线程安全的，但不是可重入的。
```c++
int function()
{
 mutex_lock();
 ...
 function body
 ...
 mutex_unlock();
}
```

example: 

To illustrate reentrancy, this article uses as an example a [C](https://en.wikipedia.org/wiki/C_(programming_language)) utility function, `swap()`, that takes two pointers and transposes their values, and an interrupt-handling routine that also calls the swap function.

**Neither reentrant nor thread-safe**[[edit](https://en.wikipedia.org/w/index.php?title=Reentrancy_(computing)&action=edit&section=4)]

This is an example swap function that fails to be reentrant or thread-safe. Since the `tmp` variable is globally shared, without serialization, among any concurrent instances of the function, one instance may interfere with the data relied upon by another. As such, it should not have been used in the interrupt service routine `isr()`:

```c++
int tmp;

void swap(int* x, int* y)
{
    tmp = *x;
    *x = *y;
    /* Hardware interrupt might invoke isr() here. */
    *y = tmp;    
}

void isr()
{
    int x = 1, y = 2;
    swap(&x, &y);
}
```

**Thread-safe but not reentrant**[[edit](https://en.wikipedia.org/w/index.php?title=Reentrancy_(computing)&action=edit&section=5)]

The function `swap()` in the preceding example can be made thread-safe by making `tmp` [thread-local](https://en.wikipedia.org/wiki/Thread-local_storage). It still fails to be reentrant, and this will continue to cause problems if `isr()` is called in the same context as a thread already executing `swap()`:

```c++
_Thread_local int tmp;

void swap(int* x, int* y)
{
    tmp = *x;
    *x = *y;
    /* Hardware interrupt might invoke isr() here. */
    *y = tmp;    
}

void isr()
{
    int x = 1, y = 2;
    swap(&x, &y);
}
```

**Reentrant but not thread-safe**[[edit](https://en.wikipedia.org/w/index.php?title=Reentrancy_(computing)&action=edit&section=6)]

The following (somewhat contrived) modification of the swap function, which is careful to leave the global data in a consistent state at the time it exits, is reentrant; however, it is not thread-safe, since there are no locks employed, it can be interrupted at any time:

```c++
int tmp;

void swap(int* x, int* y)
{
    /* Save global variable. */
    int s;
    s = tmp;

    tmp = *x;
    *x = *y;
    *y = tmp;     /* Hardware interrupt might invoke isr() here. */

    /* Restore global variable. */
    tmp = s;
}

void isr()
{
    int x = 1, y = 2;
    swap(&x, &y);
}
```

**Reentrant and thread-safe**[[edit](https://en.wikipedia.org/w/index.php?title=Reentrancy_(computing)&action=edit&section=7)]

An implementation of `swap()` that allocates `tmp` on the [stack](https://en.wikipedia.org/wiki/Call_stack) instead of globally and that is called only with unshared variables as parameters[[a\]](https://en.wikipedia.org/wiki/Reentrancy_(computing)#cite_note-7) is both thread-safe and reentrant. Thread-safe because the stack is local to a thread and a function acting just on local data will always produce the expected result. There is no access to shared data therefore no data race.

```c++
void swap(int* x, int* y)
{
    int tmp;
    tmp = *x;
    *x = *y;
    *y = tmp;    /* Hardware interrupt might invoke isr() here. */
}

void isr()
{
    int x = 1, y = 2;
    swap(&x, &y);
}
```



**异步编程过程中自己使用的功能：**

1. future 异步获取返回结果。
2. threadpool使用的较多。
3. blockingqueue 生产者消费者
4. counter 计数器，多线程下统计用。
5. taskflow ayncc++ 是两个不错的异步线程库。
