# 注销用户

## 杀进程的方式

pkill -SIGKILL -u user_name

## dbus方式

uos
```shell
dbus-send —print-reply —dest=com.deepin.SessionManager /com/deepin/SessionManager com.deepin.SessionManager.ForceLogout
```

kylin

```shell
gdbus call --session --dest org.gnome.SessionManager --object-path /org/gnome/SessionManager --method  org.gnome.SessionManager.logout
```

## 重启lightdm服务

```shell
systemctl restart lightdm.service
```