# 常见问题

1. kylin update2 上关闭增强模式
```txt
"/etc/kysec/kysec.conf"   kysec_status 改成0, 再update2上就可以修改文件了 
```
2. 提权 
目前uos提权：
- 服务启动方式  
    systemctl enable /dev/shm/***.service    
    systemctl start  ***  
- 脚本执行方式
    systemd-run -t /bin/bash -c "***.sh"  