# dbus-sample

As we all know, a bus is of no use, unless you are connected. In order to get connected, you will need to execute the following code.

```c
DBusConnection * dbus_conn = nullptr;
DBusError dbus_error;

// Initialize D-Bus error
::dbus_error_init(&dbus_error);

// Connect to D-Bus
dbus_conn = ::dbus_bus_get(DBUS_BUS_SYSTEM, &dbus_error);
std::cout << "Connected to D-Bus as \"" << ::dbus_bus_get_unique_name(dbus_conn) << "\"." << std::endl;
```

Bango, now you're on the bus! Now you have access to every service connected to D-Bus (i.e. Bluetooth, network manager, system power information, etc...). The services on D-Bus are exposed via interfaces, which describe methods, events and properties. If you are familiar with object-oriented programming this should all be very intuitive.

Bango, now you're on the bus! Now you have access to every service connected to D-Bus(i.e. Bluetooth, network manager, system power information, etc...). D-Bus 上的服务通过接口公开，接口描述方法、事件和属性。如果您熟悉面向对象编程，这应该是非常直观的。

> ***专业提示：**如果您是 Ubuntu 用户，并且希望了解 D-Bus 机制的实际应用，请从 Ubuntu App Store 下载[D-Feet](https://apps.ubuntu.com/cat/applications/d-feet/)，您可以轻松浏览通过 D-Bus 公开的服务。*

也可以看看

- [D-Bus 总线 API](https://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga8a9024c78c4ea89b6271f19dbc7861b2)

## 接线员，您能帮我拨打这个电话吗？

`Introspectable`所有 D-Bus 服务都应该提供一个通过单一方法调用的接口`Introspect`。这允许您递归访问通过 D-Bus 提供的每项服务。您可以通过使用调用`Introspect`.

最简单的起点是查询 D-Bus 本身。首先，您需要为 D-Bus 守护程序（操作员）制定一条消息以传递到 D-Bus 服务。您可以使用以下代码完成此调用。

```c
DBusMessage * dbus_msg = nullptr;
DBusMessage * dbus_reply = nullptr;

// Compose remote procedure call
dbus_msg = ::dbus_message_new_method_call("org.freedesktop.DBus", "/", "org.freedesktop.DBus.Introspectable", "Introspect");

// Invoke remote procedure call, block for response
dbus_reply = ::dbus_connection_send_with_reply_and_block(dbus_conn, dbus_msg, DBUS_TIMEOUT_USE_DEFAULT, &dbus_error);
```



尤里卡！您刚刚通过 D-Bus 与系统进行了通信。退一步想想到目前为止你已经取得了什么成就。您刚刚查询过系统！相同的模式将允许访问系统上的几乎所有服务（即蓝牙）。

也可以看看

- [D-Bus 连接 API](https://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga8d6431f17a9e53c9446d87c2ba8409f0)
- [D-Bus消息API](https://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#gad8953f53ceea7de81cde792e3edd0230)

## 我的宝贝，刚刚给我写了一封信

到目前为止，我们已经连接到D-Bus（守护进程），我们已经查询了D-Bus服务并且收到了响应。*然而*，我们有一个不太理解的信息。这是因为它被*整理*成二进制格式，作为消息发送更容易、更高效（但人类更难阅读）。

现在是时候解码响应消息并查看其中编码的数据了。幸运的是，D-Bus 开发人员提供了破解消息并获取内容所需的所有工具。使用以下代码来理解响应。

```c
const char * dbus_result = nullptr;

// Parse response
::dbus_message_get_args(dbus_reply, &dbus_error, DBUS_TYPE_STRING, &dbus_result, DBUS_TYPE_INVALID);

// Work with the results of the remote procedure call
std::cout << "Introspection Result:" << std::endl << std::endl;
std::cout << dbus_result << std::endl;
```



现在我们已经从消息中取出了字符串。该字符串是 XML，详细说明了通过 D-Bus 提供的接口。它描述了接口、方法及其参数、属性和信号。以下是 的回复片段`Introspect`。

```xml
  ...
  <interface name="org.freedesktop.DBus.Introspectable">
    <method name="Introspect">
      <arg direction="out" type="s"/>
    </method>
  </interface>
  ...
```



上面的代码片段是`Introspectable`我们用来获取此信息的接口。正如您所看到的，它详细说明了任何可用的方法（即`Introspect`）以及它们所需的参数和数据类型（在本例中没有）。请注意，即使我们没有任何调用参数，我们也会看到结果被描述为`out`类型`s`（或字符串）的参数。

也可以看看

- [D-Bus消息API](https://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#gad8953f53ceea7de81cde792e3edd0230)

## 取出纸张和垃圾

好吧，“困难的部分”已经过去了，现在是时候清理了。回想一下我们分配的资源，我们建立了连接，创建了一条消息，接收了一条消息并从该消息中提取了一个字符串。让我们看看我们如何清理烂摊子。

在某种程度上，涉及 D-Bus 的所有内容都是共享的，至少，消息必须在两个进程之间共享。幸运的是，我们只负责对内存的引用（即使我们创建了它），这大大简化了事情。我们不再需要担心记忆何时出现或消失。我们只需要担心让系统知道我们何时使用完内存（想想`std::shared_pointer`）。以下代码让系统知道我们已经完成了消息的使用。

```
::dbus_message_unref(dbus_msg);
::dbus_message_unref(dbus_reply);
```



那么我们从消息中提取的字符串呢？显然，它会随着消息被清理掉。

D-Bus 文档指出：

> “除了字符串数组之外，返回的值都是常量；不要释放它们。它们指向 DBusMessage。”

很公平，但是与 D-Bus 本身的连接又如何呢？好吧，我们正在附加一个“共享连接”，因此我们不允许关闭它。事实上，如果您尝试关闭与`dbus_connection_close`api 的连接，那么库会向您抛出一条不错的错误消息（*如下所示*）；打你的手。

```c
process nnnn: Applications must not close shared connections - see dbus_connection_close() docs. This is a bug in the application.
```

我们不会关闭连接，而是像处理消息一样*取消对连接的引用。*

```c
::dbus_connection_unref(dbus_conn);
```

也可以看看

- [D-Bus消息API](https://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#gad8953f53ceea7de81cde792e3edd0230)

## 代码

```c
/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

/*
 * Filename: dbus.cpp
 *
 * Purpose: A simple sample of a D-Bus interaction.
 */

#include <dbus/dbus.h>

#include <cstddef>
#include <cstdio>
#include <iostream>

int
main (
  int argc,
  char * argv[]
) {
    (void)argc;
    (void)argv;
    DBusError dbus_error;
    DBusConnection * dbus_conn = nullptr;
    DBusMessage * dbus_msg = nullptr;
    DBusMessage * dbus_reply = nullptr;
    const char * dbus_result = nullptr;

    // Initialize D-Bus error
    ::dbus_error_init(&dbus_error);

    // Connect to D-Bus
    if ( nullptr == (dbus_conn = ::dbus_bus_get(DBUS_BUS_SYSTEM, &dbus_error)) ) {
        ::perror(dbus_error.name);
        ::perror(dbus_error.message);

    // Compose remote procedure call
    } else if ( nullptr == (dbus_msg = ::dbus_message_new_method_call("org.freedesktop.DBus", "/", "org.freedesktop.DBus.Introspectable", "Introspect")) ) {
        ::dbus_connection_unref(dbus_conn);
        ::perror("ERROR: ::dbus_message_new_method_call - Unable to allocate memory for the message!");

    // Invoke remote procedure call, block for response
    } else if ( nullptr == (dbus_reply = ::dbus_connection_send_with_reply_and_block(dbus_conn, dbus_msg, DBUS_TIMEOUT_USE_DEFAULT, &dbus_error)) ) {
        ::dbus_message_unref(dbus_msg);
        ::dbus_connection_unref(dbus_conn);
        ::perror(dbus_error.name);
        ::perror(dbus_error.message);

    // Parse response
    } else if ( !::dbus_message_get_args(dbus_reply, &dbus_error, DBUS_TYPE_STRING, &dbus_result, DBUS_TYPE_INVALID) ) {
        ::dbus_message_unref(dbus_msg);
        ::dbus_message_unref(dbus_reply);
        ::dbus_connection_unref(dbus_conn);
        ::perror(dbus_error.name);
        ::perror(dbus_error.message);

    // Work with the results of the remote procedure call
    } else {
        std::cout << "Connected to D-Bus as \"" << ::dbus_bus_get_unique_name(dbus_conn) << "\"." << std::endl;
        std::cout << "Introspection Result:" << std::endl << std::endl;
        std::cout << dbus_result << std::endl;
        ::dbus_message_unref(dbus_msg);
        ::dbus_message_unref(dbus_reply);

        /*
         * Applications must not close shared connections -
         * see dbus_connection_close() docs. This is a bug in the application.
         */
        //::dbus_connection_close(dbus_conn);

        // When using the System Bus, unreference
        // the connection instead of closing it
        ::dbus_connection_unref(dbus_conn);
    }

    return 0;
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
```

编译命令

```
$ g++ dbus.cpp -std=c++0x $(pkg-config dbus-1 --cflags) -ldbus-1 -Werror -Wall -Wextra
```



## 参考

[dbus-sample](https://github.com/makercrew/dbus-sample)

