
1、Valgrind
2、检测内存泄漏可以用bcc memleak进行检查和定位：
memleak-bpfcc -a -p pid  5
-p PID 跟踪进程 ID
-t 打印所有分配和释放请求及结果的踪迹。
-a 打印未释放的分配列表（及其大小）以及它们的调用堆栈
5 表示间隔多少秒