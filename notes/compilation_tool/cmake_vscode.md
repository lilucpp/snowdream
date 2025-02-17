# vscode 中调试 cmake工程
1. launch.json
```json
{
    // 使用 IntelliSense 了解相关属性。 
    // 悬停以查看现有属性的描述。
    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "CMake: Compile and Debug",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceRoot}/build/a.out",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [
                { "name": "LD_LIBRARY_PATH", "value": "../" },
            ],
            "externalConsole": false,
            "MIMode": "gdb",
            "preLaunchTask": "build",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```

2. tasks.json
```json
//.vscode/tasks.json
{
    "version": "2.0.0",
    "tasks": [
        {
            "type": "shell",
            "label": "mkdirbuild",
            "command": "mkdir",
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "args": [
                "-p",
                "build"
            ]
        },
        {
            "type": "shell",
            "label": "cmake",
            "command": "cmake",
            "args": [
                "-DCMAKE_BUILD_TYPE=Debug",
                //在此处添加其它CMAKE选项
                ".."
            ],
            "options": {
                "cwd": "${workspaceFolder}/build"
            },
            "dependsOn": [
                "mkdirbuild"
            ]
        },
        {
            "label": "make",
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "command": "make",
            "args": [
                "-j6",
            ], //根据机器cpu核心数量自行调整
            "options": {
                "cwd": "${workspaceFolder}/build"
            },
        },
        {
            "label": "build",
            "dependsOrder": "sequence",
            "dependsOn": [
                "cmake",
                "make"
            ]
        },
    ],
}
```

3、 root下启动
`sudo code --user-data-dir="~/.vscode-root" --no-sandbox`

## tasks.json 简介


```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "任务名称",
            "type": "shell|process",
            "command": "命令或脚本",
            "args": ["命令参数"],
            "group": {
                "kind": "build|test",
                "isDefault": true|false
            },
            "dependsOn": ["任务1", "任务2"],
            "dependsOrder": "sequence|parallel",
            "problemMatcher": [],
            "options": {
                "cwd": "当前工作目录",
                "env": {
                    "环境变量名": "环境变量值"
                }
            }
        }
    ]
}
```

### 关键字段

1. **version**：指定文件版本，当前为 `2.0.0`。
2. **tasks**：包含任务定义的数组。每个任务由若干属性组成。

### 每个任务的属性说明

1. **label**：

   - 任务的名称，用于标识任务。`label` 必须是唯一的。

2. **type**：

   - `shell`：表示任务是一个 Shell 命令，可以在终端中执行。
   - `process`：表示任务是一个可执行文件或程序。

3. **command**：

   - 要执行的命令或可执行文件路径。

4. **args**：

   - 传递给命令的参数数组。例如：`["-j4"]`。

5. **group**：

   - 设置任务的类别：
     - `"build"` 表示编译任务。
     - `"test"` 表示测试任务。
     - `isDefault`：`true` 表示该任务为默认任务，在按 `Ctrl+Shift+B` 时执行。

6. **dependsOn**：

   - 定义当前任务依赖的其他任务。任务会按依赖关系执行。例如：

     ```json
     "dependsOn": ["task1", "task2"]
     ```

   - 任务将依次等待 `task1` 和 `task2` 执行完成后再执行。

7. **dependsOrder**：

   - 定义依赖的任务执行顺序：
     - `sequence`：依次执行。
     - `parallel`：并行执行。

8. **problemMatcher**：

   - 用于匹配编译输出中的错误或警告，通常用于分析编译日志。

9. **options**：

   - `cwd`：指定任务的工作目录。
   - `env`：设置任务的环境变量。

资料：
[1] https://blog.csdn.net/liuqixuan1994/article/details/125394642
