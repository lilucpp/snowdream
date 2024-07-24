# clang-format

## 简介

配置： 下载llvm，把bin目录添加到环境变量，即可在全局访问clang-format。

可以使用如下命令格式化代码：

```shell
clang-format -style="{BasedOnStyle: Google, IndentWidth: 4, ColumnLimit: 100}" updateImpl.cpp -i
```

导出配置

```shell
clang-format -style=llvm -dump-config > .clang-format
```

## 资料：

1. [官网](https://clang.llvm.org/docs/ClangFormat.html)

2. [Clang-Format格式化选项介绍](https://blog.csdn.net/softimite_zifeng/article/details/78357898)

## 格式化python脚本

```python
# -*- coding: utf-8 -*- 
import os 
import sys
import win32api

def isSkipFile(filename):
    filename = filename.lower()
    skippath = ['skip.cpp','skip2.cpp']
    for val in skippath:
        if filename.endswith(val):
            return True 

    suffix = ['.cpp', '.cc', '.c', '.cxx', '.h', '.hpp', '.hxx']
    for val in suffix:
        if filename.endswith(val):
            return False 
    
    return True

def isSkipPath(dir):
    dir = dir.lower()
    skipdir = ['skip','demo','debug']
    for val in skipdir:
        if dir.endswith(val):
            return True 
    
    return False

def dir(rootDir): 
    if(isSkipPath(rootDir)):
        return ;

    for filename in os.listdir(rootDir):
        path = os.path.join(rootDir, filename)
        if os.path.isdir(path): 
            dir(path)
            
        if(isSkipFile(path)):
            continue
        
        #param = r'''-style="{BasedOnStyle: Google, IndentWidth: 4, ColumnLimit: 100, SpacesInParentheses: true, SpacesInAngles: true}" ''' + filename + " -i"
        param = "-style=file \"" + filename + "\" -i"
        print(param)
        win32api.ShellExecute(0, 'open', r'clang-format.exe', param, rootDir, 0) 

def main(argv):
    dir(argv[1])

if __name__=="__main__":
    if len(sys.argv) != 2:
        print("Usage: python clang-format.py path")
    else:
        main(sys.argv)
```

.clang-format

```
BasedOnStyle: Google
DerivePointerAlignment: false
PointerAlignment: Right
ColumnLimit: 120

# Default for clang-8, changed in later clangs. Set explicitly for forwards
# compatibility for students with modern clangs
IncludeBlocks: Preserve
```

## skip sections of c++ code

Use `// clang-format off` and `// clang-format on` to make it skip code sections.

```c++
// clang-format off
// Don't touch this!
connect( m_Job, SIGNAL( error( const QString&, const QString& ) ),  this, SLOT( onError( const QString&, const QString& ) ) );
connect( m_Job, SIGNAL( message( const QString& ) ),                this, SLOT( onMessage( const QString& ) ) );
connect( m_Job, SIGNAL( progress( int, int ) ),                     this, SLOT( onProgress( int, int ) ) );
// clang-format on
// Carry on formatting
```