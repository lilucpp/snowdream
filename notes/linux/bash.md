1. echo

   输出的文本末尾会有一个回车符。`-n`参数可以取消末尾的回车符，使得下一个提示符紧跟在输出内容的后面。  

   `-e`参数会解释引号（双引号和单引号）里面的特殊字符（比如换行符`\n`）。如果不使用`-e`参数，即默认情况下，引号会让特殊字符变成普通字符。

2. 同行命令的执行关系

   分号（`;`）：是命令的结束符，使得一行可以放置多个命令，上一个命令执行结束后，再执行第二个命令。  

   && ||：

   ```
   Command1 && Command2
   ```

   上面命令的意思是，如果`Command1`命令运行成功，则继续运行`Command2`命令。

   ```
   Command1 || Command2
   ```

   上面命令的意思是，如果`Command1`命令运行失败，则继续运行`Command2`命令。

3. type

   `-t`参数，可以返回一个命令的类型：别名（alias），关键词（keyword），函数（function），内置命令（builtin）和文件（file）。

4. Bash 的模式扩展

   [globbing](./doc_bash/globbing.md)
   
5. export

   export val="" 的变量可以传递给子shell。

6. 脚本参数

   [param](./doc_bash/param.md)

7. 字符串操作

   - 获取字符串长度

   ```txt
   $peanut="xhs"
   $echo ${peanut}
   $xhs
   $echo ${#peanut}
   $3
   ```

   - 子字符串  ${var:offset:length}  

   如果offset是负值，表示字符串从末尾算起。负数前面必须有个空格。  

   - 搜索和替换

   ```
   $ myPath=/home/cam/book/long.file.name
   
   $ echo ${myPath#/*/}
   cam/book/long.file.name
   
   $ echo ${myPath##/*/}
   long.file.name
   ```

   如果匹配不成功，则返回原始字符串。

   替换：

   ```
   $ path=/home/cam/foo/foo.name
   
   $ echo ${path/foo/bar}
   /home/cam/bar/foo.name
   
   $ echo ${path//foo/bar}
   /home/cam/bar/bar.name
   ```

   ```
   # 转为大写
   ${varname^^}
   
   # 转为小写
   ${varname,,}
   ```

8. 快捷键

   ```txt
   ctrl+a: 行首
   ctrl+e:行尾
   ctrl+k: 剪切光标-行尾
   ctrl+u:剪切光标-行首
   ctrl+y:粘贴文本
   ctrl+shift+c: 复制选择的文本
   ctrl+shift+v: 粘贴复制的文本
   ctrl+p:显示上一条历史命令
   ctrl+n:显示下一条历史命令
   ctrl+o: 执行历史中的当前条目，并自动显示下一条命令。
   alt+.: 插入上一条命令的最后一个词。
   ```

9. 目录堆栈

   ```txt
   cd -: 回到前一次目录所在。
   pushd,popd: 目录堆栈。
   dirs: 可以显示目录堆栈的内容。
   ```

10. 常用脚本

   建议在主目录新建一个～/bin子目录，专门放可执行脚本，然后把～/bin加入$PATH.    

   建议`export PATH=$PATH:~/bin`加入到~/.bashrc的最后一行。然后执行source ~/.bashrc。  

11. read

    ```txt
    read [-p] [var ...]
    ```

    默认回车键表示结束。如果没有提供var，会存到REPLY中。

    - -t:  设置超时时间。
    - -e: 使用readline库提供的快捷键，比如自动补全。
    - -d delimiter: 定义输入结束府。
    - -s: 不显示在屏幕上。

12. 条件判断

    [条件判断](./doc_bash/if.md)

13. 当前文件夹

   ```shell
   if [ -L $0 ]; then
    BASE_DIR=`dirname $(readlink $0)`
   else
      BASE_DIR=`dirname $0`
   fi
   # 当前脚本目录
   basepath=$(cd ${BASE_DIR}; pwd)
   echo ${basepath}
   ```

    

