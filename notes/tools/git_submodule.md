# 第三方库引入

## 项目中引入第三方库

使用git中的[子模块](https://git-scm.com/book/zh/v2/Git-%E5%B7%A5%E5%85%B7-%E5%AD%90%E6%A8%A1%E5%9D%97)来引入第三方库。

### 设置子模块

在需要引入第三方库的项目中使用下面命令引入子模块。

```shell
$ git submodule add 3RD_LIB_GIT_PATH
$ git add .
$ git commit -m "build: add submodule"
$ git push -u origin develop
```

引入指定分支的子模块 （ 薛磊提供）   

```shell
git submodule add -b version 3RD_LIB_GIT_PATH localpath
```

#### 示例：

demo项目引入pugixml（version 1.8的分支）到本地目录third-party： 

```shell
peanut@peanut-vm:~/project/demo$ ll
总用量 20
drwxrwxr-x  3 peanut peanut 4096 4月  18 17:30 ./
drwxr-xr-x 34 peanut peanut 4096 4月  18 17:28 ../
drwxrwxr-x  8 peanut peanut 4096 4月  18 17:30 .git/
-rw-rw-r--  1 peanut peanut   93 4月  18 17:30 main.cpp
-rw-rw-r--  1 peanut peanut    8 4月  18 17:28 README.md
peanut@peanut-vm:~/project/demo$ git submodule add -b 1.8 https://xx/pugixml.git ./third_library
正克隆到 '/home/peanut/project/demo/third_library'...
remote: Enumerating objects: 9, done.
remote: Counting objects: 100% (9/9), done.
remote: Compressing objects: 100% (7/7), done.
remote: Total 9 (delta 2), reused 0 (delta 0), pack-reused 0
展开对象中: 100% (9/9), 完成.
peanut@peanut-vm:~/project/demo$ ll
总用量 28
drwxrwxr-x  4 peanut peanut 4096 4月  18 17:32 ./
drwxr-xr-x 34 peanut peanut 4096 4月  18 17:28 ../
drwxrwxr-x  9 peanut peanut 4096 4月  18 17:32 .git/
-rw-rw-r--  1 peanut peanut  165 4月  18 17:32 .gitmodules
-rw-rw-r--  1 peanut peanut   93 4月  18 17:30 main.cpp
-rw-rw-r--  1 peanut peanut    8 4月  18 17:28 README.md
drwxrwxr-x  2 peanut peanut 4096 4月  18 17:32 third_library/
peanut@peanut-vm:~/project/demo$ git add .
peanut@peanut-vm:~/project/demo$ git status .
位于分支 develop
您的分支与上游分支 'origin/develop' 一致。

要提交的变更：
  （使用 "git reset HEAD <文件>..." 以取消暂存）

	新文件：   .gitmodules
	新文件：   third_library

peanut@peanut-vm:~/project/demo$ git commit -m "feat: add pugixml"
[develop 492cdbc] feat: add pugixml
 2 files changed, 5 insertions(+)
 create mode 100644 .gitmodules
 create mode 160000 third_library
peanut@peanut-vm:~/project/demo$ ls
main.cpp  README.md  third_library
peanut@peanut-vm:~/project/demo$ git push -u origin develop
```

### 代码拉取
- 直接拉取（包括子模块）   
  
```shell
  git clone --recursive https://xx/test_library.git
```

- 拉去主模块后，调用下面的命令拉取  

  ```shell
  $ git submodule init
  $ git submodule update
  ```

### 子模块原理

.gitmodules文件中会记录第三方库的地址以及本地路径。

```shell
peanut@peanut-vm:~/project/demo$ cat .gitmodules 
[submodule "third_library"]
	path = third_library
	url = https://xx/pugixml.git
	branch = 1.8

```

### 子模块更新

如果子模块更新后，需要在父项目中更新。

```shell
git submodule update --init
git submodule update --remote
git add 子项目所在文件夹
git commit -m "说明信息"
git push
```

