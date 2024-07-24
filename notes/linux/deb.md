# 总结

- deb包空目录<br>
    使用控制文件创建，遇到一个问题是升级安装时空文件不被创建。
- deb包自升级<br>
    自升级程序不结束，在prerm文件中改名，然后执行安装。

- deb安装过程
    ```shell
    安装
    preinst param num=2
    preinst param1=install  1.0.13 

    postinst param num=2
    postinst param1=configure  1.0.13 

    卸载
    prerm param num=1
    prerm param1=remove 

    postrm param num=1
    postrm param1=remove  


    升级
    14 prerm param num=2
    14 prerm param1=upgrade  1.0.15 

    15 preinst param num=2
    15 preinst param1=upgrade  1.0.14 

    14 postrm param num=2
    14 postrm param1=upgrade  1.0.15 

    15 postinst param num=2
    15 postinst param1=configure  1.0.14 

    ```
- 常见cpu架构
    ```txt
    x86_64
    aarch64
    mips64
    loongarch64
    ```
- 下载deb包
    ```shell
    sudo apt --download-only --assume-yes install xxx
    cp /var/cache/apt/archives/xxx ~/Desktop/
    ```
## 下载包
`apt downlaod package_name`

## 资料
deb环境变量：https://manpages.debian.org/bookworm/dpkg/dpkg.1.en.html#Internal_environment
安装升级流程图：https://www.debian.org/doc/debian-policy/ap-flowcharts.html

## rpm
```
Name: cn.com.superred.test
Version: 1.0
Release: 1
Summary: test
#BuildArch: x86_64

License: GPL
URL: www.superred.com.cn

%description
test

%prep
echo "BUILDROOT = $RPM_BUILD_ROOT"
exit

%build

%install
rm -rf $RPM_BUILD_ROOT
install -d $RPM_BUILD_ROOT/
cp -a * $RPM_BUILD_ROOT/
chmod -R 0666 $RPM_BUILD_ROOT/*


%clean

%files
%defattr(066,root,root,0777)


%pre
#!/bin/bash
set +e

#卸载0 安装1 升级2 
if [[ $1 = 2 ]]; then
	echo "xxx"
fi

%post
#!/bin/bash
set +e

#卸载0 安装1 升级2 
echo "post param=$1"

%preun
#!/bin/bash
set +e

#删除包是0 更新包是1 卸载之前
echo "preun preun=$1"


exit 0

%postun
#!/bin/bash
set +e

#删除包是0 更新包是1 
echo "postun param=$1"

echo "uninstall done!"
exit 0
```

## 如何获取包名
有个场景是获取服务器地址，一般有二次打包、根据文件吗获取。
根据文件名获取：(https://unix.stackexchange.com/questions/691854/how-to-get-the-deb-package-full-path-in-the-pre-install-script/701687#701687)
```shell
CALLING_DIR=$(realpath /proc/$PPID/cwd)
DEBIAN_PKG=$(cd "${CALLING_DIR}"; realpath $(xargs -0 -n 1 < /proc/$PPID/cmdline | tail -n 1))
echo "packagename=${DEBIAN_PKG}"
```
解析包名服务器地址：
```c++
std::string extractServerInfoFromFilenameImpl(const std::string& filepath) {
    // Extract filename from filepath
    std::string filename = filepath.substr(filepath.find_last_of("/\\") + 1);

    // Regular expression pattern to match IP address and optional port number
    std::regex ipPortPattern("_((?:(?:[a-z](?:[a-z0-9-]{0,61}[a-z0-9])?\\.)+[a-z0-9][a-z0-9-]{0,61}[a-z0-9])|(?:(?:(?:25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\.){3}(?:25[0-5]|2[0-4]\\d|[01]?\\d\\d?)))(?:_(\\d{1,5}))?");

    // Search for the first match of IP address and port
    std::smatch match;
    if (std::regex_search(filename, match, ipPortPattern)) {
        if (match.size() != 3) {
            spdlog::error("regex_search error, filename={}", filename);
            return "";
        }

        std::string protocol = filename.find("https") == std::string::npos ? "http://" : "https://";
        std::string ipOrDomain = match.str(1);
        std::string port = match.str(2);

        if (port.empty()) {
            port = (protocol == "https://") ? DEFAULT_HTTPS_PORT : DEFAULT_HTTP_PORT;
        }

        int iport = std::stoi(port);
        if (iport < 1 || iport > 65535) {
            spdlog::error("port [1,65535] error: {}", port);
            return "";
        }

        return protocol + ipOrDomain + ":" + port;
    } else {
        spdlog::info("Error: Unable to extract server information from filename: {}.", filename);
        return ""; // Parsing failed
    }
}

bool extractServerInfoFromFilename(const std::string& filepath) {
    std::string host = getServerAddr();
    if (!host.empty()) {
        spdlog::info("extractServerInfoFromFilename use pre host={}", host);
        // Use config file
        return true;
    }

    std::string serverInfo = extractServerInfoFromFilenameImpl(filepath);
    if (!serverInfo.empty()) {
        spdlog::info("extract server information from filename: {} -> {}", filepath, serverInfo);
        saveServerAddr(serverInfo);
        return true;
    } else {
        return false;
    }
}

void test_extractServerInfoFromFilename() {
   std::vector<std::pair<std::string, std::string>> validFormats = {
      {"http_installer_192.168.3.11_5000.exe", "http://192.168.3.11:5000"},
      {"installer_192.168.3.11_5000.exe", "http://192.168.3.11:5000"},
      {"https_installer_192.168.3.11_443.rpm", "https://192.168.3.11:443"},
      {"installer_192.168.3.11_5000.rpm", "http://192.168.3.11:5000"},
      {"installer_http_192.168.3.11_5000.rpm", "http://192.168.3.11:5000"},
      {"installer_http_192.168.3.11_5000(8).rpm", "http://192.168.3.11:5000"},
      {"installer_http_192.168.3.11_5000（8）.rpm", "http://192.168.3.11:5000"},
      {"installer_http_192.168.3.11.rpm", "http://192.168.3.11:80"},
      {"installer_https_192.168.3.11.rpm", "https://192.168.3.11:443"},
      {"installer_http_192.168.3.11_0.rpm", ""},
      {"installer_http_192.168.3.11_1.rpm", "http://192.168.3.11:1"},
      {"installer_http_192.168.3.11_65535(8).rpm", "http://192.168.3.11:65535"},
      {"installer_http_192.168.3.11_65536.rpm", ""},
      {"192.168.3.11_65536.rpm", ""},
      {"_192.168.3.11_123.rpm", "http://192.168.3.11:123"},
        {"你好_192.168.3.11_123.rpm", "http://192.168.3.11:123"}
   };

    for (const auto& test : validFormats) {
      std::string filepath = test.first;
      std::string expectedHost = test.second;
      std::string result = extractServerInfoFromFilenameImpl(filepath);
      if(result.compare(expectedHost) != 0) {
        std::cout << "error" << "path=" << filepath << " result=" << result << " expected=" << expectedHost << std::endl;
      } 
  }
  }

```