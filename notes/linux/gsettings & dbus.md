# gsettings命令使用简介 & dbus

您可以使用 gsettings 工具来配置、操作和管理 GSettings 键。查看涉及使用 gsettings 工具的用例的示例。

```txt
gsettings set SCHEMA[:PATH] KEY VALUE
 gsettings monitor SCHEMA[:PATH] [KEY]
 gsettings list-schemas
```

from: https://access.redhat.com/documentation/zh-cn/red_hat_enterprise_linux/8/html/using_the_desktop_environment_in_rhel_8/using-gsettings-command_configuring-gnome-at-low-level#doc-wrapper

可以设置锁屏时间 、黑屏、休眠、系统代理等。

from: https://blog.csdn.net/sz76211822/article/details/112860830  
from: https://linux.cn/article-5673-1.html


## 开启特效
```txt
kwinSettings->beginGroup("Plugins");
kwinSettings->setValue("blurEnabled", true);
kwinSettings->setValue("kwin4_effect_maximizeEnabled", true);
kwinSettings->setValue("kwin4_effect_translucencyEnabled", true);
kwinSettings->setValue("zoomEnabled", true);

 dbus-send --session --print-reply  --dest=org.ukui.KWin /Effects org.ukui.kwin.Effects.loadEffect string:"blur"
 dbus-send --session --print-reply  --dest=org.ukui.KWin /Effects org.ukui.kwin.Effects.loadEffect string:"kwin4_effect_translucency"
 dbus-send --session --print-reply  --dest=org.ukui.KWin /Effects org.ukui.kwin.Effects.loadEffect string:"kwin4_effect_maximize"
 dbus-send --session --print-reply  --dest=org.ukui.KWin /Effects org.ukui.kwin.Effects.loadEffect string:"zoom"

# 透明度
// 透明度设置
#define PERSONALSIE_SCHEMA     "org.ukui.control-center.personalise"
#define PERSONALSIE_TRAN_KEY   "transparency"
#define PERSONALSIE_BLURRY_KEY "blurry"
#define PERSONALSIE_EFFECT_KEY "effect"
#define PERSONALSIE_SAVE_TRAN_KEY "save-transparency"
	
save_trans = personliseGsettings->get(PERSONALSIE_SAVE_TRAN_KEY).toInt();
            ui->tranSlider->setValue(save_trans);
```

### uos是否支持特效
qdbus com.deepin.wm /com/deepin/wm com.deepin.wm.compositingEnabled 的返回值就可以判断。
代码中显示早期版本的dde-control-center 是否据compositingEnabled的值控制一些特效是否开启。

## dbus
```
dbus-send --session --print-reply  --dest=org.ukui.KWin /Effects org.ukui.kwin.Effects.loadEffect string:"blur"
dbus-send --session --print-reply  --dest=org.ukui.KWin /Effects org.ukui.kwin.Effects.loadEffect string:"kwin4_effect_translucency"
dbus-send --session --print-reply  --dest=org.ukui.KWin /Effects org.ukui.kwin.Effects.loadEffect string:"kwin4_effect_maximize"
dbus-send --session --print-reply  --dest=org.ukui.KWin /Effects org.ukui.kwin.Effects.loadEffect string:"zoom"
```

### 查看是否支持特效
 dbus-send --session --print-reply  --dest=org.ukui.KWin /Effects org.ukui.kwin.Effects.areEffectsSupported  array:string:"blur","kwin4_effect_translucency","kwin4_effect_maximize","zoom"
 
 
 
## gdbus
 gdbus call --session \
           --dest org.ukui.KWin \
           --object-path /Effects \
           --method org.ukui.kwin.Effects.loadEffect \
		   "blur"
		   

## uos
qdbus com.deepin.wm /com/deepin/wm com.deepin.wm.compositingEnabled true

## DBUS
可以使用 dbus-monitor 监控所有dbus的操作，也可以使用 图形软件dbustle（apt install dbustle）进行监控。

### 浏览器打开下载文件
```shell
dbus-send --session           --print-reply           --dest=org.freedesktop.FileManager1           /org/freedesktop/FileManager1           org.freedesktop.FileManager1.ShowFolders               array:string:"/home/peanut/下载/xx.zip" string:""
```

## 资料

1. dbus-send: https://dbus.freedesktop.org/doc/dbus-send.1.html
2. UKUI3 框架介绍: https://gitee.com/openkylin/docs/blob/master/%E7%A4%BE%E5%8C%BA%E4%BA%A7%E5%93%81/UKUI/UKUI3%E6%A1%86%E6%9E%B6%E4%BB%8B%E7%BB%8D.md
3. ukui-control-center源码：https://github.com/ukui/ukui-control-center
4. dbus小工具： https://developer.aliyun.com/article/255001