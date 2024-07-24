# xp支持

## 底层支持

[YY-Thunks](https://github.com/Chuyu-Team/YY-Thunks)（鸭船），存在的目的就是抹平不同系统的差异，编译时单纯添加一个 obj 即可自动解决这些兼容性问题。让你兼容旧版本 Windows 更轻松！

### 支持的编译器

全平台ABI兼容。

- 所有Visual Studio版本均支持 （比如：VC6.0、VS2008、VS2010、VS2015、VS2017、VS2019等等）。
- 所有运行库模式均支持（比如：`/MD`、`/MT`、`/MDd`、`/MTd`）。

### 手工配置

1. 下载 [YY-Thunks-Binary](https://github.com/Chuyu-Team/YY-Thunks/releases)， 然后解压到你的工程目录。
2. 【链接器】-【输入】-【附加依赖项】，添加 `objs\$(PlatformShortName)\YY_Thunks_for_WinXP.obj`。
3. 重新编译代码。

> 温馨提示：如果需要兼容 Vista，请选择 `objs\$(PlatformShortName)\YY_Thunks_for_Vista.obj`。

## qt

qt的最后支持版本5.6.3，可以使用这个版本的qt程序进行静态编译使用。

另外如果想在qt项目中使用，需要在pro文件中做一些设置。

```txt
win32:{
    LIBS += $$PWD/lib/YY_Thunks_for_WinXP.obj

    DEFINES += _ATL_XP_TARGETING
    DEFINES += PSAPI_VERSION=1
    QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01

    QMAKE_CFLAGS_RELEASE += -MT
    QMAKE_CXXFLAGS_RELEASE += -MT
}

```

## cmake

```cmake
if(UNIX)
    #xxx
else()
	target_sources(project_name PRIVATE
		./lib/YY_Thunks_for_WinXP.obj
	)
	# 设置链接器子系统为控制台应用程序，并指定 Windows 版本为 5.01
	set_target_properties(project_name PROPERTIES
		WIN32_EXECUTABLE TRUE
		LINK_FLAGS "/SUBSYSTEM:CONSOLE,5.01"
	)
    target_link_libraries(project_name PRIVATE  Version.lib)
endif()

```

## vs2019

1. 项目菜单->项目属性->链接器->系统->子系统->控制台或窗口windows（根据你的项目类型选择），第二项版本号设成5.01。

2. C/C++->链接器->命令行，输入相应的命令行，这一步非常关键，否则会出现“不是有效的win32应用程序”提示;

   ```txt
   根据程序设计的运行环境是图形窗口还是命令行窗口设置不同的命令行选项，如果运行环境是命令行窗口则使用如下命令：
   
   /SUBSYSTEM:CONSOLE,”5.01” (for Win32用在32位平台) 
   
   /SUBSYSTEM:CONSOLE,”5.02” (for x64用在64平台)
   
   如果运行环境是图形窗口则使用如下命令:
   /SUBSYSTEM:WINDOWS,”5.01” (for Win32用在32位平台) 
   
   /SUBSYSTEM:WINDOWS,”5.02” (for x64用在64平台)
   
   如果你用NMAKE工和，把这个链接参数设置到编译器的链接标志。
   ```

   参考：http://www.11.re/VC/910.html

