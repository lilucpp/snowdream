# 服务

cp xx.service /dev/shm/xx.service

systemctl enable --all  /dev/shm/xx.service


目前uos提权：
1.  服务启动方式
    systemctl enable /dev/shm/***.service  
    systemctl start  ***
2.  脚本执行方式
    systemd-run -t /bin/bash -c "***.sh"


##  服务监听
### uos上验签服务监听示例
服务开启关闭是通过systemd控制的， 可以通过监听信号来知道服务状态。
监听dbus信号，比如： dbus-monitor --system path=/org/freedesktop/systemd1/unit/deepin_2delf_2dverify_2eservice  
验签服务：
服务名是deepin-elf-verify.service    在dbus中这个服务的路径是/org/freedesktop/systemd1/unit/deepin_2delf_2dverify_2eservice  

监控关键字段：
```txt
开启 
dict entry(
         string "SubState"
         variant             string "running"
      )
关闭
      dict entry(
         string "SubState"
         variant             string "dead"
      )
```

### 示例代码
详见代码片段dbus。

```c
// sudo apt install libdbus-1-dev
// gcc dbus_monitor.c $(pkg-config dbus-1 --cflags --libs)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus.h>

#define YOUR_SIGNAL_NAME "PropertiesChanged"     // 请替换为实际的信号名
#define YOUR_INTERFACE_NAME "org.freedesktop.DBus.Properties"   // 请替换为实际的接口名
#define YOUR_PATH "/org/freedesktop/systemd1/unit/apache2_2eservice" // 请替换为实际路径

void parse_signal(DBusMessage *message) {
    DBusMessageIter args;
    dbus_message_iter_init(message, &args);

    char *interface_name;

    // 解析第一参数
    dbus_message_iter_get_basic(&args, &interface_name);

    if (strcmp(interface_name,"org.freedesktop.systemd1.Service") == 0){
        return;
    }
    while (dbus_message_iter_get_arg_type(&args) != DBUS_TYPE_INVALID) {
        int arg_type = dbus_message_iter_get_arg_type(&args);

        switch (arg_type) {
            case DBUS_TYPE_ARRAY:
                {
                    DBusMessageIter sub_iter, dict_iter;
                    dbus_message_iter_recurse(&args, &sub_iter);
                    if (dbus_message_iter_get_arg_type(&sub_iter) != DBUS_TYPE_DICT_ENTRY) {
                       break;
                    }
                    const char* key_cstr;
                    do {
	                    dbus_message_iter_recurse(&sub_iter, &dict_iter);
	                    if (dbus_message_iter_get_arg_type(&dict_iter) != DBUS_TYPE_STRING) {
	                    	continue;
	                    }
	                    dbus_message_iter_get_basic(&dict_iter, &key_cstr);
                        
                        if (strcmp(key_cstr,"ActiveState") == 0){
                            // printf("key_cstr: %s\n",key_cstr);
                            dbus_message_iter_next(&dict_iter);
                            dbus_message_iter_recurse(&dict_iter,&sub_iter);
                            const char* key_val;
	                        dbus_message_iter_get_basic(&sub_iter, &key_val);
                            // printf("key_val: %s\n",key_val);
                            if (strcmp(key_val,"deactivating") == 0){
                                printf("service stop!!\n");
                            }else if(strcmp(key_val,"activating") == 0){
                                printf("service start!!\n");
                            }
                        }  
	                } while (dbus_message_iter_next(&sub_iter)); 
                }
            // Add cases for other types as needed

            default:
                // fprintf(stderr, "Unhandled parameter type: %c\n", arg_type);
                break;
        }

        dbus_message_iter_next(&args);
    }
}

DBusHandlerResult message_handler(DBusConnection *connection, DBusMessage *message, void *user_data) {
    // printf("Received D-Bus signal: %s\n", dbus_message_get_member(message));
    // 在这里处理收到的信号，可以从 message 中提取相关信息
    parse_signal(message);
    return DBUS_HANDLER_RESULT_HANDLED;
}

// build: gcc main1.c `pkg-config --cflags --libs dbus-1`
int main() {
    DBusError error;
    dbus_error_init(&error);

    DBusConnection *connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "DBus Connection Error: %s\n", error.message);
        dbus_error_free(&error);
        exit(EXIT_FAILURE);
    }

    // 设置要监听的接口和信号
    const char *rule = "type='signal',path='" YOUR_PATH "',interface='" YOUR_INTERFACE_NAME "',member='" YOUR_SIGNAL_NAME "'";
    dbus_bus_add_match(connection, rule, &error);
    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "DBus Match Error: %s\n", error.message);
        dbus_error_free(&error);
        dbus_connection_close(connection);
        exit(EXIT_FAILURE);
    }

    // 设置 D-Bus 消息处理函数
    dbus_connection_add_filter(connection, message_handler, NULL, NULL);

    // 循环处理 D-Bus 事件
    while (1) {
        dbus_connection_read_write_dispatch(connection, -1);
    }

    return 0;
}

```