# 获取操作系统安装时间

linux上没有确定的方案获取安装时间，可以通过一些特定文件的变更（/lost+found/）， 获取则文件系统创建时间来作为操作系统安装的参考。

## 获取文件的时间

```shell
test@peanut-pc:~/桌面$ stat /lost+found/
  文件：/lost+found/
  大小：16384     	块：32         IO 块：4096   目录
设备：805h/2053d	Inode：11          硬链接：2
权限：(0700/drwx------)  Uid：(    0/    root)   Gid：(    0/    root)
最近访问：2023-10-13 14:04:49.278399623 +0800
最近更改：2023-06-29 12:26:38.000000000 +0800
最近改动：2023-06-29 12:26:38.000000000 +0800
创建时间：-
test@peanut-pc:~/桌面$ 

```

## 文件系统创建时间

```shell
test@peanut-pc:~/桌面$ df -h
文件系统        容量  已用  可用 已用% 挂载点
udev            1.9G     0  1.9G    0% /dev
tmpfs           391M  3.0M  388M    1% /run
/dev/sda5        17G   12G  4.2G   74% /
tmpfs           2.0G   41M  1.9G    3% /dev/shm
tmpfs           5.0M  4.0K  5.0M    1% /run/lock
tmpfs           2.0G     0  2.0G    0% /sys/fs/cgroup
/dev/sda7        12G  5.9G  4.7G   56% /data
/dev/sda1       976M  196M  713M   22% /boot
tmpfs           391M   24K  391M    1% /run/user/1000
tmpfs           391M   56K  391M    1% /run/user/1001
test@peanut-pc:~/桌面$ sudo dumpe2fs /dev/sda5 | grep created
dumpe2fs 1.45.5 (07-Jan-2020)
Filesystem created:       Thu Jun 29 12:26:38 2023
test@peanut-pc:~/桌面$ 

```
或者用tune2fs -l /dev/sda1 | grep Filesystem命令来获知文件系统创建时间，作为装机时间的参考。

```shell
test@peanut-pc:~/桌面$ sudo tune2fs -l /dev/sda1 | grep created
Filesystem created:       Thu Jun 29 12:26:38 2023
test@peanut-pc:~/桌面$ 

```

## 参考
- [uos](https://faq.uniontech.com/desktop/f435/install/7d85)
- [kylin](https://www.kylinos.cn/support/problem.html)
- [How do I find how long ago a Linux system was installed?](https://unix.stackexchange.com/a/9993)
- [How to check Linux Operating System (OS) installation Date And Time](https://www.2daygeek.com/linux-os-installation-date-time/)