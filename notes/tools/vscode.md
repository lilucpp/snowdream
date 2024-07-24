# vscode使用技巧

1. c++ IntelliSense

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

2. 快捷键
   - 多行同时输入：alt+shift+鼠标左键
   - 快速搜索文件：ctrl+p
   - 搜索当前文件的函数：ctrl+shit+o, 也可以在大纲视图中查看。
   - 替换标签页ctrl+tab， 可以将终端移动到标签页，配合使用。

3. vs快速格式化代码
   	ctrl+k ctrl+f
    
4. 预览markdown
    ctrl+shift+v

5. 缩放
   macOS：cmd+= cmd+-
   windows/linux：ctrl+= ctrl+-

6. mac 命令后启动

    ```txt
    Open the Command Palette (Cmd+Shift+P) and type 'shell command' to find the Shell Command: Install 'code' command in PATH command.
    ```
7. 配合docker做c++开发。
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
8. 终端字体间隔问题。
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