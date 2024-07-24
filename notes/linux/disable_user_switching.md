# 禁用用户切换

## uos
每次用户进程启动调用一次autostart.sh  
卸载执行 postun.sh  

## 麒麟
安装把96-disable-user-switching.conf 这个文件放到/usr/share/lightdm/lightdm.conf.d 下。  
卸载删除/usr/share/lightdm/lightdm.conf.d/96-disable-user-switching.conf  

## 脚本

调用autostart.sh
```c++
void disableUserSwitch() {
#ifdef __linux__
    std::string systemname = Helper::ssystem("lsb_release -i");
    spdlog::info("system name: {}", systemname);
    if(systemname.find("Uos") != std::string::npos) {// Uos
        Helper::ssystem("bash xx/autostart.sh");
    } else if(systemname.find("Kylin") != std::string::npos){ // Kylin
        std::string fname("/usr/share/lightdm/lightdm.conf.d/96-disable-user-switching.conf");
        if (access(fname.c_str(), F_OK) != 0) {
            // file doesn't exist
            spdlog::error("96-disable-user-switching.conf not exist.");
        }
    } else {
        spdlog::error("unknow system name: {}", systemname);
    }

#endif 
}
```

autostart.sh
```shell
#!/bin/bash
autologinvisable=`gsettings get com.deepin.dde.control-center auto-login-visable`
autologinvisableup=$(echo $autologinvisable | tr [a-z] [A-Z])

switchuser1=`gsettings get com.deepin.dde.sessionshell.control switchuser`
switchuser2=`gsettings get com.deepin.dde.session-shell switchuser`

if [ $autologinvisableup = TRUE ]; then
    gsettings set com.deepin.dde.control-center auto-login-visable false
    echo "set auto-login-visable $(gsettings get com.deepin.dde.control-center auto-login-visable)"
else
    echo 'already set auto-login-visable false'
fi

if [ $switchuser1 != 2 ]; then
    gsettings set com.deepin.dde.sessionshell.control switchuser 2
    echo "set control switchuser $(gsettings get com.deepin.dde.sessionshell.control switchuser)"
else
    echo 'already set control switchuser 2'
fi

if [ $switchuser2 != 2 ]; then
    gsettings set com.deepin.dde.session-shell switchuser 2
    echo "set shell switchuser $(gsettings get com.deepin.dde.session-shell switchuser)"
else
    echo 'already set shell switchuser 2'
fi
```

postun.sh
```shell
#!/bin/bash
autologinvisable=`gsettings get com.deepin.dde.control-center auto-login-visable`
autologinvisableup=$(echo $autologinvisable | tr [a-z] [A-Z])

switchuser1=`gsettings get com.deepin.dde.sessionshell.control switchuser`
switchuser2=`gsettings get com.deepin.dde.session-shell switchuser`

if [ $autologinvisableup != TRUE ]; then
    gsettings set com.deepin.dde.control-center auto-login-visable true
    echo "set auto-login-visable $(gsettings get com.deepin.dde.control-center auto-login-visable)"
else
    echo 'already set auto-login-visable true'
fi

if [ $switchuser1 != 1 ]; then
    gsettings set com.deepin.dde.sessionshell.control switchuser 1
    echo "set control switchuser $(gsettings get com.deepin.dde.sessionshell.control switchuser)"
else
    echo 'already set control switchuser 1'
fi

if [ $switchuser2 != 1 ]; then
    gsettings set com.deepin.dde.session-shell switchuser 1
    echo "set shell switchuser $(gsettings get com.deepin.dde.session-shell switchuser)"
else
    echo 'already set shell switchuser 1'
fi
```
96-disable-user-switching

```txt
[Seat:*]
allow-user-switching=False
```

卸载删除脚本

```shell
# 删除kylin 禁用用户切换
if lsb_release -i | grep Kylin; then 
rm  -f /usr/share/lightdm/lightdm.conf.d/96-disable-user-switching.conf &> /dev/null
if

# 删除UOS 禁用用户切换
if lsb_release -i | grep Uos; then 
bash /xx/postun.sh  &> /dev/null
fi 
```