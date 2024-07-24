# linux 下当前用户启动
## 服务启动
优点：可以使用输入法  
缺点：需要替换User。   
可以先enable， 启动，再disable。  
服务文件
```txt
[Service]
Environment="DISPLAY=:0"
# ??Environment="XAUTHORITY=/home/peanut/.Xauthority"
ExecStart=/opt/test
User=peanut
Type=simple
[Install]
WantedBy=graphical.target
```

sh
```shell
#!/bin/bash
touch /tmp/xxxx
gedit
```

## 脚本启动
获取当前用户可以参考三。  
```shell
#!/bin/bash

loginuser=`who | awk '{print $1}' | sort -u `
usernum=`who | awk '{print $1}' | sort -u | wc -l`
echo "usernum=$usernum"
echo "loginuser=$loginuser"

if  [ $usernum -eq  1 ];  then
  echo "change user2 $loginuser"
  xhost +
  export DISPLAY=:0.0
  export QT4_IM_MODULE=xim
  export QT_ACCESSIBILITY=1
  export QT_IM_MODULE=ibus

  setsid sudo su - $loginuser  -c   code &
else
  echo "run as root"
fi

echo "test....."
```

## 获取当前用户（active）
```shell
root@peanut-vm:xx# cat /sys/class/tty/tty0/active 
tty2

root@peanut-vm:xx# ps -ef|grep tty2
peanut     3073   2968  0 10:44 tty2     00:03:17 /usr/bin/gnome-shell
peanut     3108   3073  0 10:44 tty2     00:00:21 ibus-daemon --xim --panel disable
peanut     3112   3108  0 10:44 tty2     00:00:00 /usr/lib/ibus/ibus-dconf

```

### 脚本获取当前登陆用户。
ubuntu上
```shell
#!/bin/bash

# 读取当前活动的 tty 终端
active_tty=$(cat /sys/class/tty/tty0/active)

# 使用 ps -ef 命令获取系统中所有进程的详细信息，并使用 grep 过滤与当前活动 tty 终端相关联的进程
# 然后使用 awk 提取第一个匹配的进程的用户名
active_user=$(ps -ef | grep " $active_tty " | grep -v grep | awk '{print $1; exit}')

# 输出结果
echo "当前活动用户：$active_user"
```
kylin uos desktop/server
```shell

#!/bin/bash

# 读取当前活动终端名称
active_tty=$(cat /sys/class/tty/tty0/active)

# 使用 who 命令获取该终端对应的登录用户
active_user=$(who | grep "$active_tty" | awk '{print $1}')

echo "当前活动用户是: $active_user"
```

## qt程序在root下运行崩溃
在机随用户启动的进程中保存XDG_RUNTIME_DIR到文件currentuser
```shell
echo $XDG_RUNTIME_DIR > ./currentuser
chmod 666 ./currentuser
```
使用sudo运行进程时，做一层包装。
```shell
#!/bin/bash

export XDG_RUNTIME_DIR=$(cat ./currentuser)
./command
```