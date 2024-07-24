# 自动输入密码的三种方式

## 第一种

重定向：用重定向方法实现交互的前提是指令需要有参数来指定密码输入方式，如ftp就有-i参数来指定使用标准输入来输入密码

shell用重定向作为标准输入的用法是：cmd<<delimiter ,shell 会将分界符delimiter之后直到下一个同样的分界符之前的内容作为输入

实现ftp自动登录并运行ls指令的用法如下：其中zjk为用户名，zjk123为密码
```shell
ftp -i -n 192.168.21.46 <<EOF
user zjk zjk123
ls
 
EOF
```

## 第二种：

管道：跟重定向一样，指令同样要有参数来指定密码输入方式，如sudo的-S参数，passwd的-stdin参数
所以实现sudo自动输入密码的脚本如下：其中zjk123为密码

```shell
echo 'zjk123' | sudo -S cp file1 /etc/hosts
# 实现自动修改密码的脚本写法如下：
echo 'password' | passwd -stdin username
```

## 第三种：
expect：上面介绍的两种方法前提条件是指令有参数来设定密码输入方式，像ssh指令就没有这样的参数，第三种交互方式就派上用场了
expect就是用来做交互用的，基本任何交互登录的场合都能使用，但是需要安装expect包。
语法如下：
```shell
#!/bin/expect
set timeout 30
spawn ssh -l jikuan.zjk 10.125.25.189
expect "password:"
send "zjk123\r"   
interact

```