# 参数

$?: 上一个命令的退出码。

$#: 脚本参数数量

$0: 脚本文件名

$1-$9: 1-9个参数

$@: 全部的参数，参数之间使用空格分隔。

```bash
for i in "$@"; do 
  echo $i
done
```



## getopts 命令

`getopts`命令用在脚本内部，可以解析复杂的脚本命令行参数，通常与`while`循环一起使用，取出脚本所有的带有前置连词线（`-`）的参数。

```
getopts optstring name
```

它带有两个参数。第一个参数`optstring`是字符串，给出脚本所有的连词线参数。比如，某个脚本可以有三个配置项参数`-l`、`-h`、`-a`，其中只有`-a`可以带有参数值，而`-l`和`-h`是开关参数，那么`getopts`的第一个参数写成`lha:`，顺序不重要。注意，`a`后面有一个冒号，表示该参数带有参数值，`getopts`规定带有参数值的配置项参数，后面必须带有一个冒号（`:`）。`getopts`的第二个参数`name`是一个变量名，用来保存当前取到的配置项参数，即`l`、`h`或`a`。

下面是一个例子。

```shell
while getopts 'lha:' OPTION; do
  case "$OPTION" in
    l)
      echo "linuxconfig"
      ;;

    h)
      echo "h stands for h"
      ;;

    a)
      avalue="$OPTARG"
      echo "The value provided is $OPTARG"
      ;;
    ?)
      echo "script usage: $(basename $0) [-l] [-h] [-a somevalue]" >&2
      exit 1
      ;;
  esac
done
shift "$(($OPTIND - 1))"
```

上面例子中，`while`循环不断执行`getopts 'lha:' OPTION`命令，每次执行就会读取一个连词线参数（以及对应的参数值），然后进入循环体。变量`OPTION`保存的是，当前处理的那一个连词线参数（即`l`、`h`或`a`）。如果用户输入了没有指定的参数（比如`-x`），那么`OPTION`等于`?`。循环体内使用`case`判断，处理这四种不同的情况。

如果某个连词线参数带有参数值，比如`-a foo`，那么处理`a`参数的时候，环境变量`$OPTARG`保存的就是参数值。

注意，只要遇到不带连词线的参数，`getopts`就会执行失败，从而退出`while`循环。比如，`getopts`可以解析`command -l foo`，但不可以解析`command foo -l`。另外，多个连词线参数写在一起的形式，比如`command -lh`，`getopts`也可以正确处理。

变量`$OPTIND`在`getopts`开始执行前是`1`，然后每次执行就会加`1`。等到退出`while`循环，就意味着连词线参数全部处理完毕。这时，`$OPTIND - 1`就是已经处理的连词线参数个数，使用`shift`命令将这些参数移除，保证后面的代码可以用`$1`、`$2`等处理命令的主参数。