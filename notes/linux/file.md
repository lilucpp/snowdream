# file

## file exist

```c++
#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif

bool fileExist(const char* fname) {
    if (access(fname, F_OK) == 0) {
        return truej;
    } 

    return false;
}

```