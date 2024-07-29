# svn转git

## 安装git-svn

```shell
sudo apt install git-svn
```

## svn2git

```shell
git svn clone 项目svn地址
```

## 进入到下载的项目目录

```shell
cd project_name(项目名称)
```

## 建立连接

```shell
git remote add origin 新git地址
```

## 创建分支

```shell
git checkout -b develop
```

## 推到远程

```shell
git push -f -u origin develop  --tags
```
