# 常见问题

1. kylin update2 上关闭增强模式

```txt
"/etc/kysec/kysec.conf"   kysec_status 改成0, 再update2上就可以修改文件了 
```

2. 提权
   目前uos提权：

- 服务启动方式
  - systemctl enable /dev/shm/***.service
  - systemctl start  ***
- 脚本执行方式
  systemd-run -t /bin/bash -c "***.sh"

3. 制作系统盘

- 使用软碟通
- 使用dd命令
  ```shell
  peanut@peanut-vm:~/Desktop$ sudo mkfs.vfat /dev/sdb -I
  [sudo] peanut 的密码： 
  mkfs.fat 4.1 (2017-01-24)
  attribute "partition" not found
  peanut@peanut-vm:~/Desktop$ sudo dd if=/xx.iso of=/dev/sdb bs=8M status=progress
  3797708800 bytes (3.8 GB, 3.5 GiB) copied, 294 s, 12.9 MB/s

  记录了452+1 的读入
  记录了452+1 的写出
  3797708800 bytes (3.8 GB, 3.5 GiB) copied, 457.304 s, 8.3 MB/s
  peanut@peanut-vm:~/Desktop$ 
  peanut@peanut-vm:~/Desktop$ 
  ```

4. uos是否锁屏
   ```shell
   dbus-send --session --print-reply --dest=com.deepin.SessionManager /com/deepin/SessionManager org.freedesktop.DBus.Properties.Get string:"com.deepin.SessionManager" string:"Locked"
   ```
