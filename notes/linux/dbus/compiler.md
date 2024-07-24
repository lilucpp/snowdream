# 编译

Tutorials like this one generally assume that you have some knowledge of the language it is written for, in this case C, as well as the operating system you will run it on.

Looking at the tutorial, I see that it only contains a `main` function. As such, you will need to add the proper `#include` directives in order for this to work:

```c
#include <stdlib.h>          // for exit()   
#include <dbus/dbus.h>       // for dbus_*   
#include <dbus/dbus-glib.h>  // for dbus_g_*
```

Also, you will need to compile the libraries (in this case `dbus` and `dbus-glib`), or use the pre-compiled ones from your operating system, in order to link them to the executable.

You will also need the header files provided with the source, or the "development" packages from your operating system.

Per example, on my Ubuntu workstation, I can install both the source and the header files like so:

```shell
sudo apt-get -y install dbus libdbus-1-dev libdbus-glib-1-2 libdbus-glib-1-dev
```

Once they are compiled (or properly installed), you proceed to compile the program. You will need to specify the proper include paths and libraries to link to the compiler/linker. Per example, with GCC and my current setup it would be:

```shell
gcc test.c -I/usr/include/dbus-1.0 \
           -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include \
           -I/usr/include/glib-2.0 \
           -I/usr/lib/x86_64-linux-gnu/glib-2.0/include/ \
           -ldbus-1 \
           -ldbus-glib-1 \
           -Wall -Wextra
```

This should create an executable `a.out` in the current directory.

Granted, I have a few years of experience with C and Linux so I get figure out all that stuff easily. If you're looking to start with C, you probably should start with something easier though.



## 参考

[How to compile a basic D-Bus](https://stackoverflow.com/a/14264364)