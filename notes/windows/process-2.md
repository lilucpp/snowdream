# 进程相关

1. 判断进程是否运行

```c++
#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>

// https://stackoverflow.com/a/14751302/8330816
/*!
\brief Check if a process is running
\param [in] processName Name of process to check if is running
\returns \c True if the process is running, or \c False if the process is not running
*/
bool IsProcessRunning(const wchar_t *processName)
{
    bool exists = false;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry))
        while (Process32Next(snapshot, &entry))
            if (!wcsicmp(entry.szExeFile, processName))
                exists = true;

    CloseHandle(snapshot);
    return exists;
}
```

2. 结束进程

```c++
BOOL KillProcess(int nType, const WCHAR* szPathName, const WCHAR* param)
{
    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(pe32);
    HANDLE hexit;
    BOOL bRet = FALSE;

    HANDLE hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    if (hsnapshot != INVALID_HANDLE_VALUE)
    {
        BOOL bMore = Process32FirstW(hsnapshot,&pe32);
        while(bMore)
        {
            if (_wcsnicmp(pe32.szExeFile, szPathName, wcslen(szPathName)) == 0 && pe32.th32ProcessID != GetCurrentProcessId())
            {
                hexit = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pe32.th32ProcessID);
                TerminateProcess(hexit,4);
                CloseHandle(hexit);
                bRet = TRUE;
            }

            bMore = Process32NextW(hsnapshot,&pe32);
        }

        CloseHandle(hsnapshot);
    }

    return bRet;
}
```