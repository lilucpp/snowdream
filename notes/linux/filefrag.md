1、查看文件中在物理磁盘的偏移

```shell
lu$ cat 1.txt 
abc1111
lu$ 
lu$ filefrag -e ./1.txt 
Filesystem type is: ef53
File size of ./1.txt is 8 (1 block of 4096 bytes)
 ext:     logical_offset:        physical_offset: length:   expected: flags:
   0:        0..       0:    5641277..   5641277:      1:             last,eof
./1.txt: 1 extent found
lu$ 

```

2、查看文件系统在磁盘的位置。

```shell
lu$ sudo fdisk -l
Disk /dev/nvme0n1：238.47 GiB，256060514304 字节，500118192 个扇区
Disk model: KXG50ZNV256G TOSHIBA                    
单元：扇区 / 1 * 512 = 512 字节
扇区大小(逻辑/物理)：512 字节 / 512 字节
I/O 大小(最小/最佳)：512 字节 / 512 字节
磁盘标签类型：gpt
磁盘标识符：066C336C-AC23-45A7-B422-DAAC8EA0A8CB

设备              起点      末尾      扇区  大小 类型
/dev/nvme0n1p1    2048   1050623   1048576  512M EFI 系统
/dev/nvme0n1p2 1050624 500117503 499066880  238G Linux 文件系统
lu$ 

```

3、使用wxHexEditor打开/dev/nvme0n1p2   

- 设备--打开设备  

<img src="./img/filefrag-1.png" alt="image-20230314122021898" style="zoom:80%;" />

- CTRL+G 跳转

  5641277*4096=23106670592

![image-20230314122151260](./img/filefrag-2.png)

跳转后即可看到1.txt的内容。

4、可以结合dd hexdujmp查看。
```txt
bs=512：块大小设置为 512 字节。
skip=$((23106670592 / 512))：计算需要跳过的块数量，23106670592 字节除以块大小（512 字节）。
count=1：只读取 1 个块（512 字节）。
2>/dev/null：隐藏 dd 的标准错误输出。

dd if=/dev/nve0n1p5 bs=512 skip=$((23106670592 / 512)) count=1 2>/dev/null | hexdump -C
```
```txt
小常识：
为什么 dd 更快？
hexdump -C -s <offset> -n <size> /dev/nve0n1p5 
dd 跳过数据块：hexdump 会逐字节处理数据，而 dd 的 skip 选项可以直接跳过大块数据。
块级访问：设备文件通常以块为单位进行访问，因此使用合适的块大小（如 512 字节或 4 KB）可以显著提高效率。
```

5、清除文件  
ssd上可以使用O_DRECT+O_SYNC进行清除。不过这样清除后，使用dd hexdujmp查看会发现文件内容没有被清除，这是因为ssd的缓存机制。有两个方法可以解决这个问题：
- 使用nvme flush /dev/nvme0n1 刷新缓存。
- 重启电脑后，文件内容会被清除。

建议使用第二种方案，第一种可能对SSD使用寿命有影响。

文件名的清除可以考虑srm的方案，假如old_name的basename长度是len。 old_name -> new_name(len, 0) -> new_name1(len-1, 0) -> ... -> new_nameN(0)。使用递减的方式清除文件名。最后再删除。 