# 条件判断

`if`是最常用的条件判断结构，只有符合给定条件时，才会执行指定的命令。它的语法如下。

```shell
if commands; then
  commands
[elif commands; then
  commands...]
[else
  commands]
fi
```

这个命令分成三个部分：`if`、`elif`和`else`。其中，后两个部分是可选的。

## 1. test 命令

`if`结构的判断条件，一般使用`test`命令，有三种形式。  

test - check file types and compare values.   

```
# 写法一
test expression

# 写法二
[ expression ]

# 写法三
[[ expression ]]
```

上面三种形式是等价的，但是第三种形式还支持正则判断，前两种不支持。

上面的`expression`是一个表达式。这个表达式为真，`test`命令执行成功（返回值为`0`）；表达式为伪，`test`命令执行失败（返回值为`1`）。注意，第二种和第三种写法，`[`和`]`与内部的表达式之间必须有空格。

```
$ test -f /etc/hosts
$ echo $?
0

$ [ -f /etc/hosts ]
$  echo $?
0
```

上面的例子中，`test`命令采用两种写法，判断`/etc/hosts`文件是否存在，这两种写法是等价的。命令执行后，返回值为`0`，表示该文件确实存在。  

实际上，`[`这个字符是`test`命令的一种简写形式，可以看作是一个独立的命令，这解释了为什么它后面必须有空格。

## 2. 判断表达式

`if`关键字后面，跟的是一个命令。**这个命令可以是`test`命令，也可以是其他命令**。命令的返回值为`0`表示判断成立，否则表示不成立。因为这些命令主要是为了得到返回值，所以可以视为表达式。

常用的判断表达式有下面这些。

1. 关于某个文件名的『类型』侦测(存在与否)，如 test -e filename 

2. 字符串判断。

   [ string ] 不为空为真。

   [ string1 = string2 ] 相等为真

   [ string1 != string2 ] 不等为真

3. 关于文件的权限侦测，如 test -r filename 

4. 整数判断

   -eq   =

   -nq   !=

   -lt    <

   -le   <=

   -gt   >

   -ge  >=

5. 正则判断

   ```txt
   [[ string1 =~ regex ]]
   ```

   上面的语法中，`regex`是一个正则表示式，`=~`是正则比较运算符。

   ```shell
   #!/bin/bash
   
   INT=-5
   
   if [[ "$INT" =~ ^-?[0-9]+$ ]]; then
     echo "INT is an integer."
     exit 0
   else
     echo "INT is not an integer." >&2
     exit 1
   fi
   ```

6. 逻辑判断

   AND： &&，-a

   OR: ||， -o

   NOT：!

### 3. case 结构

`case`结构用于多值判断，可以为每个值指定对应的命令，跟包含多个`elif`的`if`结构等价，但是语义更好。它的语法如下。

```
case expression in
  pattern )
    commands ;;
  pattern )
    commands ;;
  ...
esac
```



附录：

1. true：true命令啥都不做，只设置退出码为0。在Linux底下，每个程序执行完毕都会返回一个退出码给调用者，一般情况下0表示成功，其他值表明有问题，当然某些程序的退出码有特殊含义。