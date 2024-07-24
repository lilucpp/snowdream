# When
## 查找文件

1. find 或 locate
   ```shell
   peanut@peanut:~$ locate utsname.h
   /usr/include/linux/utsname.h
   /usr/include/x86_64-linux-gnu/bits/utsname.h
   /usr/include/x86_64-linux-gnu/sys/utsname.h
   /usr/src/linux-headers-4.15.0-55/include/linux/utsname.h
   /usr/src/linux-headers-4.15.0-55/include/uapi/linux/utsname.h
   /usr/src/linux-headers-4.15.0-66/include/linux/utsname.h
   /usr/src/linux-headers-4.15.0-66/include/uapi/linux/utsname.h
   peanut@peanut:~$ find /usr/include -name utsname.h
   /usr/include/x86_64-linux-gnu/bits/utsname.h
   /usr/include/x86_64-linux-gnu/sys/utsname.h
   /usr/include/linux/utsname.h
   peanut@peanut:~$ 
   ```
   
2. 过滤和结果作为下一个命令额参数
   - 反引号``
   - $()
   - xargs -o
   - find命令-exec 
   ```shell
   peanut@peanut:~$ ls | grep date | xargs -o cat
   Thu Dec 10 11:13:23 CST 2020
   peanut@peanut:~$ echo `date`
   Thu Dec 10 11:20:22 CST 2020
   peanut@peanut:~$ find /usr/include -name utsname.h -exec ls -l {} \;
   -rw-r--r-- 1 root root 1212 Sep  2 17:18 /usr/include/x86_64-linux-gnu/bits/utsname.h
   -rw-r--r-- 1 root root 2480 Sep  2 17:18 /usr/include/x86_64-linux-gnu/sys/utsname.h
   -rw-r--r-- 1 root root 669 Oct  1  2019 /usr/include/linux/utsname.h
   ```

3. 上传下载文件
       sz、rz
   运行
4. shell 获取进程退出码
   ```shell
   echo $?
   ```
   查看error code含义
   ```shell
   # 安装errno命令
   sudo apt install moreutils
   # 列出所有errno
   errno -l
   errno -l | more
   # 查看错误玛1的含义
   errno 1
   # 搜索所有错误描述中含有per的错误玛
   errno --search per
   ```
5. 设置shell提示符
   
   使用以下命令可以显示当前的提示设置。
   
   ```shell
   echo $PS1  
   ```
   
   使用以下命令可以设置简短的提示。
   
   ```shell
   export PS1="\u > "  
   ```
   
6. 包管理
   
      ```shell
      dpkg -c xxx.deb // 安装前根据deb文件查看
      dpkg -L debname // 安装后根据包名查看
      dpkg -i xxx.deb // 安装deb包
      dpkg -r debname // 移除deb包
      dpkg -S filepath // 查看某个文件属于哪个deb包
      dpkg -X xxx.deb dirname // 释放安装内容到dirname目录中
      dpkg -e xxx.deb // 释放控制信息到当前目录下的DEBIAN子目录中
      dpkg -b packagedir  amd64-fd.deb  // 打包
      apt list --installed // 查看已安装包
      ```
   
      制作安装包资料：https://blog.csdn.net/gatieme/article/details/52829907

7. 服务

      lib/systemd/system或/etc/systemd/system/multi-user.target.wants

      ```txt
      [Unit]
      Description=xxxcbjtx
      #Before=lightdm.service
      After=local-fs.target network.target
      RefuseManualStop=yes
      ##DefaultDependencies=no
      #Conflicts=shutdown.target
      
      
      [Service]
      WorkingDirectory=/opt/XXXC/
      Type=forkingXXX
      ExecStart=/opt/XXXC/xx -d
      #RemainAfterExit=true
      
      
      [Install]
      WantedBy=multi-user.target
      
      ```
      
8. 查看当前的桌面系统

   ```
   sudo apt install screenfetch
   screenfetch
   ```
9. diff 
    -y --side-by-side  Output in two columns.

10. mount

   挂载文件夹下的文件到另外一个文件夹。

   ```shell
   sudo mount -B  ./from   ./to
   ```

   卸载：重启或者umount

   ```shell
   sudo umount  ./to
   ```

   也可以挂载一个文件到另一个地方

```shell
sudo mount -B ./test1/1.txt ./test2/1.txt
```

11.  产生core dump

    ```shell
    ulimit  -c unlimited
    sudo bash -c 'echo "/home/lu/temp/core-%e-%p-%t" > /proc/sys/kernel/core_pattern'
    gdb ./a.out core-a.out-84069-1626858768
    // bt 可以打印堆栈
    ```

