# 文件监控

使用inotify可以实现文件监控，如打开、新建、访问、关闭、移动（inotify_event中IN_MOVE_FROM和IN_MOVE_TO的cookie相同）、重命名（移动的路径相同则是重命名）等。  

## ctrl+c/v拷贝、剪切、粘贴文件。
无法做到有效监控。  
原理：可以使用下面的命令输出copy、剪切行为xclip -o -selection clipboard 