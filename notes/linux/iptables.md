# 防火墙
## 学习资料
iptables命令： https://wangchujiang.com/linux-command/c/iptables.html#%E5%B7%A5%E4%BD%9C%E6%9C%BA%E5%88%B6
超级详细的iptable教程文档: https://www.cnblogs.com/Dicky-Zhang/p/5904429.html

## 简介
iptables命令 是Linux上常用的防火墙软件，是netfilter项目的一部分。可以直接配置，也可以通过许多前端和图形界面配置。  


## 使用
`iptables -t 表名 <-A/I/D/R> 规则链名 [规则号] <-i/o 网卡名> -p 协议名 <-s 源IP/源子网> --sport 源端口 <-d 目标IP/目标子网> --dport 目标端口 -j 动作`  

注意：规则的次序非常关键，谁的规则越严格，应该放的越靠前，而检查规则的时候，是按照从上往下的方式进行检查的。

## 常用
```shell
# 端口控制
iptables  -A  INPUT  -s 192.168.122.0/23  -p  tcp  -m multiport  --dport 21:23,2227:2230  -j  ACCEPT  

# 头部插入
-I, --insert chain [rulenum] rule-specification 在链 chain 中的指定位置插入一条或多条规则。如果指定的规则号是1，则在链的头部插入。这也是默认的情况，如果没有指定规则号

# 默认全放行
iptables -P INPUT ACCEPT   
iptables -P OUTPUT ACCEPT 
```