# linux 网卡信息设置

## nmcli

### device

#### status  
第四列： CONNECTION　网卡对应的配置文件名称（配置文件名可以通过nmcli connection modify uuid xxxxxxxxx con-name xxx来修改配置文件名） 
```shell
lu@ubuntu22:~$ nmcli device status
DEVICE   TYPE      STATE         CONNECTION 
enp0s3   ethernet  已连接        有线连接   
docker0  bridge    连接（外部）  docker0    
lo       loopback  未托管        --         
lu@ubuntu22:~$ 
lu@ubuntu22:~$ nmcli connection show
NAME      UUID                                  TYPE      DEVICE  
有线连接  b19d0454-ba52-3062-80e4-9d7e33703792  ethernet  enp0s3  
docker0   d9ae1b8c-64cb-4ae1-bf62-fb17c8d3af37  bridge    docker0 
lu@ubuntu22:~$ sudo nmcli connection  modify uuid b19d0454-ba52-3062-80e4-9d7e33703792 con-name 有线连接1
[sudo] lu 的密码： 
lu@ubuntu22:~$ nmcli connection show
NAME       UUID                                  TYPE      DEVICE  
有线连接1  b19d0454-ba52-3062-80e4-9d7e33703792  ethernet  enp0s3  
docker0    d9ae1b8c-64cb-4ae1-bf62-fb17c8d3af37  bridge    docker0 
lu@ubuntu22:~$ 
```
#### show
show后面加上DEVICE名称，则显示该网卡的所有信息。
```shell
lu@ubuntu22:~$ nmcli device show enp0s3 
GENERAL.DEVICE:                         enp0s3
GENERAL.TYPE:                           ethernet
GENERAL.HWADDR:                         08:00:27:D4:7F:1C
GENERAL.MTU:                            1500
GENERAL.STATE:                          100（已连接）
GENERAL.CONNECTION:                     有线连接1
GENERAL.CON-PATH:                       /org/freedesktop/NetworkManager/ActiveConnection/4
WIRED-PROPERTIES.CARRIER:               开启
IP4.ADDRESS[1]:                         10.0.2.15/24
IP4.GATEWAY:                            10.0.2.2
IP4.ROUTE[1]:                           dst = 10.0.2.0/24, nh = 0.0.0.0, mt = 100
IP4.ROUTE[2]:                           dst = 0.0.0.0/0, nh = 10.0.2.2, mt = 100
IP4.ROUTE[3]:                           dst = 169.254.0.0/16, nh = 0.0.0.0, mt = 1000
IP4.DNS[1]:                             192.168.1.1
IP6.ADDRESS[1]:                         fe80::1a8f:9a1d:21d4:64d1/64
IP6.GATEWAY:                            --
IP6.ROUTE[1]:                           dst = fe80::/64, nh = ::, mt = 1024

```
#### disconnect
disconnect　　断开网卡与配置文件之间的连接
```shell
lu@ubuntu22:~$ nmcli device disconnect enp0s3 
成功断开设备 "enp0s3"。
lu@ubuntu22:~$ nmcli device status 
DEVICE   TYPE      STATE         CONNECTION 
docker0  bridge    连接（外部）  docker0    
enp0s3   ethernet  已断开        --         
lo       loopback  未托管        --         
lu@ubuntu22:~$ nmcli device connect enp0s3 
成功用 "enp0s3b19d0454-ba52-3062-80e4-9d7e33703792" 激活了设备 ""。
lu@ubuntu22:~$ nmcli device status 
DEVICE   TYPE      STATE         CONNECTION 
enp0s3   ethernet  已连接        有线连接1  
docker0  bridge    连接（外部）  docker0    
lo       loopback  未托管        --         
lu@ubuntu22:~$ 

```

### connction  
用来配置配置文件，主要用来针对配置文件信息
```shell
lu@ubuntu22:~$ nmcli connection 
add      clone    delete   down     edit     export   help     import   load     modify   monitor  reload   show     up       
```
主要有add添加配置文件、delete删除配置文件、load加载、reload重新加载配置文件、show显示配置文件、down关闭、modify修改、up激活。  
无论是delete还是connection后紧跟什么参数，都可以通过name，uuid来删除修改。

#### show
显示配置文件概述信息
第一列：NAME　配置文件的名称（可以重复）


```shell
lu@ubuntu22:~$ nmcli connection show
NAME       UUID                                  TYPE      DEVICE  
有线连接1  b19d0454-ba52-3062-80e4-9d7e33703792  ethernet  enp0s3  
docker0    d9ae1b8c-64cb-4ae1-bf62-fb17c8d3af37  bridge    docker0 

```
#### add
添加配置文件（注意网卡只有一个配置文件是活动的，但是可以存在多个配置文件，可以通过connection.autoconnect-priority 设置配置文件的优先级）。  
如果在add添加配置文件时没有ipv4.method 则Linux会自动获取ip地址，即dhcp。connection.autoconnect 为开机是否自动连接


### general

#### status
显示网络管理器的整体状态。
```shell
lu@ubuntu22:~$ nmcli general status 
STATE   CONNECTIVITY  WIFI-HW  WIFI    WWAN-HW  WWAN   
已连接  完全          已启用   已启用  已启用   已启用 
```
#### permissions
显示当前用户对网络管理器可允许的操作权限。 如启用和禁用网络、更改WI-FI和WWAN状态、修改连接等。
```shell
lu@ubuntu22:~$ nmcli general permissions 
PERMISSION                                                        VALUE 
org.freedesktop.NetworkManager.checkpoint-rollback                验证  
org.freedesktop.NetworkManager.enable-disable-connectivity-check  否    
org.freedesktop.NetworkManager.enable-disable-network             否    
org.freedesktop.NetworkManager.enable-disable-statistics          否    
org.freedesktop.NetworkManager.enable-disable-wifi                否    
org.freedesktop.NetworkManager.enable-disable-wimax               否    
org.freedesktop.NetworkManager.enable-disable-wwan                否    
org.freedesktop.NetworkManager.network-control                    验证  
org.freedesktop.NetworkManager.reload                             验证  
org.freedesktop.NetworkManager.settings.modify.global-dns         验证  
org.freedesktop.NetworkManager.settings.modify.hostname           验证  
org.freedesktop.NetworkManager.settings.modify.own                验证  
org.freedesktop.NetworkManager.settings.modify.system             否    
org.freedesktop.NetworkManager.sleep-wake                         否    
org.freedesktop.NetworkManager.wifi.scan                          验证  
org.freedesktop.NetworkManager.wifi.share.open                    否    
org.freedesktop.NetworkManager.wifi.share.protected               否

```
### radio 
无线限传输控制
```shell 
lu@ubuntu22:~$ nmcli radio all 
WIFI-HW  WIFI    WWAN-HW  WWAN   
已启用   已启用  已启用   已启用 
lu@ubuntu22:~$ sudo nmcli radio wifi on
lu@ubuntu22:~$ sudo nmcli radio wifi off 
```

## NetworkManager
NetworkManager是一个为系统提供自动连接网络的检测和配置的程序。NetworkManager 的功能对于无线和有线网络都很有用。对于无线网络，NetworkManager 更喜欢已知的无线网络，并且能够切换到最可靠的网络。

### 附加接口
nm-connection-editor用于图形用户界面，  
系统托盘小程序的网络管理器小程序nm-applet。  

### 其他
其他参见：https://wiki.archlinux.org/title/NetworkManager  

## 静态ip与手动ip
静态 IP 意味着 DHCP 服务器检查设备的 MAC 地址，如果在静态分配列表中找到该地址，则会将该 IP 分配给该设备。否则，它将获取下一个可用的“随机”IP 地址。

手动意味着您手动设置设备上的所有内容。因此，它将向网关询问该特定的 IP 地址，如果可用，网关会将其分配给该地址。如果任何设置不正确，设备将根本无法获取 IP。

## 判断是dhcp还是静态ip

If the output is auto, then it is DHCP.  
If the output is manual, then it is static.
```shell
lu@ubuntu22:~$ nmcli -f ipv4.method connection show 有线连接1
ipv4.method:                            auto
lu@ubuntu22:~$ 

```

## 参考

- [nmcli工具详解](https://www.cnblogs.com/liuhedong/p/10695969.html)
- [NMCLI DEVICE AND NMCLI CONNECTION](https://www.cnblogs.com/stdin/p/15026175.html)
- [NetworkManager](https://wiki.archlinux.org/title/NetworkManager)
- [manual IP vs static ip](https://forums.tomshardware.com/threads/static-ip-vs-manual-ip.3026281/)
- [dhcp-or-static](https://stackoverflow.com/questions/61345988/check-networkmanager-is-dhcp-or-static-with-command-line-ubuntu)
- [dhcp-or-static](https://superuser.com/questions/955790/determine-if-its-a-static-ip-or-a-dhcp-ip)
- [LinuxTutorialNetworking](http://www.yolinux.com/TUTORIALS/LinuxTutorialNetworking.html)
- [nmcli](https://developer-old.gnome.org/NetworkManager/stable/nmcli.html)
