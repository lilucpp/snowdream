# usb设备

## udevadm

### info
udevadm info [OPTIONS] [DEVPATH|FILE] 查询udev数据库中的设备信息。  

```shell
Query sysfs or the udev database.
  -q --query=TYPE             Query device information:
       name                     Name of device node
       symlink                  Pointing to node
       path                     sysfs device path
       property                 The device properties
       all                      All values
  -p --path=SYSPATH           sysfs device path used for query or attribute walk
  -n --name=NAME              Node or symlink name used for query or attribute walk
  -a --attribute-walk         Print all key matches walking along the chain
                              of parent devices
  -d --device-id-of-file=FILE Print major:minor of device containing this file
```
example: 
```shell
lil@lil:~/xtask/test/udisks2-examples$ udevadm info --query=all --name=/dev/sda | grep ID_SERIAL
E: ID_SERIAL=WD_Elements_SE_SSD_323331383244343032303834-0:0
E: ID_SERIAL_SHORT=323331383244343032303834
lil@lil:~/xtask/test/udisks2-examples$ udevadm info --query=all --name=/dev/nvme0n1 | grep ID_SERIAL
E: ID_SERIAL_SHORT=88LB8194K5QS
E: ID_SERIAL=KXG50ZNV256G_TOSHIBA_88LB8194K5QS
```
### monitor
udevadm monitor [options] 监听内核事件和udev发送的events事件。打印事件发出的设备。可以通过比较内核或者udev事件的时间戳来分析事件时序。  
```txt
-p --property                            Print the event properties
-k --kernel                              Print kernel uevents
-u --udev                                Print udev events
```

### 自动挂载

在/etc/udev/rules.d  下创建一个.rules文件，然后执行sudo udevadm control --reload

```txt

ATTRS{idVendor}=="3535", ATTRS{idProduct}=="6387",  ENV{UDISKS_AUTO}="0", ENV{UDISKS_PRESENTATION_HIDE}="1", ENV{UDISKS_PRESENTATION_NOPOLICY}="1", ENV{UDISKS_AUTOMOUNT_HINT}="never", ENV{UDISKS_SYSTEM_INTERNAL}="1", ENV{UDISKS_IGNORE}="1"
```

## smartctl

需要安装smartmontools包： `sudo apt install smartmontools`

```txt
-a  打印磁盘所有的SMART信息
```

```shell
peanut@peanut-vm:~$ sudo smartctl -a /dev/sdb4
smartctl 6.6 2016-05-31 r4324 [x86_64-linux-5.4.0-146-generic] (local build)
Copyright (C) 2002-16, Bruce Allen, Christian Franke, www.smartmontools.org

=== START OF INFORMATION SECTION ===
Vendor:               aigo
Product:              U268
Revision:             8.07
User Capacity:        8,053,063,680 bytes [8.05 GB]
Logical block size:   512 bytes
Logical Unit id:      0x4945450005080f000x200049454505080f
Serial number:        FB280D80
Device type:          disk
Local Time is:        Mon Apr 15 17:54:06 2024 CST
SMART support is:     Available - device has SMART capability.
SMART support is:     Enabled
Temperature Warning:  Disabled or Not Supported

=== START OF READ SMART DATA SECTION ===
SMART Health Status: OK
Current Drive Temperature:     0 C
Drive Trip Temperature:        0 C

Error Counter logging not supported

Device does not support Self Test logging

```
## lsblk

 用于列出块设备的信息。

```shell
peanut@peanut-vm:~$ lsblk -J -no NAME,SERIAL,MOUNTPOINT,TYPE
{
   "blockdevices": [
      {"name": "loop0", "serial": null, "mountpoint": "/snap/gnome-logs/119", "type": "loop"},
      {"name": "sda", "serial": null, "mountpoint": null, "type": "disk",
         "children": [
            {"name": "sda1", "serial": null, "mountpoint": "/", "type": "part"}
         ]
      },
      {"name": "sdb", "serial": "FB280D81", "mountpoint": null, "type": "disk",
         "children": [
            {"name": "sdb4", "serial": null, "mountpoint": "/media/peanut/UOS", "type": "part"}
         ]
      },
      {"name": "sr0", "serial": "01000000000000000001", "mountpoint": null, "type": "rom"}
   ]
}
```

## 其他

检查和收集硬件信息的几个命令。  

- lscpu    # 用于查询CPU信息
- lshw     # 用于显示硬件信息表
- hwinfo   # 用于查询硬件信息
- lsppci   # 用于列出PCI总线的信息以及连接到PCI总线上的设备信息
- lsusb    # 用于列出USB总线信息
- lsscsi   # 用于列出SCSI的设备信息

