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
            "environment": [],
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

资料：
[1] https://blog.csdn.net/liuqixuan1994/article/details/125394642
