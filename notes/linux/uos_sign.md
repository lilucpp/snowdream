# UOS 系统签名
## 场景

系统开发者系统下正常，但是在正常用户模式下安装软件，提示没有签名。

## 解决

- 打开统信应用商店，搜索“证书工具”，单击安装证书工具。
- 打开终端（位置随意）执行命令：

    PS: 如果没有UOS账号，去下面贴的官网注册一个，用时约1分钟。
    ```shell
    $ cert-tool -username="UOS帐号" -password="UOS密码"
    # UOS帐号和UOS密码为UOS官方网站https://www.chinauos.com/中注册的帐号和密码
    ```
- deb包签名

    > 这一步是选择未签名的deb包，对deb包进行签名。
    >
    > 首先进入要签名的deb包路径的文件夹下，打开终端。

    ```shell
    进入要签名的deb包路径的文件夹下，打开终端
    $ deepin-elf-sign-deb 要签名的deb包路径
    举例：deepin-elf-sign-deb com.xxx.xxx

    # 单个文件签名
    deepin-elf-sign -f ./test -c /usr/share/ca-certificates/deepin/private/priv.crt  -k   /usr/share/ca-certificates/deepin/private/priv.key
    ```

- deb包签名-验证签名。（这一步可以不做）
    ```shell
    $ deepin-deb-verify [包路径]
    # 比如：
    deepin-deb-verify com.xxx.xx.xxx
    ```
	看到有success就说明成功了，接下来就可以安装了。  

- 安装已签名deb包
