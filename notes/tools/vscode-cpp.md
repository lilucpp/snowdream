# 使用vscode配置c++开发环境

## 插件

- c/c++
- cpplint
- Remote-SSH

## 语义补全

目前官方的c/c++插件也可以开启语义补全，需要设置

"C_Cpp.intelliSenseEngine": "Default",

## 格式化

在 VS Code 中安装了 C/C++ 插件后会自动带上格式化工具 clang-format。windows上`alt+shift+f`可对文件进行格式化。

## 代码风格检查

使用cpplint检查代码风格。

需要安装python3，然后使用`pip install cpplint`进行安装。

然后进行插件设置：

cpplint.cpplintPatp、cpplint.lineLength、cpplint.excludes等。

## 版权声明

可使用vscode自身的snipper。`文件-首选项-配置用户代码片段`。  

```json
{
	"add Copyright notice": {
	   "prefix": [ "header", "stub", "copyright"],
	 "body": [
		"// Base - A C++ base library",
		"// Copyright (c) 2023, Lu Li.  All rights reserved.",
		"// https://github.com/lilucpp/base",
		"//",
		"// Redistribution and use in source and binary forms, with or without",
		"// modification, are permitted provided that the following conditions",
		"// are met:",
		"//",
		"//   * Redistributions of source code must retain the above copyright",
		"// notice, this list of conditions and the following disclaimer.",
		"//   * Redistributions in binary form must reproduce the above copyright",
		"// notice, this list of conditions and the following disclaimer in the",
		"// documentation and/or other materials provided with the distribution.",
		"//   * Neither the name of Shuo Chen nor the names of other contributors",
		"// may be used to endorse or promote products derived from this software",
		"// without specific prior written permission.",
		"//",
		"// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS",
		"// \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT",
		"// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR",
		"// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT",
		"// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,",
		"// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT",
		"// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,",
		"// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY",
		"// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT",
		"// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE",
		"// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.",
		"//",
		],
	 "description": "Adds copyright..."
	}
}
```

添加头文件是输入`header`或者`copyright`就可以插入声明。  

## 代码静态检查。

检查工具有cppcheck、clang-tidy。此次说明的时clang-tidy。  

vscode中不需要安装clang-tidy， clang-tidy已集成在c++扩展中。  

### vs code 中运行clang-tidy

#### 手动运行

如果需要手动运行clang-tidy，请打开”Command Palette (Ctrl + Shift + P)”，并输入”Run Code Analysis”。你可以直接在单个文件上执行clang-tidy，也可以在所有已打开的文件上执行，也可以在整个工作区上执行。  

如果有一些文件夹你不想在上面执行clang-tidy，则可以将它们的路径添加到”Clang Tidy: Exclude”配置项中(位于配置文件settings.json中的C_Cpp.codeAnalysis.exclude)。  

#### 自动运行

使用`ctrl+,`打开ui设置，然后搜索”code analysis”关键字来找到所有clang-tidy相关的设置项，然后将”Clang Tidy: Enabled”设置为true。

### 检查规则

// todo

- [1] [Visual Studio Code C++扩展更新：clang-tidy](https://www.163.com/dy/article/GRBCQGH905317NFH.html)

## 调试

### windows

使用vs 脚本（如：Developer Command Prompt for VS 2019）切换到工作目录，`code .`打开项目。按F5，选择相应的编译器即可。

### linux

// todo

#### linux root下启动
```
to open vs-code as root, open terminal and:

sudo code /directory-to-open --user-data-dir='.' --no-sandbox
sudo code . --user-data-dir='/root/.config/Code' --no-sandbox  --disable-gpu-sandbox
make sure you check this too: https://stackoverflow.com/a/68637450/10606346
```

## 连接linux虚拟机开发

需要安装Remote-SSH插件。

- `ctrl+,`打开设置，输入Remote-SSH，勾选Always reveal the SSH login terminal。
- 在ssh-config文件中输入主机地址和用户名。
- 连接，在终端中输入密码登录。





## 其他

- 在windows上可以使用`where cl.exe` 找到编译器问题。如`IntelliSense配置-编译器路径`时。
- tidy或cpplint出现了错误，可以在下方的问题中查看。

- 提示找不到boost头文件，可以在`IntelliSense-包含路径`中设置第三方库路径。

- vbox的虚拟机开启ssh后需要设置端口转发，然后使用127.0.0.1:local_port连接。
