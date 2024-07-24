# process infomation

## libprocps-dev

## 可以参考go实现

github.com/shirou/gopsutil/process

## proc 字段含义
man proc
```txt
/proc/[pid]/cmdline
              This read-only file holds the complete command line for the process, unless the process is a zombie.  In the latter case, there is nothing in this file: that is, a read on this file  will  return  0  characters.
              The command-line arguments appear in this file as a set of strings separated by null bytes ('\0'), with a further null byte after the last string.

```