# [About PSAPI](https://docs.microsoft.com/zh-cn/windows/win32/psapi/about-psapi)

The process status API (PSAPI) provides sets of functions for retrieving the following information:

- [Process Information](https://docs.microsoft.com/zh-cn/windows/win32/psapi/process-information)
- [Module Information](https://docs.microsoft.com/zh-cn/windows/win32/psapi/module-information)
- [Device Driver Information](https://docs.microsoft.com/zh-cn/windows/win32/psapi/device-driver-information)
- [Process Memory Usage Information](https://docs.microsoft.com/zh-cn/windows/win32/psapi/process-memory-usage-information)
- [Working Set Information](https://docs.microsoft.com/zh-cn/windows/win32/psapi/working-set-information)
- [Memory-Mapped File Information](https://docs.microsoft.com/zh-cn/windows/win32/psapi/memory-mapped-file-information)

# [Snapshots of the System](https://docs.microsoft.com/zh-cn/windows/win32/toolhelp/snapshots-of-the-system)

A snapshot is a read-only copy of the current state of one or more of the following lists that reside in system memory: processes, threads, modules, and heaps.

要获取系统内存的快照，请使用[**CreateToolhelp32Snapshot**](https://docs.microsoft.com/en-us/windows/desktop/api/TlHelp32/nf-tlhelp32-createtoolhelp32snapshot)函数。调用此函数时，可以通过指定以下一个或多个值来控制快照的内容：

- **TH32CS_SNAPHEAPLIST**
- **TH32CS_SNAPMODULE**
- **TH32CS_SNAPPROCESS**
- **TH32CS_SNAPTHREAD**

使用快照完成处理后，请使用[**CloseHandle**](https://docs.microsoft.com/windows/desktop/api/handleapi/nf-handleapi-closehandle)函数销毁它。如果不销毁快照，则该过程将泄漏内存，直到快照退出为止，此时系统将回收内存。

This section contains examples demonstrating how to perform the following tasks:

- [Taking a snapshot and viewing processes](#taking-a-snapshot-and-viewing processes)
- [Traversing the thread list](#traversing-the-thread-list)
- [Traversing the module list](#traversing-the-module-list)
- [Traversing the heap list](#traversing-the-heap-list)

## Other

1. two project

   [Process viewer](https://www.codeproject.com/Articles/18704/Process-viewer)  [backup address](https://1drv.ms/u/s!AiOHW4QDJaFOggGvzmo_7n-VT0CA?e=NKq2cu)

   [process hacker](https://github.com/processhacker/processhacker/tree/v2.39)  [backup address](https://1drv.ms/u/s!AiOHW4QDJaFOggKXT8h2BFNnWdgD?e=w5q1hn)

2. GetMainThreadId

```c++
#include <windows.h>
#include <tlhelp32.h>

using namespace std;

// https://stackoverflow.com/questions/1969579/getting-a-handle-to-the-processs-main-thread
DWORD GetMainThreadId() {
	HANDLE hThreadSnapshot =
		CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnapshot == INVALID_HANDLE_VALUE) {
		// throw std::runtime_error("GetMainThreadId failed");
		return 0;
	}
	THREADENTRY32 tEntry;
	tEntry.dwSize = sizeof(THREADENTRY32);
	DWORD result = 0;
	DWORD currentPID = GetCurrentProcessId();
	for (BOOL success = Thread32First(hThreadSnapshot, &tEntry);
		!result && success && GetLastError() != ERROR_NO_MORE_FILES;
		success = Thread32Next(hThreadSnapshot, &tEntry)) {
			if (tEntry.th32OwnerProcessID == 17800) {
				result = tEntry.th32ThreadID;
			}
	}
	CloseHandle(hThreadSnapshot);
	return result;
}

```

2. [Get the command line of a process](https://wj32.org/wp/2009/01/24/howto-get-the-command-line-of-processes/)

[backup address](https://1drv.ms/b/s!AiOHW4QDJaFOggCPhpL7ma7eCDN1?e=UeFf3l)

```c++
#include <windows.h>
#include <stdio.h>
 
typedef NTSTATUS (NTAPI *_NtQueryInformationProcess)(
    HANDLE ProcessHandle,
    DWORD ProcessInformationClass,
    PVOID ProcessInformation,
    DWORD ProcessInformationLength,
    PDWORD ReturnLength
    );
 
typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;
 
typedef struct _PROCESS_BASIC_INFORMATION
{
    LONG ExitStatus;
    PVOID PebBaseAddress;
    ULONG_PTR AffinityMask;
    LONG BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR ParentProcessId;
} PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;
 
PVOID GetPebAddress(HANDLE ProcessHandle)
{
    _NtQueryInformationProcess NtQueryInformationProcess =
        (_NtQueryInformationProcess)GetProcAddress(
        GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
    PROCESS_BASIC_INFORMATION pbi;
 
    NtQueryInformationProcess(ProcessHandle, 0, &pbi, sizeof(pbi), NULL);
 
    return pbi.PebBaseAddress;
}
 
int wmain(int argc, WCHAR *argv[])
{
    int pid;
    HANDLE processHandle;
    PVOID pebAddress;
    PVOID rtlUserProcParamsAddress;
    UNICODE_STRING commandLine;
    WCHAR *commandLineContents;
 
    if (argc < 2)
    {
        printf("Usage: getprocesscommandline [pid]\n");
        return 1;
    }
 
    pid = _wtoi(argv[1]);
 
    if ((processHandle = OpenProcess(
        PROCESS_QUERY_INFORMATION | /* required for NtQueryInformationProcess */
        PROCESS_VM_READ, /* required for ReadProcessMemory */
        FALSE, pid)) == 0)
    {
        printf("Could not open process!\n");
        return GetLastError();
    }
 
    pebAddress = GetPebAddress(processHandle);
 
    /* get the address of ProcessParameters */
    if (!ReadProcessMemory(processHandle, (PCHAR)pebAddress + 0x10,
        &rtlUserProcParamsAddress, sizeof(PVOID), NULL))
    {
        printf("Could not read the address of ProcessParameters!\n");
        return GetLastError();
    }
 
    /* read the CommandLine UNICODE_STRING structure */
    if (!ReadProcessMemory(processHandle, (PCHAR)rtlUserProcParamsAddress + 0x40,
        &commandLine, sizeof(commandLine), NULL))
    {
        printf("Could not read CommandLine!\n");
        return GetLastError();
    }
 
    /* allocate memory to hold the command line */
    commandLineContents = (WCHAR *)malloc(commandLine.Length);
 
    /* read the command line */
    if (!ReadProcessMemory(processHandle, commandLine.Buffer,
        commandLineContents, commandLine.Length, NULL))
    {
        printf("Could not read the command line string!\n");
        return GetLastError();
    }
 
    /* print it */
    /* the length specifier is in characters, but commandLine.Length is in bytes */
    /* a WCHAR is 2 bytes */
    printf("%.*S\n", commandLine.Length / 2, commandLineContents);
    CloseHandle(processHandle);
    free(commandLineContents);
 
    return 0;
}
```

### Taking a snapshot and viewing processes

```c++
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

//  Forward declarations:
BOOL GetProcessList( );
BOOL ListProcessModules( DWORD dwPID );
BOOL ListProcessThreads( DWORD dwOwnerPID );
void printError( TCHAR* msg );

int main( void )
{
  GetProcessList( );
  return 0;
}

BOOL GetProcessList( )
{
  HANDLE hProcessSnap;
  HANDLE hProcess;
  PROCESSENTRY32 pe32;
  DWORD dwPriorityClass;

  // Take a snapshot of all processes in the system.
  hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
  if( hProcessSnap == INVALID_HANDLE_VALUE )
  {
    printError( TEXT("CreateToolhelp32Snapshot (of processes)") );
    return( FALSE );
  }

  // Set the size of the structure before using it.
  pe32.dwSize = sizeof( PROCESSENTRY32 );

  // Retrieve information about the first process,
  // and exit if unsuccessful
  if( !Process32First( hProcessSnap, &pe32 ) )
  {
    printError( TEXT("Process32First") ); // show cause of failure
    CloseHandle( hProcessSnap );          // clean the snapshot object
    return( FALSE );
  }

  // Now walk the snapshot of processes, and
  // display information about each process in turn
  do
  {
    _tprintf( TEXT("\n\n=====================================================" ));
    _tprintf( TEXT("\nPROCESS NAME:  %s"), pe32.szExeFile );
    _tprintf( TEXT("\n-------------------------------------------------------" ));

    // Retrieve the priority class.
    dwPriorityClass = 0;
    hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
    if( hProcess == NULL )
      printError( TEXT("OpenProcess") );
    else
    {
      dwPriorityClass = GetPriorityClass( hProcess );
      if( !dwPriorityClass )
        printError( TEXT("GetPriorityClass") );
      CloseHandle( hProcess );
    }

    _tprintf( TEXT("\n  Process ID        = 0x%08X"), pe32.th32ProcessID );
    _tprintf( TEXT("\n  Thread count      = %d"),   pe32.cntThreads );
    _tprintf( TEXT("\n  Parent process ID = 0x%08X"), pe32.th32ParentProcessID );
    _tprintf( TEXT("\n  Priority base     = %d"), pe32.pcPriClassBase );
    if( dwPriorityClass )
      _tprintf( TEXT("\n  Priority class    = %d"), dwPriorityClass );

    // List the modules and threads associated with this process
    ListProcessModules( pe32.th32ProcessID );
    ListProcessThreads( pe32.th32ProcessID );

  } while( Process32Next( hProcessSnap, &pe32 ) );

  CloseHandle( hProcessSnap );
  return( TRUE );
}


BOOL ListProcessModules( DWORD dwPID )
{
  HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
  MODULEENTRY32 me32;

  // Take a snapshot of all modules in the specified process.
  hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwPID );
  if( hModuleSnap == INVALID_HANDLE_VALUE )
  {
    printError( TEXT("CreateToolhelp32Snapshot (of modules)") );
    return( FALSE );
  }

  // Set the size of the structure before using it.
  me32.dwSize = sizeof( MODULEENTRY32 );

  // Retrieve information about the first module,
  // and exit if unsuccessful
  if( !Module32First( hModuleSnap, &me32 ) )
  {
    printError( TEXT("Module32First") );  // show cause of failure
    CloseHandle( hModuleSnap );           // clean the snapshot object
    return( FALSE );
  }

  // Now walk the module list of the process,
  // and display information about each module
  do
  {
    _tprintf( TEXT("\n\n     MODULE NAME:     %s"),   me32.szModule );
    _tprintf( TEXT("\n     Executable     = %s"),     me32.szExePath );
    _tprintf( TEXT("\n     Process ID     = 0x%08X"),         me32.th32ProcessID );
    _tprintf( TEXT("\n     Ref count (g)  = 0x%04X"),     me32.GlblcntUsage );
    _tprintf( TEXT("\n     Ref count (p)  = 0x%04X"),     me32.ProccntUsage );
    _tprintf( TEXT("\n     Base address   = 0x%08X"), (DWORD) me32.modBaseAddr );
    _tprintf( TEXT("\n     Base size      = %d"),             me32.modBaseSize );

  } while( Module32Next( hModuleSnap, &me32 ) );

  CloseHandle( hModuleSnap );
  return( TRUE );
}

BOOL ListProcessThreads( DWORD dwOwnerPID ) 
{ 
  HANDLE hThreadSnap = INVALID_HANDLE_VALUE; 
  THREADENTRY32 te32; 
 
  // Take a snapshot of all running threads  
  hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 ); 
  if( hThreadSnap == INVALID_HANDLE_VALUE ) 
    return( FALSE ); 
 
  // Fill in the size of the structure before using it. 
  te32.dwSize = sizeof(THREADENTRY32); 
 
  // Retrieve information about the first thread,
  // and exit if unsuccessful
  if( !Thread32First( hThreadSnap, &te32 ) ) 
  {
    printError( TEXT("Thread32First") ); // show cause of failure
    CloseHandle( hThreadSnap );          // clean the snapshot object
    return( FALSE );
  }

  // Now walk the thread list of the system,
  // and display information about each thread
  // associated with the specified process
  do 
  { 
    if( te32.th32OwnerProcessID == dwOwnerPID )
    {
      _tprintf( TEXT("\n\n     THREAD ID      = 0x%08X"), te32.th32ThreadID ); 
      _tprintf( TEXT("\n     Base priority  = %d"), te32.tpBasePri ); 
      _tprintf( TEXT("\n     Delta priority = %d"), te32.tpDeltaPri ); 
      _tprintf( TEXT("\n"));
    }
  } while( Thread32Next(hThreadSnap, &te32 ) ); 

  CloseHandle( hThreadSnap );
  return( TRUE );
}

void printError( TCHAR* msg )
{
  DWORD eNum;
  TCHAR sysMsg[256];
  TCHAR* p;

  eNum = GetLastError( );
  FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL, eNum,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         sysMsg, 256, NULL );

  // Trim the end of the line and terminate it with a null
  p = sysMsg;
  while( ( *p > 31 ) || ( *p == 9 ) )
    ++p;
  do { *p-- = 0; } while( ( p >= sysMsg ) &&
                          ( ( *p == '.' ) || ( *p < 33 ) ) );

  // Display the message
  _tprintf( TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg );
}
```

### Traversing the thread list

The following example function lists running threads for a specified process. First, the `ListProcessThreads` function takes a snapshot of the currently executing threads in the system using **CreateToolhelp32Snapshot**, and then it walks through the list recorded in the snapshot using the **Thread32First** and **Thread32Next** functions. The parameter for `ListProcessThreads` is the process identifier of the process whose threads are to be listed.

```c++
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

//  Forward declarations:
BOOL ListProcessThreads( DWORD dwOwnerPID );
void printError( TCHAR* msg );

int main( void )
{
  ListProcessThreads(GetCurrentProcessId() );
  return 0;
}

BOOL ListProcessThreads( DWORD dwOwnerPID ) 
{ 
  HANDLE hThreadSnap = INVALID_HANDLE_VALUE; 
  THREADENTRY32 te32; 
 
  // Take a snapshot of all running threads  
  hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 ); 
  if( hThreadSnap == INVALID_HANDLE_VALUE ) 
    return( FALSE ); 
 
  // Fill in the size of the structure before using it. 
  te32.dwSize = sizeof(THREADENTRY32 ); 
 
  // Retrieve information about the first thread,
  // and exit if unsuccessful
  if( !Thread32First( hThreadSnap, &te32 ) ) 
  {
    printError( TEXT("Thread32First") );  // Show cause of failure
    CloseHandle( hThreadSnap );     // Must clean up the snapshot object!
    return( FALSE );
  }

  // Now walk the thread list of the system,
  // and display information about each thread
  // associated with the specified process
  do 
  { 
    if( te32.th32OwnerProcessID == dwOwnerPID )
    {
      _tprintf( TEXT("\n     THREAD ID      = 0x%08X"), te32.th32ThreadID ); 
      _tprintf( TEXT("\n     base priority  = %d"), te32.tpBasePri ); 
      _tprintf( TEXT("\n     delta priority = %d"), te32.tpDeltaPri ); 
    }
  } while( Thread32Next(hThreadSnap, &te32 ) );

  _tprintf( TEXT("\n"));

//  Don't forget to clean up the snapshot object.
  CloseHandle( hThreadSnap );
  return( TRUE );
}

void printError( TCHAR* msg )
{
  DWORD eNum;
  TCHAR sysMsg[256];
  TCHAR* p;

  eNum = GetLastError( );
  FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL, eNum,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         sysMsg, 256, NULL );

  // Trim the end of the line and terminate it with a null
  p = sysMsg;
  while( ( *p > 31 ) || ( *p == 9 ) )
    ++p;
  do { *p-- = 0; } while( ( p >= sysMsg ) &&
                          ( ( *p == '.' ) || ( *p < 33 ) ) );

  // Display the message
  _tprintf( TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg );
}
```

### Traversing the module list

```c++
#include <windows.h> 
#include <tlhelp32.h> 
#include <tchar.h> 
 
//  Forward declarations: 
BOOL ListProcessModules( DWORD dwPID ); 
void printError( TCHAR* msg ); 
 
int main( void )
{
  ListProcessModules(GetCurrentProcessId() );
  return 0;
}

BOOL ListProcessModules( DWORD dwPID ) 
{ 
  HANDLE hModuleSnap = INVALID_HANDLE_VALUE; 
  MODULEENTRY32 me32; 
 
//  Take a snapshot of all modules in the specified process. 
  hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwPID ); 
  if( hModuleSnap == INVALID_HANDLE_VALUE ) 
  { 
    printError( TEXT("CreateToolhelp32Snapshot (of modules)") ); 
    return( FALSE ); 
  } 
 
//  Set the size of the structure before using it. 
  me32.dwSize = sizeof( MODULEENTRY32 ); 
 
//  Retrieve information about the first module, 
//  and exit if unsuccessful 
  if( !Module32First( hModuleSnap, &me32 ) ) 
  { 
    printError( TEXT("Module32First") );  // Show cause of failure 
    CloseHandle( hModuleSnap );     // Must clean up the snapshot object! 
    return( FALSE ); 
  } 
 
//  Now walk the module list of the process, 
//  and display information about each module 
  do 
  { 
    _tprintf( TEXT("\n\n     MODULE NAME:     %s"),             me32.szModule ); 
    _tprintf( TEXT("\n     executable     = %s"),             me32.szExePath ); 
    _tprintf( TEXT("\n     process ID     = 0x%08X"),         me32.th32ProcessID ); 
    _tprintf( TEXT("\n     ref count (g)  =     0x%04X"),     me32.GlblcntUsage ); 
    _tprintf( TEXT("\n     ref count (p)  =     0x%04X"),     me32.ProccntUsage ); 
    _tprintf( TEXT("\n     base address   = 0x%08X"), (DWORD) me32.modBaseAddr ); 
    _tprintf( TEXT("\n     base size      = %d"),             me32.modBaseSize ); 
 
  } while( Module32Next( hModuleSnap, &me32 ) ); 

    _tprintf( TEXT("\n"));
 
//  Do not forget to clean up the snapshot object. 
  CloseHandle( hModuleSnap ); 
  return( TRUE ); 
} 
 
 
void printError( TCHAR* msg )
{
  DWORD eNum;
  TCHAR sysMsg[256];
  TCHAR* p;

  eNum = GetLastError( );
  FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL, eNum,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         sysMsg, 256, NULL );

  // Trim the end of the line and terminate it with a null
  p = sysMsg;
  while( ( *p > 31 ) || ( *p == 9 ) )
    ++p;
  do { *p-- = 0; } while( ( p >= sysMsg ) &&
                          ( ( *p == '.' ) || ( *p < 33 ) ) );

  // Display the message
  _tprintf( TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg );
}
```

### Traversing the heap list

```c++
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

int main( void )
{
   HEAPLIST32 hl;
   
   HANDLE hHeapSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, GetCurrentProcessId());
   
   hl.dwSize = sizeof(HEAPLIST32);
   
   if ( hHeapSnap == INVALID_HANDLE_VALUE )
   {
      printf ("CreateToolhelp32Snapshot failed (%d)\n", GetLastError());
      return 1;
   }
   
   if( Heap32ListFirst( hHeapSnap, &hl ) )
   {
      do
      {
         HEAPENTRY32 he;
         ZeroMemory(&he, sizeof(HEAPENTRY32));
         he.dwSize = sizeof(HEAPENTRY32);

         if( Heap32First( &he, GetCurrentProcessId(), hl.th32HeapID ) )
         {
            printf( "\nHeap ID: %d\n", hl.th32HeapID );
            do
            {
               printf( "Block size: %d\n", he.dwBlockSize );
               
               he.dwSize = sizeof(HEAPENTRY32);
            } while( Heap32Next(&he) );
         }
         hl.dwSize = sizeof(HEAPLIST32);
      } while (Heap32ListNext( hHeapSnap, &hl ));
   }
   else printf ("Cannot list first heap (%d)\n", GetLastError());
   
   CloseHandle(hHeapSnap); 

   return 0;
}
```







