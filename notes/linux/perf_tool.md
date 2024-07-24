# 性能监控
## cpu监控命令
### top
```
-u<用户名>：指定用户名；
-p<进程号>：指定进程；
-H：所有线程占用资源情况。
-c：显示完整的治命令；
```

实例
```
top - 09:44:56 up 16 days, 21:23,  1 user,  load average: 9.59, 4.75, 1.92
Tasks: 145 total,   2 running, 143 sleeping,   0 stopped,   0 zombie
Cpu(s): 99.8%us,  0.1%sy,  0.0%ni,  0.2%id,  0.0%wa,  0.0%hi,  0.0%si,  0.0%st
Mem:   4147888k total,  2493092k used,  1654796k free,   158188k buffers
Swap:  5144568k total,       56k used,  5144512k free,  2013180k cached
```
解释：
```
top - 09:44:56[当前系统时间],
16 days[系统已经运行了16天],
1 user[个用户当前登录],
load average: 9.59, 4.75, 1.92[系统负载，即任务队列的平均长度]
Tasks: 145 total[总进程数],
2 running[正在运行的进程数],
143 sleeping[睡眠的进程数],
0 stopped[停止的进程数],
0 zombie[冻结进程数],
Cpu(s): 99.8%us[用户空间占用CPU百分比],
0.1%sy[内核空间占用CPU百分比],
0.0%ni[用户进程空间内改变过优先级的进程占用CPU百分比],
0.2%id[空闲CPU百分比], 0.0%wa[等待输入输出的CPU时间百分比],
0.0%hi[],
0.0%st[],
Mem: 4147888k total[物理内存总量],
2493092k used[使用的物理内存总量],
1654796k free[空闲内存总量],
158188k buffers[用作内核缓存的内存量]
Swap:  5144568k total[交换区总量],
56k used[使用的交换区总量],
5144512k free[空闲交换区总量],
2013180k cached[缓冲的交换区总量],
```
#### 其他
- 按“1”，可监控每个逻辑cpu得状况。
- 按“b” 打开关闭加亮效果
- 按“x”打开列加亮效果
- 按“shift+>” 或 “shift+<”可以向右或向左改变列。

### nmon
使用nmon可以监控的点有：cpu占用率、内存使用情况、 磁盘I/O速度，传输和读写比率、文件系统的使用率、 网络I/O速度、传输和读写比率、错误统计率与传输包的大小、消耗资源最多的进程、计算机详细信息和资源、页面空间和页面I/O速度等等。

```
c : 显示cpu利用率数据
m：显示内存数据
n：显示网络信息
d：显示磁盘信息
t：系统进程信息
h：查看帮助信息
q：退出Nmon界面
```
## memory

### buffer vs cache
- Buffer is for storing file metadata (permissions, location, etc). Every memory page is kept track of here.

- Cache is for storing actual file contents.

### swap vs cache
它们是完全不同的东西，甚至在很多方面是相反的。我将尝试简单而简短地解释。请注意，这个答案只是一个简化，实际情况要复杂得多。单击维基百科链接以获得更好的解释。

- [Disk Cache memory](https://en.wikipedia.org/wiki/Page_cache): This are chunks of the physical memory, the RAM, used to store files. That way when a program needs to read the file, it's fetched from memory instead of the hard disk. This is done because memory is way faster.
- [Swap](https://en.wikipedia.org/wiki/Memory_paging): This is a place on the hard disk (usually a dedicated partition) that is used to store programs or data that can't fit in memory, like when a program grows more than the available memory. SWAP is way slower than RAM, so when you hit swap the computer gets slower, but at least the program can work. In linux swap is also used to hibernate, or to move low used program out of memory to allow more space to the disk cache.

翻译：
- 磁盘高速缓存：这是物理内存（RAM）的块，用于存储文件。这样，当程序需要读取文件时，将从内存而不是硬盘中获取文件。这样做是因为内存速度更快。
- 交换：这是硬盘上的一个位置（通常是专用分区），用于存储无法容纳在内存中的程序或数据，例如当程序增长超过可用内存时。SWAP 比 RAM 慢得多，因此当您点击交换时，计算机会变慢，但至少程序可以运行。在 Linux 中，交换还用于休眠，或者将低使用率的程序移出内存，以便为磁盘缓存提供更多空间。

### free
选项
```
-s <间隔秒数> # 持续观察内存使用状况；
```

```shell
peanut@peanut-vm:~$ free -hw
              total        used        free      shared     buffers       cache   available
Mem:           7.7G        2.4G        1.2G         45M        443M        3.7G        5.0G
Swap:          2.0G        166M        1.8G
peanut@peanut-vm:~$ 
```

### nmon

## io性能

### iostat
需要安装：`apt install sysstat`  

iostat命令 被用于监视系统输入输出设备和CPU的使用情况。它的特点是汇报磁盘活动统计情况，同时也会汇报出CPU使用情况。同vmstat一样，iostat也有一个弱点，就是它不能对某个进程进行深入分析，仅对系统的整体情况进行分析。  

iostat [选项] [时间] [次数]  
#### 选项
```
-c：仅显示CPU使用情况；
-d：仅显示设备利用率；
-k：显示状态以千字节每秒为单位，而不使用块每秒；
-m：显示状态以兆字节每秒为单位；
-p：仅显示块设备和所有被使用的其他分区的状态；
-t：显示每个报告产生时的时间；
-V：显示版号并退出；
-x：显示扩展状态。
```
#### 参数
间隔时间：每次报告的间隔时间（秒）；  
次数：显示报告的次数。

## 网络性能
### netstat 
todo
### tcpdump
todo

## 参考
- [1] [What is the difference between buffer and cache memory in Linux?](https://stackoverflow.com/a/16093403)
- [2] [Is there a difference between SWAP and Cache memory?](https://askubuntu.com/a/89222)

