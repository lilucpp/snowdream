# vim使用技巧

1. 粘贴代码格式混乱
   
   在/etc/vim/vimrc中添加set pastetoggle=<F9>，在黏贴代码之前，进入insert模式，按F9，就可以关闭自动缩进。

2. 窗口分割
   
   ```text
   :Explore    " explore with a capital E
       :E          " or simply, colon-big-e
       :Sex        " sexplore with a capital S
       :Vex        " vexplore with a capital V
       With a file explorer window open, you can create a new file in the currently selected         directory by pressing the key   "%"
       You can create a new directory from the file explorer by pressing the key: "d"
       rename existing files and directories.  you can do so with the ‘shift-R’ 
       You can also delete files from the file explorer, by pressing the key:D       " shift-D
   ```

3. 窗口切换
   :ctrl+w+j/k，通过j/k可以上下切换，或者:ctrl+w加上下左右键，还可以通过快速双击ctrl+w依次切换窗口。

4. 查找
   在normal模式下按下/即可进入查找模式，输入要查找的字符串并按下回车。 Vim会跳转到第一个匹配。按下n查找下一个，按下N查找上一个。
   Vim查找支持正则表达式，例如/vim$匹配行尾的"vim"。 需要查找特殊字符需要转义，例如/vim\$匹配"vim$"。
   在查找模式中加入\c表示大小写不敏感查找，\C表示大小写敏感查找

5. 替换
   :{作用范围}s/{目标}/{替换}/{替换标志}
   例如:%s/foo/bar/g会在全局范围(%)查找foo并替换为bar，所有出现都会被替换（g）
   作用范围分为当前行、全文、选区等等
   当前行：:s/foo/bar/g
   全文：:%s/foo/bar/g
   选区，在Visual模式下选择区域后输入:，Vim即可自动补全为 :'<,'>。:'<,'>s/foo/bar/g
   2-11行：:5,12s/foo/bar/g
   替换标志
   空替换标志表示只替换从光标位置开始，目标的第一次出现：
   g即是替换标志之一，表示全局global替换
   c表示需要确认 :%s/foo/bar/gc

6. 光标移到到行首行尾
   1$"表示当前行的行尾  1^行首

7. 选中多行
   ctrl +v ，下移动
   ctrl+shift+i

8. 撤销
   esc --> u

9. 16进制查看文件
   :%!xxd ——将当前文本转换为16进制格式。
   查看内容是对应的。你可以后面看到对应的字符内容
   :%!xxd -r ——将当前文件转换回文本格式。

10. config
    
    在文件/etc/vim/vimrc中添加如下设置
    简单的设置。
    
    ```txt
    To define this on a permanent basis for the current user, create (or edit) the .vimrc file:
    
    $ vim ~/.vimrc
    Then, paste the configuration below into the file. Once vim is restarted, the tab settings will apply.
    
    set tabstop=2       " The width of a TAB is set to 4.
                        " Still it is a \t. It is just that
                        " Vim will interpret it to be having
                        " a width of 4.
    
    set shiftwidth=2    " Indents will have a width of 4
    
    set softtabstop=2   " Sets the number of columns for a TAB
    
    set expandtab       " Expand TABs to spaces
    ```
    
    更详细的设置


set nocompatible " 关闭 vi 兼容模式
syntax on " 自动语法高亮
set number " 显示行号
set softtabstop=2 " 使得按退格键时可以一次删掉 2 个空格
set tabstop=2 " 设定 tab 长度为 2

set shiftwidth=2
filetype plugin indent on " 开启插件
set incsearch " 输入搜索内容时就显示搜索结果
set hlsearch " 搜索时高亮显示被找到的文本
set smartindent " 开启新行时使用智能自动缩进
