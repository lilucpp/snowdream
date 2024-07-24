# Linux Command 

- [Linux Command](#linux-command)
  - [Shell](#shell)
  - [Linux Command 1](#linux-command-1)
  - [Linux Command 2](#linux-command-2)
  - [Linux Command 3](#linux-command-3)

## Shell

移动光标：
ctrl-a 光标移至行首
ctrl-e 光标移至行尾
ctrl-f 光标前移一个字符
ctrl-b 光标后移一个字符
alt-f 光标前移一个字
alt-b 光标后移一个字
ctrl-l 清空屏幕

修改文本：
ctrl-d 删除光标位置的字符

剪贴和粘贴
ctrl-k 剪切光标到行尾的文本
ctrl-u 剪切光标到行首的文本
ctrl-y 剪切环中的文本粘贴到光标位置

搜索历史命令
ctrl-p 移动到上一条历史条目
ctrl-n 移动到下一条历史条目
ctrl-r 反向增量搜索

历史命令展开
!! 重复最后一条执行的命令

## Linux Command 1

1. ulimit: The shell contains a built in command called "Ulimit" which allows you to display and set resource limits for users.The systems resources are defined in a file called "/etc/security/limits.conf". Ulimit can then be used to view these settings.
   To display all of your current settings you can issue the command: "ulimit -a"
   john@john-desktop:~$ ulimit -a
   core file size          (blocks, -c) 0
   data seg size           (kbytes, -d) unlimited
   scheduling priority             (-e) 0
   file size               (blocks, -f) unlimited
   pending signals                 (-i) 19868
   max locked memory       (kbytes, -l) 64
   max memory size         (kbytes, -m) unlimited
   open files                      (-n) 1024
   pipe size            (512 bytes, -p) 8
   POSIX message queues     (bytes, -q) 819200
   real-time priority              (-r) 0
   stack size              (kbytes, -s) 8192
   cpu time               (seconds, -t) unlimited
   max user processes              (-u) 19868
   virtual memory          (kbytes, -v) unlimited
   file locks                      (-x) unlimited
2. ps
   ps **axu** ps ax ps -ef ps -Al
   ps -Ao uid,ppid,pgid,vsz,pcpu,ruser,user,euser,rgroup,group,args
   print a process tree:
             ps -ejH
             **ps axjf**
   ps -axo pid,pgid,ppid,sid,tty,comm
3. top
   实时显示 
4. touch
   change file timestamps
   A FILE argument that does not exist is created empty
5. cpu架构
   [root@svr-db-test ~]# uname -m
   x86_64
6. 查看静态库和动态库是32位还是64位
   动态库：file ssss.so
   静态库：objdump -a ssss.a 
7. 查看linux版本
   cat /proc/version
   Linux version 3.10.52.x86_64 (root@localhost) (gcc version 4.9.3 20150626 (NeoKylin 4.9.3-3) (GCC) ) 
8. 头文件提供
    yum provides */selinux.h
9. 查看安装包内容
   rpm -aql  packagename
   dpkg -L packagename

10. 服务相关
    systemctl list-unit-files  查看服务列表
    sudo systemctl list-units
    systemctl enable servername 开启服务
    systemctl --help
    systemctl stop servername
    systemctl restart servername
    systemctl status servername
11. 开关机
    reboot
    poweroff
12. 加载驱动
    insmod  rmmod lsmod
13. 查看文件磁盘占用空间
    ncdu
14. htop  是 top 的一个加强版
15. 小技巧
    sudo !!
    以root的身份执行上一条命令 
    cd –
    回到上一次的目录 。
16. ls
    -t：按时间排  -r：reverse order while sorting
    排序： none  (-U) 名称,  size  (-S),  time(-t), version (-v), extension (-X)
    第三列是文件目录数（包括. ..）或 硬链接数(ln 可增加硬链接数)
    ls -lh --color=auto --time-style=long-iso -t -r
    total 596K
    drwxrwxr-x  2 ubuntu ubuntu 4.0K 2019-02-02 16:24 bin
    -rw-rw-r--  1 ubuntu ubuntu  522 2019-02-02 17:31 test.tpp
    -rw-rw-r--  1 ubuntu ubuntu  937 2019-02-03 08:49 commond
    drwxrwxr-x  8 ubuntu ubuntu 4.0K 2019-03-14 10:36 leetcode
    drwxrwxr-x  3 ubuntu ubuntu 4.0K 2019-05-10 01:21 drive
    drwxrwxr-x  2 ubuntu ubuntu 4.0K 2019-07-03 14:54 boost-demo
    drwxrwxr-x  2 ubuntu ubuntu 4.0K 2019-07-03 14:55 mysql
    drwxrwxr-x  2 ubuntu ubuntu 4.0K 2019-07-03 14:55 learnning_linux
    drwxrwxr-x 11 ubuntu ubuntu 4.0K 2019-07-05 15:38 cmake-examples
    drwxrwxr-x  8 ubuntu ubuntu 4.0K 2019-07-08 17:12 project
    -rw-rw-r--  1 ubuntu ubuntu 549K 2019-07-08 17:21 1.jpg
    -rwxrw-r--  1 ubuntu ubuntu   80 2019-07-09 08:55 job1.sh
17. less
    less [参数] 文件
    -m 显示类似more命令的百分比
    -N 显示每行的行号
    /字符串：向下搜索"字符串"的功能
    ?字符串：向上搜索"字符串"的功能
    n：重复前一个搜索（与 / 或 ? 有关）
    N：反向重复前一个搜索（与 / 或 ? 有关）
    b 向后翻一页
    d 向后翻半页
    h 显示帮助界面
    Q 退出less 命令
    类似：head->开头（默认10行）若干行, tail ->结尾（默认10行）若干行
    tail 命令可用于查看文件的内容，有一个常用的参数 -f 常用于查阅正在改变的日志文件。
    tail -f filename 会把 filename 文件里的最尾部的内容显示在屏幕上，并且不断刷新，只要 filename 更新就可以看到最新的文件内容。
18. ln
    ln [参数][源文件或目录][目标文件或目录]
    Linux文件系统中，有所谓的链接(link)，我们可以将其视为档案的别名，而链接又可分为两种 : 硬链接(hard link)与软链接(symbolic link)，硬链接的意思是一个档案可以有多个名称，而软链接的方式则是产生一个特殊的档案，该档案的内容是指向另一个档案的位置。硬链接是存在同一个文件系统中，而软链接却可以跨越不同的文件系统。
    不论是硬链接或软链接都不会将原本的档案复制一份，只会占用非常少量的磁碟空间。
    软链接：
    1.软链接，以路径的形式存在。类似于Windows操作系统中的快捷方式
    2.软链接可以 跨文件系统 ，硬链接不可以
    3.软链接可以对一个不存在的文件名进行链接
    4.软链接可以对目录进行链接
    硬链接：
    1.硬链接，以文件副本的形式存在。但不占用实际空间。
    2.不允许给目录创建硬链接
    3.硬链接只有在同一个文件系统中才能创建
    -b 删除，覆盖以前建立的链接
    -d 允许超级用户制作目录的硬链接
    -f 强制执行
    -i 交互模式，文件存在则提示用户是否覆盖
    -n 把符号链接视为一般目录
    -s 软链接(符号链接)
    -v 显示详细的处理过程
19. chown
    需要root权限
    将文件 file1.txt 的拥有者设为 runoob，群体的使用者 runoobgroup :
    chown runoob:runoobgroup file1.txt
    将目前目录下的所有文件与子目录的拥有者皆设为 runoob，群体的使用者 runoobgroup:
    chown -R runoob:runoobgroup *
20. chmod
    chmod [-cfvR] [--help] [--version] mode file...
    参数说明
    mode : 权限设定字串，格式如下 :
    [ugoa...][[+-=][rwxX]...][,...]
21. du df
    du -sk *   du -sh *
    -s或--summarize 仅显示总计
    df - report file system disk space usage
    df -h    df --total
22. mount  umount
    将 /dev/hda1 挂在 /mnt 之下。
    #mount /dev/hda1 /mnt
    将 /dev/hda1 用唯读模式挂在 /mnt 之下。
    #mount -o ro /dev/hda1 /mnt
23. netstat
    netstat -ap
    <ubuntu@ workspace>$sudo netstat -lntp
    Active Internet connections (only servers)
    Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name
    tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN      1351/sshd       
    tcp6       0      0 :::3306                 :::*                    LISTEN      1154/mysqld     
    tcp6       0      0 :::21                   :::*                    LISTEN      1149/vsftpd     
    tcp6       0      0 :::22                   :::*                    LISTEN      1351/sshd       

## Linux Command 2

24. grep/egrep  
    -n 显示行数  
    在当前目录中，查找含有main字符串的.cpp文件，并打印出该字符串的行。  
    ```
    grep -n main *.cpp
    ```
    -r 以递归的方式查找符合条件的文件。
    例如，查找指定目录/etc/acpi 及其子目录（如果存在子目录的话）下所有文件中包含字符串"update"的文件，并打印出该字符串所在行的内容.
    ```
    grep -rn update /etc/acpi 
    ```
    -v 反向查找。  
    前面各个例子是查找并打印出符合条件的行，通过"-v"参数可以打印出不符合条件行的内容。
    查找文件名中包含 test 的文件中不包含test 的行.
    ```
    grep -v test *test*
    ```

    -i 忽略大小写  
    -A<显示行数> 或 --after-context=<显示行数> : 除了显示符合范本样式的那一列之外，并显示该行之后的内容。  
    -B<显示行数> 或 --before-context=<显示行数> : 除了显示符合样式的那一行之外，并显示该行之前的内容。  
    -o 或 --only-matching : 只显示匹配PATTERN 部分。  

    **查找字符串department.txt在当前目录（递归）的所有的xml中的出现的行数。**
    ```
     find . -name "*.xml" | xargs grep -n  "department.xml"
    ```


25. find
    find   path   -option   [   -print ]   [ -exec   -ok   command ]   {} \;

    Linux find命令用来在指定目录下查找文件。任何位于参数之前的字符串都将被视为欲查找的目录名。如果使用该命令时，不设置任何参数，则find命令将在当前目录下查找子目录与文件。并且将查找到的子目录和文件全部进行显示。

    find 根据下列规则判断 path 和 expression，在命令列上第一个 - ( ) , ! 之前的部份为 path，之后的是 expression。如果 path 是空字串则使用目前路径，如果 expression 是空字串则使用 -print 为预设 expression。

    -amin n : 在过去 n 分钟内被读取过

    -atime n : 在过去n天内被读取过的文件

    -cmin n : 在过去 n 分钟内被修改过

    -ctime n : 在过去n天内被修改过的文件

    -empty : 空的文件

    -gid n or -group name : gid 是 n 或是 group 名称是 name

    -ipath p, -path p : 路径名称符合 p 的文件，ipath 会忽略大小写

    -name name, -iname name : 文件名称符合 name 的文件。iname 会忽略大小写

    -size n : 文件大小 是 n 单位，b 代表 512 位元组的区块，c 表示字元数，k 表示 kilo bytes，w 是二个位元组。

    -type c : 文件类型是 c 的文件。
    d: 目录 c: 字型装置文件 b: 区块装置文件
    f: 一般文件 l: 符号连结 s: socket

    你可以使用 ( ) 将运算式分隔，并使用下列运算。
    exp1 -and exp2
    ! expr
    -not expr
    exp1 -or exp2
    exp1, exp2

    将目前目录及其子目录下所有延伸档名是 c 的文件列出来。

    `find . -name "*.c"`

    将目前目录其其下子目录中所有一般文件列出

    `find . -type f`

    将目前目录及其子目录下所有最近 20 天内更新过的文件列出

    `find . -ctime -20`

    查找/var/log目录中更改时间在7日以前的普通文件，并在删除之前询问它们：

    `find /var/log -type f -mtime +7 -ok rm {} \;`

    为了查找系统中所有文件长度为0的普通文件，并列出它们的完整路径：
    ```shell
    find / -type f -size 0 -exec ls -l {} \;
    find / -size +10000c　　#查找出大于10000000字节的文件(c:字节，w:双字，k:KB，M:MB，G:GB)
    find / -size -1000k 　　#查找出小于1000KB的文件
    find /tmp -size +10000c -and -mtime +2 　　#在/tmp目录下查找大于10000字节并在最后2分钟内修改的文件
    find / -user fred -or -user george 　　#在/目录下查找用户是fred或者george的文件文件
    find /tmp ! -user panda　　#在/tmp目录中查找所有不属于panda用户的文件
    find ./test/ -type f ! -path "./test/usr/bin/*"  #排除bin的文件
    ```

26. 7z（注：不推荐使用，解压不出链接，推荐使用tar）
    安装方法： sudo apt-get install p7zip

    解压文件： 7z x manager.7z -r -o /home/xx

    解释如下：x 代表解压缩文件，并且是按原始目录解压（还有个参数 e 也是解压缩文件，但其会将所有文件都解压到根下，而不是自己原有的文件夹下）manager.7z 是压缩文件，这里大家要换成自己的。如果不在当前目录下要带上完整的目录

    -r 表示递归所有的子文件夹

    -o 是指定解压到的目录，这里大家要注意-o后是没有空格的直接接目录

    压缩文件: 7z a -t7z -r manager.7z /home/manager/*

    解释如下：
    a 代表添加文件／文件夹到压缩包


    -t 是指定压缩类型 一般我们定为7z
    
    -r 表示递归所有的子文件夹，manager.7z 是压缩好后的压缩包名，/home/manager/* 是要压缩的目录，＊是表示该目录下所有的文件。


27. tar
    .tar.gz 和 .tgz

    解压：tar -zxvf FileName.tar.gz

    压缩：tar -zcvf FileName.tar.gz DirName

    ```txt
    Tar Usage and Options
    c – create a archive file.
    x – extract a archive file.
    v – show the progress of archive file.
    f – filename of archive file.
    t – viewing content of archive file.
    j – filter archive through bzip2.
    z – filter archive through gzip.
    r – append or update files or directories to existing archive file.
    W – Verify a archive file.
    wildcards – Specify patterns in unix tar command.
    a)Create tar Archive File：tar -cvf tecmint-14-09-12.tar /home/tecmint/
    b)Create tar.gz Archive File(Note : tar.gz and tgz both are similar):
        tar cvzf MyImages-14-09-12.tar.gz /home/MyImages 
        tar cvzf MyImages-14-09-12.tgz /home/MyImages
    c)Create tar.bz2 Archive File: 
        tar cvfj Phpfiles-org.tar.bz2 /home/php
        tar cvfj Phpfiles-org.tar.tbz /home/php
        tar cvfj Phpfiles-org.tar.tb2 /home/php
    d)Untar tar Archive File:
        Untar files in Current Directory ##
        tar -xvf public_html-14-09-12.tar
        Untar files in specified Directory ##
        tar -xvf public_html-14-09-12.tar -C /home/public_html/videos/
    e)Uncompress tar.gz Archive File:
        tar -xvf thumbnails-14-09-12.tar.gz
    f)Uncompress tar.bz2 Archive File:
        tar -xvf videos-14-09-12.tar.bz2
    g)List Content of tar Archive File/List Content tar.gz Archive File/List Content tar.bz2 Archive File
        tar -tvf uploadprogress.tar/tar.gz/tar.bz2
    h)Untar Single file from tar File:
        tar -xvf cleanfiles.sh.tar cleanfiles.sh
        tar --extract --file=cleanfiles.sh.tar cleanfiles.sh
    i)Untar Single file from tar.gz File:
        tar -zxvf tecmintbackup.tar.gz tecmintbackup.xml
        tar --extract --file=tecmintbackup.tar.gz tecmintbackup.xml
    j)Untar Single file from tar.bz2 File:
        tar -jxvf Phpfiles-org.tar.bz2 home/php/index.php
        tar --extract --file=Phpfiles-org.tar.bz2 /home/php/index.php
    k) Untar Multiple files from tar, tar.gz and tar.bz2 File:
        tar -xvf tecmint-14-09-12.tar "file 1" "file 2" 
        tar -zxvf MyImages-14-09-12.tar.gz "file 1" "file 2"     
        tar -jxvf Phpfiles-org.tar.bz2 "file 1" "file 2"
    l)Extract Group of Files using Wildcard
        tar -xvf Phpfiles-org.tar --wildcards '*.php'
        tar -zxvf Phpfiles-org.tar.gz --wildcards '*.php'
        tar -jxvf Phpfiles-org.tar.bz2 --wildcards '*.php'
    m)Add Files or Directories to tar Archive File
        tar -rvf tecmint-14-09-12.tar xyz.txt
        tar -rvf tecmint-14-09-12.tar php
    n)Add Files or Directories to tar.gz and tar.bz2 files:
        The tar command don’t have a option to add files or directories to an existing compressed tar.gz and tar.bz2 archive file. 
    o)How To Verify tar, tar.gz and tar.bz2 Archive File:
    To verfify any tar or compressed archived file we use option as W (verify). To do, just use the following examples of command. (Note : You cannot do verification on a compressed ( *.tar.gz, *.tar.bz2 ) archive file).
        tar tvfW tecmint-14-09-12.tar
    ```
## Linux Command 3

28. pstree
    pstree -p 可以帮你显示进程树

29. nohup
    You can also use nohup command, which also enables a process to continue running in the background when a user exits a shell.
    后台方式执行作业时，需要在作业后面加一个&

    要把前台作业放到后台，并暂停执行，则需要在前台ID执行的时候按 Ctrl+z，

    可以使用jobs -l命令查看后台作业，方括号内的表示作业编号，第三列表示作业的PID

    使用fg命令将后台作业调到前台，需要跟上%作业号: fg %jobnumber，下面是把第二个作业调到前台并继续执行

    后台作业在后台继续执行,使用bg命令：bg %jobnumber
    
    若想要关闭终端后使作业继续保持运行，可以在作业前加上nohup命令：
    nohup command &
    使用 nohup 或  disown 如果你要让某个进程运行在后台
    
30. 选者内容，按鼠标中间会把文本粘贴到光标所在位置。

31. 显示图片 ，xdg-open 1.png

32. scp

    ```shell
    scp local_path username@host_address:remote_path　　//复制本地文件至远端主机
    scp username@host_address:remote_path local_path　　//复制远端主机文件至本地
    ```
33. 把账户添加到sudo
    usermod -aG sudo newuser


  参考：http://www.ruanyifeng.com/blog/2018/09/vimrc.html

34. 合并两个文件夹[^1]

    ```txt
    rsync -a /path/to/source/ /path/to/destination
    ```

    [^1]: https://www.ruanyifeng.com/blog/2020/08/rsync.html

