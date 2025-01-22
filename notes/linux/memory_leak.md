
# 进程性能分析

## 内存泄漏检测
- Valgrind
- 检测内存泄漏可以用bcc memleak进行检查和定位。
  ```txt
  memleak-bpfcc -a -p pid  5  
  -p PID 跟踪进程 ID
  -t 打印所有分配和释放请求及结果的踪迹。
  -a 打印未释放的分配列表（及其大小）以及它们的调用堆栈
  5 表示间隔多少秒
  ```


## 性能分析
perf 命令（performance 的缩写），它是 Linux 系统原生提供的性能分析工具，会返回 CPU 正在执行的函数名以及调用栈（stack）。  

例如： `sudo perf record -F 99 -p 13204 -g -- sleep 30`  
上面的代码中，perf record表示记录，-F 99表示每秒99次，-p 13204是进程号，即对哪个进程进行分析，-g表示记录调用栈，sleep 30则是持续30秒。

常用：一直记录，直到按下Ctrl+C，然后再用perf report查看分析结果。
```txt
perf  record  -g -p pid

# enter键可以查看函数调用栈
perf report
```
