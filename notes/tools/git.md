# git基础
![git](./img/git.png)
专用名词的译名如下: 

```txt
Workspace：工作区
Index / Stage：暂存区
Repository：仓库区（或本地仓库）
Remote：远程仓库
```

## 新建代码库
```bash
# 在当前目录新建一个Git代码库
git init
git add .
git commit -m "init"
git remote add origin https://github.com/lilucpp/RegEx.git
git push -u origin master

# 新建一个目录，将其初始化为Git代码库
git init your_project
cd your_project

# 下载一个项目和它的整个代码历史
git clone xxx
```
## 配置
```bash
# 显示当前的配置
git config --list

# 编辑Git配置文件<br>
git config -e [--global]

# 设置提交代码时的用户信息
git config --global user.name 'lilu'
git config --global user.email 'lilucpp@gmail.com'

# 设置代理
git config --global https.proxy http://127.0.0.1:1080
git config --global https.proxy https://127.0.0.1:1080
git config --global --unset http.proxy
git config --global --unset https.proxy

git config --global http.proxy 'socks5://127.0.0.1:1080'
git config --global https.proxy 'socks5://127.0.0.1:1080'
```

## 增加/删除文件
```bash
# 添加指定文件到暂存区
$ git add [file1] [file2] ...

# 添加指定目录到暂存区，包括子目录
$ git add [dir]

# 添加当前目录的所有文件到暂存区
$ git add .

# 删除工作区文件，并且将这次删除放入暂存区
$ git rm [file1] [file2] ...

# 改名文件，并且将这个改名放入暂存区
$ git mv [file-original] [file-renamed]

# 恢复删除的文件
$ git checkout  .
```

## 分支管理
```bash
# 列出所有本地分支
$ git branch

# 列出所有远程分支
$ git branch -r

# 列出所有本地分支和远程分支
$ git branch -a

# 新建一个分支，但依然停留在当前分支
$ git branch [branch-name]

# 新建一个分支，并切换到该分支
$ git checkout -b [branch]

# 切换到指定分支，并更新工作区
$ git checkout [branch-name]

# 建立追踪关系，在现有分支与指定的远程分支之间
$ git branch --set-upstream [branch] [remote-branch]

# 删除远程分支
$ git push origin --delete develop

# 删除本地分支
$ git branch -d develop

# 拉取远程分支
$ git checkout -b develop origin/develop

# 提交到远程仓库
$ git push -u origin develop
$ git push -u origin feature-branch:feature-branch //本地:远程(没有会自动创建)

```

## 标签
```bash
# 列出所有tag
$ git tag

# 新建一个tag在当前commit
$ git tag [tag]

# 新建一个tag在指定commit
$ git tag [tag] [commit]

# 删除本地tag
$ git tag -d [tag]

# 删除远程tag
$ git push origin :refs/tags/[tagName]

# 新建一个分支，指向某个tag
$ git checkout -b [branch] [tag]
```

## 日志
```bash
# 显示一个文件的记录
git log -p filename

#指定路径或文件
git log pathxx

# 搜索提交的代码中特定字符串
git log -S function_name

# 查看两个版本的不同
git diff commit_id commit_id
```
## 远程同步
```bash
# 下载远程仓库的所有变动
$ git fetch [remote]

# 同步分支列表，本地列表就不会显示远程被删除的分支
$ git fetch -p

# 显示所有远程仓库
$ git remote -v

# 显示某个远程仓库的信息
$ git remote show [remote]

# 增加一个新的远程仓库，并命名
$ git remote add [shortname] [url]

# 取回远程仓库的变化，并与本地分支合并
$ git pull [remote] [branch]

# 上传本地指定分支到远程仓库
$ git push [remote] [branch]

# 强行推送当前分支到远程仓库，即使有冲突
$ git push [remote] --force

# 推送所有分支到远程仓库
$ git push [remote] --all
```

## 其他
```bash
# 依赖模块， 同时项目中会出现一个新的 .gitmodules 配置文件，保存了一些映射关系。
git submodule add 3RD_LIB_GIT_PATH
git clone --recursive MAIN_PROJECT_GIT

# 不输入密码
  # 长期存储密码：
    git config --global credential.helper store
  # 还可以设置[ssh]
    参考conf_ssh.md

# 生成一个可供发布的压缩包
git archive

# 拉去子模块
  # 首次拉取时，直接拉去所有的子模块
    git clone --recursive https://github.com/lilucpp/base.git
  # 拉去主模块后，调用下面的命令拉去
    git submodule init
    git submodule update
```

## 查看状态
  使用 `git status -s` 命令或 `git status --short` 命令，你将得到一种格式更为紧凑的输出。
  ```bash
  $ git status -s
  M README
  MM Rakefile
  A  lib/git.rb
  M  lib/simplegit.rb
  ?? LICENSE.txt
  ```
  新添加的未跟踪文件前面有 `??` 标记，新添加到暂存区中的文件前面有 `A` 标记，修改过的文件前面有 `M` 标记。 <br>
  输出中有两栏，左栏指明了暂存区的状态，右栏指明了工作区的状态。例如，`Rakefile` 文件已修，暂存后又作了修改，因此该文件的修改中既有已暂存的部分，又有未暂存的部分。<br>

`.gitignore` 的格式规范如下：
```txt
- 所有空行或者以 `#` 开头的行都会被 Git 忽略。
- 可以使用标准的 glob 模式匹配，它会递归地应用在整个工作区中。// 简化了的正则表达式
- 匹配模式可以以（`/`）开头防止递归。
- 匹配模式可以以（`/`）结尾指定目录。
- 要忽略指定模式以外的文件或目录，可以在模式前加上叹号（`!`）取反。
```

星号（`*`）匹配零个或多个任意字符；`[abc]` 匹配任何一个列在方括号中的字符；<br> 
问号（`?`）只匹配一个任意字符；<br>
`[0-9]` 表示匹配所有 0 到 9 的数字；<br>
使用两个星号（`**`）表示匹配任意中间目录，比如 `a/**/z` 可以匹配 `a/z` 、 `a/b/z` 或 `a/b/c/z` 等；<br>

  ```.gitignore
  # 忽略所有的 .a 文件
  *.a

  # 但跟踪所有的 lib.a，即便你在前面忽略了 .a 文件
  !lib.a

  # 只忽略当前目录下的 TODO 文件，而不忽略 subdir/TODO
  /TODO

  # 忽略任何目录下名为 build 的文件夹
  build/

  # 忽略 doc/notes.txt，但不忽略 doc/server/arch.txt
  doc/*.txt

  # 忽略 doc/ 目录及其所有子目录下的 .pdf 文件
  doc/**/*.pdf
  ```
  GitHub 有一个十分详细的针对数十种项目及语言的 `.gitignore` 文件列表， 你可以在 https://github.com/github/gitignore 找到它。<br>
  在最简单的情况下，一个仓库可能只根目录下有一个 `.gitignore` 文件，它递归地应用到整个仓库中。 然而，子目录下也可以有额外的 `.gitignore` 文件。子目录中的 `.gitignore` 文件中的规则只作用于它所在的目录中。 

## Git 客户端使用
- git自身的gui界面<br>
    不太好用，使用git gui查看文件时有乱码，可参考：https://segmentfault.com/a/1190000000578037
- TortoiseGit<br>
    和svn的使用方式基本一致，对于习惯svn客户端的用户来说，上好的选择。
- vscode的插件gitlens<br>
    显示酷炫，鼠标移动时可查看代码的提交记录，提交时输入ctrl+shift+p --> git commit --> 输入提交记录 --> git push
- **Sourcetree**：推荐这个

## 清除历史记录

```shell
git checkout --orphan new_branch
git add .
git commit -m "init"
git branch -D develop
git branch -m develop
git push -f -u origin develop
```

## clone 指定标签
```shell
git clone --depth 1 --branch <tag_name> <repo_url>
```

## 撤销
撤销commit提交
```shell
git reset --soft HEAD^
```

## utf-16问题
智能解决本地比较，gitlab上的diff还是不显示。  

```
None of the solutions worked for me, so I did some more research and at this link I found out this:

in your project main directory, create or edit a .gitattributes file adding this line:

*.strings diff=localizablestrings
in the .gitconfig file in your home directory, add these lines:

[diff "localizablestrings"]
textconv = "iconv -f utf-16 -t utf-8"
This did the trick.
```

## 设置代理
```shell
# 设置代理
git config --global http.proxy socks5://127.0.0.1:10808
git config --global https.proxy socks5://127.0.0.1:10808

# 取消代理
git config --global --unset http.proxy
git config --global --unset https.proxy
```

## 切换到某一次
```shell
git checkout commit_hash
```

参考

1. https://www.ruanyifeng.com/blog/2015/12/git-cheat-sheet.html