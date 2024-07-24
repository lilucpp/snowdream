# ini解析
## 简介
目前个人常用库，inihelper.h、inicpp.h
## 使用示例

```c++

#include "stdafx.h"
#include "inihelper.h"
#include <iostream>
#include "inicpp.h"

void test1() {
	std::wstring path;
	PathHelper::GetModulePathW(path, L"smb.conf");

	std::wstring value = IniHelper::GetText(L"global\\wins server", L"", path);

	std::cout << "value=" << StringHelper::SRStrW2A(value) << std::endl;
}

void test2() {
  using namespace ini;
	IniFile inifile;
	std::string path;
	PathHelper::GetModulePathA(path, "smb.conf");
    inifile.setCommentPrefixes({"#", ";"});
	inifile.load(path);
	for(auto itr = inifile.begin(); itr != inifile.end(); ++itr) {
          std::cout << itr->first << "\n";
          for (auto itrsec = itr->second.begin(); itrsec !=itr->second.end(); ++itrsec) {
            std::cout << "	" << itrsec->first << "=" << itrsec->second.as<std::string>() << std::endl;
		  }
	}

	inifile["global"]["syslog"]=3;
    inifile.save(path);
}

int _tmain(int argc, _TCHAR* argv[])
{
	test2();
	return 0;
}


```