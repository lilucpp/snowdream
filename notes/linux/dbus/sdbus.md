# sdbus

## busctl

一段时间以来，systemd 已经包含了一个`busctl`对于探索 D-Bus 对象系统并与之交互非常有用的工具。当不带参数调用时，它将显示连接到系统总线的所有对等点的列表。（用于`--user`查看用户总线的对等点）：

```
$ busctl
NAME                                       PID PROCESS         USER             CONNECTION    UNIT                      SESSION    DESCRIPTION
:1.1                                         1 systemd         root             :1.1          -                         -          -
:1.11                                      705 NetworkManager  root             :1.11         NetworkManager.service    -          -
:1.14                                      744 gdm             root             :1.14         gdm.service               -          -
:1.4                                       708 systemd-logind  root             :1.4          systemd-logind.service    -          -
:1.7200                                  17563 busctl          lennart          :1.7200       session-1.scope           1          -
[…]
org.freedesktop.NetworkManager             705 NetworkManager  root             :1.11         NetworkManager.service    -          -
org.freedesktop.login1                     708 systemd-logind  root             :1.4          systemd-logind.service    -          -
org.freedesktop.systemd1                     1 systemd         root             :1.1          -                         -          -
org.gnome.DisplayManager                   744 gdm             root             :1.14         gdm.service               -          -
[…]
```

（为了保持简洁，我稍微缩短了输出）。

该列表以当前连接到总线的所有对等点的列表开头。它们通过“:1.11”等对等名称来标识。这些在 D-Bus 命名法中称为 *唯一名称*。基本上，每个对等点都有一个唯一的名称，当对等点连接到总线时，它们会自动分配。如果您愿意的话，它们很像 IP 地址。您会注意到几个对等点已经连接，包括我们的小busctl 工具本身以及许多系统服务。然后，该列表显示总线上的所有实际服务，由服务名称标识（如上所述；为了从唯一名称中区分它们，这些名称也称为*众所周知的名称*）。

看该 `org.freedesktop.login1`服务实际提供了哪些对象：

```
$ busctl tree org.freedesktop.login1
└─/org/freedesktop/login1
  ├─/org/freedesktop/login1/seat
  │ ├─/org/freedesktop/login1/seat/seat0
  │ └─/org/freedesktop/login1/seat/self
  ├─/org/freedesktop/login1/session
  │ ├─/org/freedesktop/login1/session/_31
  │ └─/org/freedesktop/login1/session/self
  └─/org/freedesktop/login1/user
    ├─/org/freedesktop/login1/user/_1000
    └─/org/freedesktop/login1/user/self
```

很漂亮，不是吗？实际上更好的是，输出没有*显示*的是，可以使用完整的命令行补全：当您按 TAB 时，shell 会自动为您补全服务名称。以这种方式探索 D-Bus 对象真是一种乐趣！

输出显示了一些您可能从上面的解释中认识到的对象。现在，让我们更进一步。让我们看看这些对象之一实际公开了哪些接口、方法、信号和属性：

```
$ busctl introspect org.freedesktop.login1 /org/freedesktop/login1/session/_31
NAME                                TYPE      SIGNATURE RESULT/VALUE                             FLAGS
org.freedesktop.DBus.Introspectable interface -         -                                        -
.Introspect                         method    -         s                                        -
org.freedesktop.DBus.Peer           interface -         -                                        -
.GetMachineId                       method    -         s                                        -
.Ping                               method    -         -                                        -
org.freedesktop.DBus.Properties     interface -         -                                        -
.Get                                method    ss        v                                        -
.GetAll                             method    s         a{sv}                                    -
.Set                                method    ssv       -                                        -
.PropertiesChanged                  signal    sa{sv}as  -                                        -
org.freedesktop.login1.Session      interface -         -                                        -
.Activate                           method    -         -                                        -
.Kill                               method    si        -                                        -
.Lock                               method    -         -                                        -
.PauseDeviceComplete                method    uu        -                                        -
.ReleaseControl                     method    -         -                                        -
.ReleaseDevice                      method    uu        -                                        -
.SetIdleHint                        method    b         -                                        -
.TakeControl                        method    b         -                                        -
.TakeDevice                         method    uu        hb                                       -
.Terminate                          method    -         -                                        -
.Unlock                             method    -         -                                        -
.Active                             property  b         true                                     emits-change
.Audit                              property  u         1                                        const
.Class                              property  s         "user"                                   const
.Desktop                            property  s         ""                                       const
.Display                            property  s         ""                                       const
.Id                                 property  s         "1"                                      const
.IdleHint                           property  b         true                                     emits-change
.IdleSinceHint                      property  t         1434494624206001                         emits-change
.IdleSinceHintMonotonic             property  t         0                                        emits-change
.Leader                             property  u         762                                      const
.Name                               property  s         "lennart"                                const
.Remote                             property  b         false                                    const
.RemoteHost                         property  s         ""                                       const
.RemoteUser                         property  s         ""                                       const
.Scope                              property  s         "session-1.scope"                        const
.Seat                               property  (so)      "seat0" "/org/freedesktop/login1/seat... const
.Service                            property  s         "gdm-autologin"                          const
.State                              property  s         "active"                                 -
.TTY                                property  s         "/dev/tty1"                              const
.Timestamp                          property  t         1434494630344367                         const
.TimestampMonotonic                 property  t         34814579                                 const
.Type                               property  s         "x11"                                    const
.User                               property  (uo)      1000 "/org/freedesktop/login1/user/_1... const
.VTNr                               property  u         1                                        const
.Lock                               signal    -         -                                        -
.PauseDevice                        signal    uus       -                                        -
.ResumeDevice                       signal    uuh       -                                        -
.Unlock                             signal    -         -                                        -
```

和以前一样，busctl 命令支持命令行完成，因此只需按 TAB 键即可轻松地将所使用的服务名称和对象路径放在 shell 上。输出显示当前可用的会话对象之一的方法、属性和信号`systemd-logind`。对象知道的每个接口都有一个部分。第二列告诉您该行中显示的成员类型。第三列显示成员的签名。如果方法调用是输入参数，则第四列显示返回的内容。对于属性，第四列编码它们的当前值。

到目前为止，我们只是进行了探索。现在让我们进行下一步：让我们变得活跃起来 - 让我们调用一个方法：

```
# busctl call org.freedesktop.login1 /org/freedesktop/login1/session/_31 org.freedesktop.login1.Session Lock
```

我认为我不需要再提及这一点，但无论如何：再次提供完整的命令行完成功能。第三个参数是接口名称，第四个参数是方法名称，这两个参数都可以通过按 TAB 轻松完成。在本例中`Lock`，我们选择了为特定会话激活屏幕锁定的方法。是的，当我在这一行按下回车键时，我的屏幕锁定就打开了（这只适用于正确连接的桌面环境，`systemd-logind`GNOME 工作正常，KDE 也应该工作）。

我们选择的方法`Lock`调用非常简单，因为它不带任何参数，也不返回任何参数。当然，对于某些调用来说，情况可能会变得更加复杂。这是另一个示例，这次使用 systemd 自己的总线调用之一来启动任意系统单元：

```
# busctl call org.freedesktop.systemd1 /org/freedesktop/systemd1 org.freedesktop.systemd1.Manager StartUnit ss "cups.service" "replace"
o "/org/freedesktop/systemd1/job/42684"
```

此调用采用两个字符串作为输入参数，正如我们在方法名称后面的签名字符串中所表示的那样（与往常一样，命令行完成可以帮助您正确完成此操作）。签名之后的接下来的两个参数只是要传递的两个字符串。因此，指定的签名字符串指示接下来发生的事情。systemd 的 StartUnit 方法调用将要启动的单元名称作为第一个参数，将启动模式作为第二个参数。该调用返回单个对象路径值。它的编码方式与输入参数相同：签名（仅`o`适用于对象路径）后跟实际值。

当然，某些方法调用参数可能会变得更加复杂，但对`busctl`它们进行编码相对容易。有关详细信息，请参阅[手册页](http://www.freedesktop.org/software/systemd/man/busctl.html)。

`busctl`知道许多其他操作。例如，您可以使用它来监控发生的 D-Bus 流量（包括生成 `.cap`与 Wireshark 一起使用的文件！），或者您可以设置或获取特定属性。然而，这个博客故事应该是关于 sd-bus 的，而不是关于 sd-bus 的`busctl`，因此让我们在这里简短地说一下，如果您想了解有关该工具的更多信息，让我引导您访问手册页。

`busctl`（与系统的其余部分一样）是使用 sd-bus API 实现的。因此它暴露了 sd-bus 本身的许多特性。例如，您可以用于连接到远程或容器总线。它了解 kdbus 和经典 D-Bus 以及更多！



## SD总线

但够了！言归正传，我们来说说sd-bus本身。

sd-bus API 集主要包含在头文件 [sd-bus.h](https://github.com/systemd/systemd/blob/master/src/systemd/sd-bus.h)中。

这是该库的随机选择的功能，使其与其他可用的实现进行比较。

- 支持 kdbus 和 dbus1 作为后端。
- 具有对通过 ssh 连接到远程总线以及本地操作系统容器总线的高级支持。
- 强大的凭证模型，实现服务中客户端的认证。目前支持 34 个单独的字段，从客户端的 PID 到 cgroup 或功能集。
- 支持跟踪对等点的生命周期，以便在引用本地对象的所有对等点断开连接时自动释放本地对象。
- 客户端构建一个高效的决策树来确定将传入总线消息传递给哪些处理程序。
- 自动将 D-Bus 错误转换为 UNIX 风格的错误并转换回来（尽管这是有损的），以确保 D-Bus 最好地集成到低级 Linux 程序中。
- 强大但轻量级的对象模型，用于在总线上公开本地对象。根据需要自动生成内省。

该 API 目前尚未完整记录，但我们正在努力完成一组手册页。有关详细信息， [请参阅以 开头的所有页面`sd_bus_`](http://www.freedesktop.org/software/systemd/man/index.html#S)。

### 使用 sd-bus 从 C 调用方法

关于图书馆的总体情况就这么多了。以下是连接到总线并发出方法调用的示例：

```c
#include <stdio.h>
#include <stdlib.h>
#include <systemd/sd-bus.h>

int main(int argc, char *argv[]) {
        sd_bus_error error = SD_BUS_ERROR_NULL;
        sd_bus_message *m = NULL;
        sd_bus *bus = NULL;
        const char *path;
        int r;

        /* Connect to the system bus */
        r = sd_bus_open_system(&bus);
        if (r < 0) {
                fprintf(stderr, "Failed to connect to system bus: %s\n", strerror(-r));
                goto finish;
        }

        /* Issue the method call and store the respons message in m */
        r = sd_bus_call_method(bus,
                               "org.freedesktop.systemd1",           /* service to contact */
                               "/org/freedesktop/systemd1",          /* object path */
                               "org.freedesktop.systemd1.Manager",   /* interface name */
                               "StartUnit",                          /* method name */
                               &error,                               /* object to return error in */
                               &m,                                   /* return message on success */
                               "ss",                                 /* input signature */
                               "cups.service",                       /* first argument */
                               "replace");                           /* second argument */
        if (r < 0) {
                fprintf(stderr, "Failed to issue method call: %s\n", error.message);
                goto finish;
        }

        /* Parse the response message */
        r = sd_bus_message_read(m, "o", &path);
        if (r < 0) {
                fprintf(stderr, "Failed to parse response message: %s\n", strerror(-r));
                goto finish;
        }

        printf("Queued service job as %s.\n", path);

finish:
        sd_bus_error_free(&error);
        sd_bus_message_unref(m);
        sd_bus_unref(bus);

        return r < 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
```

将此示例保存为`bus-client.c`，然后使用以下命令构建它：

```shell
$ gcc bus-client.c -o bus-client `pkg-config --cflags --libs libsystemd`
```

这将生成一个您现在可以运行的二进制文件`bus-client`。但请确保以 root 身份运行它，因为对该`StartUnit`方法的访问是有特权的：

```shell
# ./bus-client
Queued service job as /org/freedesktop/systemd1/job/3586.
```

这就是我们的第一个例子。它展示了我们如何在总线上调用方法。该方法的实际函数调用与`busctl`我们之前使用的命令行非常接近。我希望代码摘录不需要进一步解释。它应该让您体验如何使用 sd-bus 编写 D-Bus 客户端。有关更多信息，请查看头文件、手册页甚至 sd-bus 源代码。

### 使用 C 语言和 sd-bus 实现服务

当然，仅调用单个方法是一个相当简单的示例。让我们看看如何编写总线服务。我们将编写一个小型计算器服务，它公开一个对象，该对象实现一个公开两种方法的接口：一种用于将两个 64 位有符号整数相乘，另一种用于将一个 64 位有符号整数除以另一个。

```c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <systemd/sd-bus.h>

static int method_multiply(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
        int64_t x, y;
        int r;

        /* Read the parameters */
        r = sd_bus_message_read(m, "xx", &x, &y);
        if (r < 0) {
                fprintf(stderr, "Failed to parse parameters: %s\n", strerror(-r));
                return r;
        }

        /* Reply with the response */
        return sd_bus_reply_method_return(m, "x", x * y);
}

static int method_divide(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
        int64_t x, y;
        int r;

        /* Read the parameters */
        r = sd_bus_message_read(m, "xx", &x, &y);
        if (r < 0) {
                fprintf(stderr, "Failed to parse parameters: %s\n", strerror(-r));
                return r;
        }

        /* Return an error on division by zero */
        if (y == 0) {
                sd_bus_error_set_const(ret_error, "net.poettering.DivisionByZero", "Sorry, can't allow division by zero.");
                return -EINVAL;
        }

        return sd_bus_reply_method_return(m, "x", x / y);
}

/* The vtable of our little object, implements the net.poettering.Calculator interface */
static const sd_bus_vtable calculator_vtable[] = {
        SD_BUS_VTABLE_START(0),
        SD_BUS_METHOD("Multiply", "xx", "x", method_multiply, SD_BUS_VTABLE_UNPRIVILEGED),
        SD_BUS_METHOD("Divide",   "xx", "x", method_divide,   SD_BUS_VTABLE_UNPRIVILEGED),
        SD_BUS_VTABLE_END
};

int main(int argc, char *argv[]) {
        sd_bus_slot *slot = NULL;
        sd_bus *bus = NULL;
        int r;

        /* Connect to the user bus this time */
        r = sd_bus_open_user(&bus);
        if (r < 0) {
                fprintf(stderr, "Failed to connect to system bus: %s\n", strerror(-r));
                goto finish;
        }

        /* Install the object */
        r = sd_bus_add_object_vtable(bus,
                                     &slot,
                                     "/net/poettering/Calculator",  /* object path */
                                     "net.poettering.Calculator",   /* interface name */
                                     calculator_vtable,
                                     NULL);
        if (r < 0) {
                fprintf(stderr, "Failed to issue method call: %s\n", strerror(-r));
                goto finish;
        }

        /* Take a well-known service name so that clients can find us */
        r = sd_bus_request_name(bus, "net.poettering.Calculator", 0);
        if (r < 0) {
                fprintf(stderr, "Failed to acquire service name: %s\n", strerror(-r));
                goto finish;
        }

        for (;;) {
                /* Process requests */
                r = sd_bus_process(bus, NULL);
                if (r < 0) {
                        fprintf(stderr, "Failed to process bus: %s\n", strerror(-r));
                        goto finish;
                }
                if (r > 0) /* we processed a request, try to process another one, right-away */
                        continue;

                /* Wait for the next request to process */
                r = sd_bus_wait(bus, (uint64_t) -1);
                if (r < 0) {
                        fprintf(stderr, "Failed to wait on bus: %s\n", strerror(-r));
                        goto finish;
                }
        }

finish:
        sd_bus_slot_unref(slot);
        sd_bus_unref(bus);

        return r < 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
```

将此示例保存为`bus-service.c`，然后使用以下命令构建它：

```
$ gcc bus-service.c -o bus-service `pkg-config --cflags --libs libsystemd`
```

现在，让我们运行它：

```
$ ./bus-service
```

在另一个终端中，让我们尝试与它交谈。请注意，该服务现在位于用户总线上，而不是像以前那样位于系统总线上。我们这样做是为了简单起见：在系统总线上对服务的访问受到严格控制，因此非特权客户端无法请求特权操作。然而，在用户总线上，事情更简单：因为只有拥有总线的用户进程才能连接，进一步的策略执行不会使这个示例复杂化。因为服务是在用户总线上的，所以我们要通过命令行`--user`上的开关`busctl` 。让我们首先查看服务的对象树。

```
$ busctl --user tree net.poettering.Calculator
└─/net/poettering/Calculator
```

正如我们所看到的，服务上只有一个对象，这并不奇怪，因为我们上面的代码只注册了一个对象。让我们看看这个对象公开的接口和成员：

```
$ busctl --user introspect net.poettering.Calculator /net/poettering/Calculator
NAME                                TYPE      SIGNATURE RESULT/VALUE FLAGS
net.poettering.Calculator           interface -         -            -
.Divide                             method    xx        x            -
.Multiply                           method    xx        x            -
org.freedesktop.DBus.Introspectable interface -         -            -
.Introspect                         method    -         s            -
org.freedesktop.DBus.Peer           interface -         -            -
.GetMachineId                       method    -         s            -
.Ping                               method    -         -            -
org.freedesktop.DBus.Properties     interface -         -            -
.Get                                method    ss        v            -
.GetAll                             method    s         a{sv}        -
.Set                                method    ssv       -            -
.PropertiesChanged                  signal    sa{sv}as  -            -
```

如上所述，sd-bus 库自动添加了几个通用接口。但我们看到的第一个界面实际上是我们添加的界面！它显示了我们的两个方法，都以“xx”（两个64位有符号整数）作为输入参数，并返回一个“x”。伟大的！但这有效吗？

```
$ busctl --user call net.poettering.Calculator /net/poettering/Calculator net.poettering.Calculator Multiply xx 5 7
x 35
```

呜呼！我们传递了两个整数 5 和 7，服务实际上为我们将它们相乘并返回一个整数 35！让我们尝试一下另一种方法：

```
$ busctl --user call net.poettering.Calculator /net/poettering/Calculator net.poettering.Calculator Divide xx 99 17
x 5
```

哦，哇！它甚至可以进行整数除法！极好的！但让我们欺骗它除以零：

```
$ busctl --user call net.poettering.Calculator /net/poettering/Calculator net.poettering.Calculator Divide xx 43 0
Sorry, can't allow division by zero.
```

好的！它很好地检测到了这一点并返回了一个关于它的干净错误。如果您查看上面的源代码示例，您将看到我们生成错误的精确程度。

### 文档

https://www.freedesktop.org/software/systemd/man/latest/sd-bus.html

## cpp wrapper

可以使用sdbus-cpp https://github.com/Kistler-Group/sdbus-cpp/tree/master, 不过是使用c++17实现的。

