## [Interprocess Communications](https://docs.microsoft.com/zh-cn/windows/win32/ipc/interprocess-communications?redirectedfrom=MSDN#using-a-file-mapping-for-ipc)

## 通信选择问题：

1. 尽量避免进程间通信
2. 尽量避免数据竟争的情况，简化设计。使用共享内存。
3. 使用命名管道。
4. 使用进程间同步机制+共享内容。
5. 更复杂的设计，考虑使用库，如ZeroMQ。
6. 也可使用chrome ipc做进程间通讯，项目主服务上使用过。

简单的进程间同步可以使用共享内存+Interlocked*函数（例如InterlockedIncrement).



个人认为理想的进程之间通讯应如线程间同步一样。

如[blocking queue](https://github.com/lilucpp/base/blob/master/src/BlockingQueue.h)：

1.  插入和取出数据有锁。
2. 取数据阻塞（使用条件变量）
3. 模板：支持常见数据类型。

附录：

1. 类似的[shared_mpmc_queue.h](https://raw.githubusercontent.com/erez-strauss/lockfree_mpmc_queue/master/shared_mpmc_queue.h)
2. [WFMPMC](https://github.com/MengRao/WFMPMC)：实现了linux上进程间queue。





进程间的现状：可以使用boost库中的interprocess模块。

1. 实现简单的进程间数据传递（如自定义分配器的vector、map等），单有一缺点，打开共享内容的进程无法修改数据。
2. 有同意义进程间的mutex、condition，算是不错。[example](https://www.boost.org/doc/libs/1_68_0/doc/html/interprocess/synchronization_mechanisms.html#interprocess.synchronization_mechanisms.conditions)。
3. 有消息队列，算是不错的一个功能。[message queue](https://www.boost.org/doc/libs/1_68_0/doc/html/interprocess/synchronization_mechanisms.html#interprocess.synchronization_mechanisms.message_queue)。



[msdn](https://docs.microsoft.com/zh-cn/windows/win32/sync/interlocked-variable-access?redirectedfrom=MSDN)

The interlocked functions provide a simple mechanism for synchronizing access to a variable that is shared by multiple threads. They also perform operations on variables in an atomic manner. The threads of different processes can use these functions if the variable is in shared memory.

[linux上也有对应得函数](https://stackoverflow.com/a/41421376/8330816)

GCC has [atomic built-ins](http://gcc.gnu.org/onlinedocs/gcc-4.4.3/gcc/Atomic-Builtins.html#Atomic-Builtins), `__sync_val_compare_and_swap` should be equivalent of `InterLockedCompareExchange`
If you use C++ 11, you can use `std::atomic_compare_exchange_*` in [atomic operations library](http://en.cppreference.com/w/cpp/atomic/atomic_compare_exchange) as needed.



## linux IPC

和windows类似，参考下图

![communication](../img/communication.png)



## windows IPC

The following IPC mechanisms are supported by Windows:

- [Data Copy](#data-copy)

- [File Mapping](#file-mapping)

- [Pipes](#pipes)

- [Interprocess Synchronization](#interprocess-synchronization)

  - [Event](#event)
  - [Mutex](#mutex)
  - [Semaphore](#semaphore)
  - [Waitable Timer](#waitable-timer)

  

## Data Copy

Data copy enables an application to send information to another application using the **WM_COPYDATA** message. This method requires cooperation between the sending application and the receiving application. The receiving application must know the format of the information and be able to identify the sender. 

**Key Point:** Data copy can be used to quickly send information to another application using Windows messaging. 

[example:](https://docs.microsoft.com/zh-cn/windows/win32/dataxchg/using-data-copy)

```c++
typedef struct tagMYREC
{
   char  s1[80];
   char  s2[80];
   DWORD n;
} MYREC;
COPYDATASTRUCT MyCDS;
MYREC MyRec;
// Fill the COPYDATA structure
// 
   MyCDS.dwData = MYPRINT;          // function identifier
   MyCDS.cbData = sizeof( MyRec );  // size of data
   MyCDS.lpData = &MyRec;           // data structure
//
// Call function, passing data in &MyCDS
//
   hwDispatch = FindWindow( "Disp32Class", "Hidden Window" );
   SendMessage( hwDispatch,
                WM_COPYDATA,
                (WPARAM)(HWND) hWnd,
                (LPARAM) (LPVOID) &MyCDS );
```

The receiving application has a hidden window which receives the information from **WM_COPYDATA** and displays it to the user.

```c++
//
case WM_COPYDATA:
   pMyCDS = (PCOPYDATASTRUCT) lParam;
   switch( pMyCDS->dwData )
   {
      case MYDISPLAY:
         MyDisplay( (LPSTR) ((MYREC *)(pMyCDS->lpData))->s1,
                    (LPSTR) ((MYREC *)(pMyCDS->lpData))->s2,
                    (DWORD) ((MYREC *)(pMyCDS->lpData))->n );
   }
```

## File Mapping

共享内存。

*File mapping* enables a process to treat the contents of a file as if they were a block of memory in the process's address space. The processes must use a synchronization object, such as a semaphore, to prevent data corruption in a multitasking environment.

You can use a special case of file mapping to provide *named shared memory* between processes. 

**Key Point:** File mapping is an efficient way for two or more processes on the same computer to share data, but you must provide synchronization between the processes. For more information.

The following illustration shows the relationship between the file on disk, a file mapping object, and a file view.

<img src="../img/fmap.png" alt="fmap" style="zoom:150%;" />  

- `CreateFileMapping()`
- `MapViewOfFile()`
- `UnMapViewOfFile()`
- `CloseHandle()`

[Creating Named Shared Memory](https://docs.microsoft.com/en-us/windows/win32/memory/creating-named-shared-memory)

```c++
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define BUF_SIZE 256
TCHAR szName[]=TEXT("Global\\MyFileMappingObject");
TCHAR szMsg[]=TEXT("Message from first process.");

int _tmain()
{
   HANDLE hMapFile;
   LPCTSTR pBuf;

   hMapFile = CreateFileMapping(
                 INVALID_HANDLE_VALUE,    // use paging file
                 NULL,                    // default security
                 PAGE_READWRITE,          // read/write access
                 0,                       // maximum object size (high-order DWORD)
                 BUF_SIZE,                // maximum object size (low-order DWORD)
                 szName);                 // name of mapping object

   if (hMapFile == NULL)
   {
      _tprintf(TEXT("Could not create file mapping object (%d).\n"),
             GetLastError());
      return 1;
   }
   pBuf = (LPTSTR) MapViewOfFile(hMapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
                        0,
                        0,
                        BUF_SIZE);

   if (pBuf == NULL)
   {
      _tprintf(TEXT("Could not map view of file (%d).\n"),
             GetLastError());

       CloseHandle(hMapFile);

      return 1;
   }


   CopyMemory((PVOID)pBuf, szMsg, (_tcslen(szMsg) * sizeof(TCHAR)));
    _getch();

   UnmapViewOfFile(pBuf);

   CloseHandle(hMapFile);

   return 0;
}
```



```c++
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#pragma comment(lib, "user32.lib")

#define BUF_SIZE 256
TCHAR szName[]=TEXT("Global\\MyFileMappingObject");

int _tmain()
{
   HANDLE hMapFile;
   LPCTSTR pBuf;

   hMapFile = OpenFileMapping(
                   FILE_MAP_ALL_ACCESS,   // read/write access
                   FALSE,                 // do not inherit the name
                   szName);               // name of mapping object

   if (hMapFile == NULL)
   {
      _tprintf(TEXT("Could not open file mapping object (%d).\n"),
             GetLastError());
      return 1;
   }

   pBuf = (LPTSTR) MapViewOfFile(hMapFile, // handle to map object
               FILE_MAP_ALL_ACCESS,  // read/write permission
               0,
               0,
               BUF_SIZE);

   if (pBuf == NULL)
   {
      _tprintf(TEXT("Could not map view of file (%d).\n"),
             GetLastError());

      CloseHandle(hMapFile);

      return 1;
   }

   MessageBox(NULL, pBuf, TEXT("Process2"), MB_OK);

   UnmapViewOfFile(pBuf);

   CloseHandle(hMapFile);

   return 0;
}
```

项目中的应用：解析文件

先创建一个共享内存 ---> 然后调用解析程序 ---> 解析程序把内容写道共享内存 ---> 解析返回 ---> 释放共享内存

没有涉及同步，何时销毁问题。 参考：[Interprocess Synchronization](#interprocess-synchronization)

## Pipes

Anonymous pipes provide an efficient way to redirect standard input or output to child processes on the same computer. Named pipes provide a simple programming interface for transferring data between two processes, whether they reside on the same computer or over a network.



## Interprocess Synchronization

Multiple processes can have handles to the same event, mutex, semaphore, or timer object, so these objects can be used to accomplish interprocess synchronization. The process that creates an object can use the handle returned by the creation function ([**CreateEvent**](https://msdn.microsoft.com/en-us/library/ms682396(v=VS.85).aspx), [**CreateMutex**](https://msdn.microsoft.com/en-us/library/ms682411(v=VS.85).aspx), [**CreateSemaphore**](https://docs.microsoft.com/en-us/windows/desktop/api/WinBase/nf-winbase-createsemaphorea), or [**CreateWaitableTimer**](https://docs.microsoft.com/en-us/windows/desktop/api/WinBase/nf-winbase-createwaitabletimera)). Other processes can open a handle to the object by using its name, or through inheritance or duplication.

引用：

[1] [**using-synchronization**](https://docs.microsoft.com/en-us/windows/win32/sync/using-synchronization)

[2] [interprocess-synchronization](https://docs.microsoft.com/en-us/windows/win32/sync/interprocess-synchronization)

### Event

Applications can use [event objects](https://docs.microsoft.com/zh-cn/windows/win32/sync/event-objects) in a number of situations to notify a waiting thread of the occurrence of an event. 

```c++
#include <windows.h>
#include <stdio.h>

#define THREADCOUNT 4 

HANDLE ghWriteEvent; 
HANDLE ghThreads[THREADCOUNT];

DWORD WINAPI ThreadProc(LPVOID);

void CreateEventsAndThreads(void) 
{
    int i; 
    DWORD dwThreadID; 

    // Create a manual-reset event object. The write thread sets this
    // object to the signaled state when it finishes writing to a 
    // shared buffer. 

    ghWriteEvent = CreateEvent( 
        NULL,               // default security attributes
        TRUE,               // manual-reset event
        FALSE,              // initial state is nonsignaled
        TEXT("WriteEvent")  // object name
        ); 

    if (ghWriteEvent == NULL) 
    { 
        printf("CreateEvent failed (%d)\n", GetLastError());
        return;
    }

    // Create multiple threads to read from the buffer.

    for(i = 0; i < THREADCOUNT; i++) 
    {
        // TODO: More complex scenarios may require use of a parameter
        //   to the thread procedure, such as an event per thread to  
        //   be used for synchronization.
        ghThreads[i] = CreateThread(
            NULL,              // default security
            0,                 // default stack size
            ThreadProc,        // name of the thread function
            NULL,              // no thread parameters
            0,                 // default startup flags
            &dwThreadID); 

        if (ghThreads[i] == NULL) 
        {
            printf("CreateThread failed (%d)\n", GetLastError());
            return;
        }
    }
}

void WriteToBuffer(VOID) 
{
    // TODO: Write to the shared buffer.
    
    printf("Main thread writing to the shared buffer...\n");

    // Set ghWriteEvent to signaled

    if (! SetEvent(ghWriteEvent) ) 
    {
        printf("SetEvent failed (%d)\n", GetLastError());
        return;
    }
}

void CloseEvents()
{
    // Close all event handles (currently, only one global handle).
    
    CloseHandle(ghWriteEvent);
}

int main( void )
{
    DWORD dwWaitResult;

    // TODO: Create the shared buffer

    // Create events and THREADCOUNT threads to read from the buffer

    CreateEventsAndThreads();

    // At this point, the reader threads have started and are most
    // likely waiting for the global event to be signaled. However, 
    // it is safe to write to the buffer because the event is a 
    // manual-reset event.
    
    WriteToBuffer();

    printf("Main thread waiting for threads to exit...\n");

    // The handle for each thread is signaled when the thread is
    // terminated.
    dwWaitResult = WaitForMultipleObjects(
        THREADCOUNT,   // number of handles in array
        ghThreads,     // array of thread handles
        TRUE,          // wait until all are signaled
        INFINITE);

    switch (dwWaitResult) 
    {
        // All thread objects were signaled
        case WAIT_OBJECT_0: 
            printf("All threads ended, cleaning up for application exit...\n");
            break;

        // An error occurred
        default: 
            printf("WaitForMultipleObjects failed (%d)\n", GetLastError());
            return 1;
    } 
            
    // Close the events to clean up

    CloseEvents();

    return 0;
}

DWORD WINAPI ThreadProc(LPVOID lpParam) 
{
    // lpParam not used in this example.
    UNREFERENCED_PARAMETER(lpParam);

    DWORD dwWaitResult;

    printf("Thread %d waiting for write event...\n", GetCurrentThreadId());
    
    dwWaitResult = WaitForSingleObject( 
        ghWriteEvent, // event handle
        INFINITE);    // indefinite wait

    switch (dwWaitResult) 
    {
        // Event object was signaled
        case WAIT_OBJECT_0: 
            //
            // TODO: Read from the shared buffer
            //
            printf("Thread %d reading from buffer\n", 
                   GetCurrentThreadId());
            break; 

        // An error occurred
        default: 
            printf("Wait error (%d)\n", GetLastError()); 
            return 0; 
    }

    // Now that we are done reading the buffer, we could use another
    // event to signal that this thread is no longer reading. This
    // example simply uses the thread handle for synchronization (the
    // handle is signaled when the thread terminates.)

    printf("Thread %d exiting\n", GetCurrentThreadId());
    return 1;
}
```

### Mutex

You can use a [mutex object](https://docs.microsoft.com/zh-cn/windows/win32/sync/mutex-objects) to protect a shared resource from simultaneous access by multiple threads or processes. Each thread must wait for ownership of the mutex before it can execute the code that accesses the shared resource.

```c++
#include <windows.h>
#include <stdio.h>

#define THREADCOUNT 2

HANDLE ghMutex; 

DWORD WINAPI WriteToDatabase( LPVOID );

int main( void )
{
    HANDLE aThread[THREADCOUNT];
    DWORD ThreadID;
    int i;

    // Create a mutex with no initial owner

    ghMutex = CreateMutex( 
        NULL,              // default security attributes
        FALSE,             // initially not owned
        NULL);             // unnamed mutex

    if (ghMutex == NULL) 
    {
        printf("CreateMutex error: %d\n", GetLastError());
        return 1;
    }

    // Create worker threads

    for( i=0; i < THREADCOUNT; i++ )
    {
        aThread[i] = CreateThread( 
                     NULL,       // default security attributes
                     0,          // default stack size
                     (LPTHREAD_START_ROUTINE) WriteToDatabase, 
                     NULL,       // no thread function arguments
                     0,          // default creation flags
                     &ThreadID); // receive thread identifier

        if( aThread[i] == NULL )
        {
            printf("CreateThread error: %d\n", GetLastError());
            return 1;
        }
    }

    // Wait for all threads to terminate

    WaitForMultipleObjects(THREADCOUNT, aThread, TRUE, INFINITE);

    // Close thread and mutex handles

    for( i=0; i < THREADCOUNT; i++ )
        CloseHandle(aThread[i]);

    CloseHandle(ghMutex);

    return 0;
}

DWORD WINAPI WriteToDatabase( LPVOID lpParam )
{ 
    // lpParam not used in this example
    UNREFERENCED_PARAMETER(lpParam);

    DWORD dwCount=0, dwWaitResult; 

    // Request ownership of mutex.

    while( dwCount < 20 )
    { 
        dwWaitResult = WaitForSingleObject( 
            ghMutex,    // handle to mutex
            INFINITE);  // no time-out interval
 
        switch (dwWaitResult) 
        {
            // The thread got ownership of the mutex
            case WAIT_OBJECT_0: 
                __try { 
                    // TODO: Write to the database
                    printf("Thread %d writing to database...\n", 
                            GetCurrentThreadId());
                    dwCount++;
                } 

                __finally { 
                    // Release ownership of the mutex object
                    if (! ReleaseMutex(ghMutex)) 
                    { 
                        // Handle error.
                    } 
                } 
                break; 

            // The thread got ownership of an abandoned mutex
            // The database is in an indeterminate state
            case WAIT_ABANDONED: 
                return FALSE; 
        }
    }
    return TRUE; 
}
```

### Semaphore

[信号量 维基百科](https://en.wikipedia.org/wiki/Semaphore_(programming))

The following example uses a [semaphore object](https://docs.microsoft.com/zh-cn/windows/win32/sync/semaphore-objects) to limit the number of threads that can perform a particular task. 

```c++
#include <stdio.h>
#include <windows.h>

#define MAX_SEM_COUNT 10
#define THREADCOUNT 12

HANDLE ghSemaphore;

DWORD WINAPI ThreadProc(LPVOID);

int main(void) {
  HANDLE aThread[THREADCOUNT];
  DWORD ThreadID;
  int i;

  // Create a semaphore with initial and max counts of MAX_SEM_COUNT

  ghSemaphore = CreateSemaphore(NULL,           // default security attributes
                                MAX_SEM_COUNT,  // initial count
                                MAX_SEM_COUNT,  // maximum count
                                NULL);          // unnamed semaphore

  if (ghSemaphore == NULL) {
    printf("CreateSemaphore error: %d\n", GetLastError());
    return 1;
  }

  // Create worker threads

  for (i = 0; i < THREADCOUNT; i++) {
    aThread[i] = CreateThread(NULL,  // default security attributes
                              0,     // default stack size
                              (LPTHREAD_START_ROUTINE)ThreadProc,
                              NULL,        // no thread function arguments
                              0,           // default creation flags
                              &ThreadID);  // receive thread identifier

    if (aThread[i] == NULL) {
      printf("CreateThread error: %d\n", GetLastError());
      return 1;
    }
  }

  // Wait for all threads to terminate

  WaitForMultipleObjects(THREADCOUNT, aThread, TRUE, INFINITE);

  // Close thread and semaphore handles

  for (i = 0; i < THREADCOUNT; i++) CloseHandle(aThread[i]);

  CloseHandle(ghSemaphore);

  return 0;
}

DWORD WINAPI ThreadProc(LPVOID lpParam) {
  // lpParam not used in this example
  UNREFERENCED_PARAMETER(lpParam);

  DWORD dwWaitResult;
  BOOL bContinue = TRUE;

  while (bContinue) {
    // Try to enter the semaphore gate.

    dwWaitResult = WaitForSingleObject(ghSemaphore,  // handle to semaphore
                                       0L);  // zero-second time-out interval

    switch (dwWaitResult) {
      // The semaphore object was signaled.
      case WAIT_OBJECT_0:
        // TODO: Perform task
        printf("Thread %d: wait succeeded\n", GetCurrentThreadId());
        bContinue = FALSE;

        // Simulate thread spending time on task
        Sleep(1000);

        // Release the semaphore when task is finished

        if (!ReleaseSemaphore(ghSemaphore,  // handle to semaphore
                              1,            // increase count by one
                              NULL))        // not interested in previous count
        {
          printf("ReleaseSemaphore error: %d\n", GetLastError());
        }
        break;

      // The semaphore was nonsignaled, so a time-out occurred.
      case WAIT_TIMEOUT:
        printf("Thread %d: wait timed out\n", GetCurrentThreadId());
        Sleep(100);
        break;
    }
  }
  return TRUE;
}
```

### Waitable Timer

The following example creates a timer that will be signaled after a 10 second delay.

```c++
#include <windows.h>
#include <stdio.h>

int main()
{
    HANDLE hTimer = NULL;
    LARGE_INTEGER liDueTime;

    liDueTime.QuadPart = -100000000LL;

    // Create an unnamed waitable timer.
    hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
    if (NULL == hTimer)
    {
        printf("CreateWaitableTimer failed (%d)\n", GetLastError());
        return 1;
    }

    printf("Waiting for 10 seconds...\n");

    // Set a timer to wait for 10 seconds.
    if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0))
    {
        printf("SetWaitableTimer failed (%d)\n", GetLastError());
        return 2;
    }

    // Wait for the timer.

    if (WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0)
        printf("WaitForSingleObject failed (%d)\n", GetLastError());
    else printf("Timer was signaled.\n");

    return 0;
}
```

























