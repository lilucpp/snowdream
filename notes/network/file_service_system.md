# 文件服务系统



## [cifs文件系统](https://cifs.com/)

[cifs](https://1drv.ms/b/s!AiOHW4QDJaFOgjhi4nbgszCoudQy?e=jLkivs)

[cifs文件系统](https://blog.csdn.net/yuanbinquan/article/details/51734705)

**Common Internet File System** 通用Internet文件系统

Common Internet File System (CIFS) is a network filesystem protocol used for providing shared access to files and printers between machines on the network. A CIFS client application can read, write, edit and even remove files on the remote server. CIFS client can communicate with any server that is set up to receive a CIFS client request.

With the release of Windows 95 in the early 1990’s, Microsoft has made considerable modifications to the most commonly used SMB version. Microsoft then merged the updated version of the SMB protocol (and rebranded it as CIFS) with the LAN Manager product bringing both client and server support. 

1. windows端共享文件夹，设置权限，如,目录 E:\cifs, 用户： Everyone , 权限：所有权限

2. 其他windows电脑可以输入**\\\\ip\\cifs** 进行访问

3. linux端：

   This is relevant given that you are trying to mount NFS.

   ```bash
   sudo apt install nfs-common
   ```

   On the other hand, if you are trying to mount CIFS.

   ```bash
   sudo apt install cifs-utils
   ```

   then, input command:

   ```bash
   sudo mount -t cifs -o user=Everyone,password='' //192.168.3.159/cifs /mnt/cifs
   ```

## [SBM](https://en.wikipedia.org/wiki/Server_Message_Block)

[SBM wiki_bak](https://1drv.ms/b/s!AiOHW4QDJaFOgjqlf-kbYZHziLK7?e=otbnuV)

In computer networking, Server Message Block (SMB), one version of which was also known as Common Internet File System (CIFS /sɪfs/), is a network communication protocol for providing shared access to files, printers, and serial ports between nodes on a network.

