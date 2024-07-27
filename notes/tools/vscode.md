# vscode使用技巧

## c++ IntelliSense

ctrl + shift + p-->c/c++ -->c/c++: edit configurations(json)

配置includepath

```json
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**",
                "/usr/include/boost"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/clang-8",
            "cStandard": "c11",
            "cppStandard": "c++14",
            "intelliSenseMode": "clang-x64"
        }
    ],
    "version": 4
}
```

[参考]:

1.https://code.visualstudio.com/docs/cpp/configure-intellisense-crosscompilation

## 快捷键

- 快速搜索文件：ctrl+p
- 关闭当前文件：ctrl+w
- 打开命令板：ctrl+shift+p
- 打开终端： ctrl+`
- 搜索当前文件的函数：ctrl+shit+o, 也可以在大纲视图中查看。
- 替换标签页ctrl+tab， 可以将终端移动到标签页，配合使用。
- 预览markdown：ctrl+shift+v
- vs快速格式化代码：ctrl+k ctrl+f
- 切出一个新的编辑器 `ctrl+\`
- 编辑器之间切换ctrl+1 ctrl+2 ctrl+3

代码编辑相关

- 代码行缩进 ctrl+[  、ctrl+]
- 当前行上移/下移：alt+up/down
- 当前行下边插入一行 ctrl+enter
- 当前行上边插入一行 ctrl+shift+enter

光标相关

- 移动到行首： Home
- 移动到行尾：End
- 移动到文件结尾：ctrl+end
- 移动的文件开头：ctrl+home
- 转到定义处：F12
- 定义缩略图：alt+f12
- 多行同时输入：alt+shift+鼠标左键

代码重构

- 找到所有的引用：shift+f12
- 变量统一重命名：F2
- 查看diff：在explorer里选择文件右键set file to compare， 然后需要对比的文件上邮件选择 compare with filename。

显示相关

- 缩放
  - macOS：cmd+= cmd+-
  - windows/linux：ctrl+= ctrl+-
- 侧边栏显/隐：ctrl+B
- 显示资源管理器：ctrl+shift+E
- 显示debug: ctrl+shift+D
- 显示output: ctrl+shift+U

## Ctrl+P窗口

- 直接输入文件名，跳到文件。
- ? 列出当前可执行动作。
- ! 显示Errors或Warning， 也可以Ctrl+shift+m
- @ 跳转到symbol（搜索变量或则函数），也可以ctrl+shift+O直接进入。
- @ 根据分类跳转到symbol，查找属性或函数，也可以ctrl+shift+O后输入 `:` 进入。
- 根据名字查找symbol，也可以ctrl+T。

## 配合docker做c++开发。t

- docker pull ubuntu // ubuntu22.04
- 更新源
  ```shell
  sudo bash -c "cat << EOF > /etc/apt/sources.list && apt update 
  deb http://mirrors.aliyun.com/ubuntu/ jammy main restricted universe multiverse
  deb-src http://mirrors.aliyun.com/ubuntu/ jammy main restricted universe multiverse
  deb http://mirrors.aliyun.com/ubuntu/ jammy-security main restricted universe multiverse
  deb-src http://mirrors.aliyun.com/ubuntu/ jammy-security main restricted universe multiverse
  deb http://mirrors.aliyun.com/ubuntu/ jammy-updates main restricted universe multiverse
  deb-src http://mirrors.aliyun.com/ubuntu/ jammy-updates main restricted universe multiverse
  deb http://mirrors.aliyun.com/ubuntu/ jammy-proposed main restricted universe multiverse
  deb-src http://mirrors.aliyun.com/ubuntu/ jammy-proposed main restricted universe multiverse
  deb http://mirrors.aliyun.com/ubuntu/ jammy-backports main restricted universe multiverse
  deb-src http://mirrors.aliyun.com/ubuntu/ jammy-backports main restricted universe multiverse
  EOF"
  ```
- 安装开发包
  ```shell
  apt install git gcc g++ gdb cmake
  ```

## 终端字体间隔问题。

   配置setting（/home/peanut/.config/Code/User/settings.json）

```josn
   {
       "C_Cpp.clang_format_style": "Google",
       "terminal.integrated.defaultProfile.linux": "bash",
       "Lingma.LocalStoragePath": "/home/peanut/.lingma",
       "editor.tabSize": 2,
       "terminal.integrated.fastScrollSensitivity": 1,
       "terminal.integrated.rightClickBehavior": "copyPaste",
       "terminal.integrated.fontWeight": "normal",
       "terminal.integrated.fontFamily": "monospace",
       "terminal.integrated.fontWeightBold": "bold"
   }
```

   参考：https://blog.csdn.net/wotainanlejnhj/article/details/132394191

## 其他

1. mac 命令后启动

   ```txt
   Open the Command Palette (Cmd+Shift+P) and type 'shell command' to find the Shell Command: Install 'code' command in PATH command.
   ```
