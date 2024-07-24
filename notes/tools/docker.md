# docker

1. 导出iamge文件

   您将需要将Docker映像另存为tar文件：
   ```
   docker save -o <path for generated tar file> <image name>
   ```
   那么你的图像复制到与普通文件传输工具，如一个新的系统`cp`，`scp`或`rsync`（首选大文件）。之后，您必须将映像加载到Docker中：
   ```
   docker load -i <path to image tar file>
   ```
   如果是arm的docker，需要安装qemu-user-static
   ```
   apt-get install qemu-user-static
   ```
   PS：您可能需要`sudo`所有命令。  
   编辑：您应该使用-o添加文件名（而不仅仅是目录），例如：
   ```shell
   docker save -o c:/myfile.tar centos:16
   ```
   来源：https://stackoverflow.com/a/23938978/8330816

2. 提交变化
   ```shell
   docker commit -a="lilu" -m="add dep library" 65ee072b60df xxxc-amd64-fdzx:1.2
   ```
3. 运行容器
   ```txt
   常用参数：
   -i: 表示运行容器
   -d: 创建一个守护式进程在后台运行
   -t: 表示容器启动后进入其命令行
   --name: 设置容器名字
   -v: 表示目录映射关系，前者为主机目录，后者为容器内目录，多个-v可以做多个映射
   -p: 端口映射，前者为宿主机端口，后者为容器内端口，可以hi用多个-p做多个端口映射
   ```
- 如挂载宿主机/root/docker到容器的/root目录
   ```shell
   docker run -it -d --name amd64-qt4 -v /root/docker:/root xxxc-amd64-fdzx:1.2 /bin/bash
   docker attach amd64-qt4
   ```
- 常见的用法
   ```shell
   docker run -id --name ubuntu ubuntu
   docker exec -it ubuntu /bin/bash
   ```
4. 其他
   ```txt
   docker stop ubuntu
   docker start ubuntu
   docker cp 1.txt ubuntu:/home
   docker cp ubuntu:/home/1.txt ./2.txt
   // 查看容器运行的信息
   docker inspect ubuntu
   docker rm 容器名
   docker rmi 删除镜像
   ```
5. ssh连接docker
   ```txt
   # cmd中执行
   docker run -id --name peanut  -v C:\\Users\\lu\\Documents\\wkspace:/root/wkspace -p 10023:22 ubuntu:latest
   docker exec -it exec -it peanut /bin/bash
   
   #进入容器后
   apt-get update
   apt-get install passwd openssl openssh-server openssh-clients vim
   
   # 修改密码
   passwd root

   # 修改sshd配置 vim /etc/ssh/sshd_config
   PubkeyAuthentication yes #启用公钥私钥配对认证方式
   PermitRootLogin yes #root能使用ssh登录
   port=22 #开启22端口
   RSAAuthentication yes #启用 RSA 认证
   
   #最后需要重启一下ssh服务
   service ssh restart
   ```




