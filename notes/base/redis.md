# redis

## 快速入门

```shell
# 安装
sudo apt install redis-server redis-tools

# 查看redis服务是否启动 
systemctl status redis

# 客户端连接，基本使用
redis-cli -h 127.0.0.1 -p 6379
> SET bike:1 "Process 134"
> GET bike:1
Process 134
> HSET bike:1 model Deimos brand Ergonom type 'Enduro bikes' price 4972
(integer) 4
> HGET bike:1 model
"Deimos"
> HGET bike:1 price
"4972"
> HGETALL bike:1
1) "model"
2) "Deimos"
3) "brand"
4) "Ergonom"
5) "type"
6) "Enduro bikes"
7) "price"
8) "4972"

```

支持 Data structure store、Document database、Vector database。

### Redis 内存占用是多少？

- 一个空实例使用约 3MB 内存。
- 100 万个小的键 -> 字符串值对使用约 85MB 的内存。

### “Redis”这个名字从何而来？

Redis 是**RE** mote **DI** ctionary **S** erver的首字母缩写。

## 连接

### 连接方式

redis-cli、redis insight图形用户界面、client library。

c library: hiredis。

### 设置密码

修改配置文件后，重启redis服务。

```
# /etc/redis/redis.conf
requirepass your_password
```

### 云环境

- 云环境规则放开6379端口。

- 修改redis.conf，重启redisk。

  ```
  # /etc/redis/redis.conf bind 127.0.0.1
  bind 0.0.0.0
  ```

  



## 参考

- [Redis教程 ](https://www.redis.net.cn/tutorial/3502.html)
- [Redis文档](https://redis.ac.cn/docs/)
- [Redis 阿里云连接问题](https://www.cnblogs.com/flzs/p/15517067.html)