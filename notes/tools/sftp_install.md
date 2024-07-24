# 安装SFTP服务

## 安装openssl-server

```shell
peanut@peanut-vm:/home$ sudo apt install openssh-server
正在读取软件包列表... 完成
正在分析软件包的依赖关系树       
正在读取状态信息... 完成       
openssh-server 已经是最新版 (1:7.6p1-4ubuntu0.3)。
下列软件包是自动安装的并且现在不需要了：
  linux-headers-4.4.0-184 linux-headers-4.4.0-184-generic
  linux-image-4.4.0-184-generic linux-modules-4.4.0-184-generic
  linux-modules-extra-4.4.0-184-generic
使用'sudo apt autoremove'来卸载它(它们)。
升级了 0 个软件包，新安装了 0 个软件包，要卸载 0 个软件包，有 1735 个软件包未被升级。
peanut@peanut-vm:/home$ dpkg --get-selections | grep ssh
libssh-4:amd64					install
openssh-client					install
openssh-server					install
openssh-sftp-server				install
ssh-import-id					install
peanut@peanut-vm:/home$ 
```

使用sudo apt install openssh-server安装程序。然后使用dpkg --get-selections | grep ssh查看是否安装成功。



## 新建用户组 sftp-users，并新建用户sftp，配置

```shell
peanut@peanut-vm:/home$ sudo addgroup sftp-users
正在添加组"sftp-users" (GID 1001)...
完成。
peanut@peanut-vm:/home$ sudo adduser sftp
正在添加用户"sftp"...
正在添加新组"sftp" (1002)...
正在添加新用户"sftp" (1001) 到组"sftp"...
主目录"/home/sftp"已经存在。没有从"/etc/skel"复制文件。
输入新的 UNIX 密码： 
重新输入新的 UNIX 密码： 
passwd：已成功更新密码
正在改变 sftp 的用户信息
请输入新值，或直接敲回车键以使用默认值
	全名 []:  
	房间号码 []: 
	工作电话 []: 
	家庭电话 []: 
	其它 []: 
这些信息是否正确？ [Y/n] Y
peanut@peanut-vm:/home$ sudo usermod -G sftp-users -s /bin/false sftp
peanut@peanut-vm:/home$ sudo mkdir /home/sftp_root
peanut@peanut-vm:/home$ sudo mkdir /home/sftp_root/shared
peanut@peanut-vm:/home$ sudo chown peanut:sftp-users /home/sftp_root/shared
peanut@peanut-vm:/home$ sudo chmod 770 /home/sftp_root/shared
peanut@peanut-vm:/home$ 

```

其中 sudo chown peanut:sftp-users /home/sftp_root/shared的**peanut**为当前用户名。

## SSH配置文件

sudo vim  /etc/ssh/sshd_config
在sshd_config文件的最后，添加以下内容：

```shell
AllowGroups sftp-users
Match Group sftp-users
ChrootDirectory /home/sftp_root
AllowTcpForwarding no
X11Forwarding no
ForceCommand internal-sftp
```



然后重启服务：sudo systemctl restart sshd

最后验证上传下载等。



## Author

安全研发C++小组



## 参考文献

1. **[^](https://blog.csdn.net/bbc2005/article/details/80034065)** Ubuntu安装SFTP服务。

