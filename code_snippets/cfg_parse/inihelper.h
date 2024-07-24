#ifndef INIHELPER_H
#define INIHELPER_H
#include <string>
#include <vector>
#include "Path.h"

using namespace std;

class IniHelper
{
public:
	//	按XPath获取ini参数
	//	xPath如 L"config\\key"
	//	szFilePath可以是全路径或者只是文件名，文件名时前端不要加"\\"
	static wstring GetText(wstring xPath, wstring szDefault, wstring szFilePath)
	{
		if(szFilePath.find(L":") == -1)
		{
			wstring szName = szFilePath;
			PathHelper::GetModulePathW(szFilePath, szName, NULL);
		}
		vector<wstring> secList;
		String2Vector(xPath, secList);

		if(secList.size() != 2)
			return szDefault;

		wchar_t buf[2048]={0};

		GetPrivateProfileStringW(secList[0].c_str(), secList[1].c_str(), szDefault.c_str(), buf, 2047, szFilePath.c_str());
		return wstring(buf);
	}


	static int GetInt(wstring xPath, int nDefault, wstring szFilePath)
	{
		wchar_t szDefault[64]={0};
		wsprintfW(szDefault, L"%d", nDefault);
		wstring szRet = GetText(xPath, szDefault, szFilePath);

		return _wtoi(szRet.c_str());
	}
	static __int64 GetInt64(wstring xPath, __int64 nDefault, wstring szFilePath)
	{
		wchar_t szDefault[64]={0};
		wsprintfW(szDefault, L"%I64d", nDefault);
		wstring szRet = GetText(xPath, szDefault, szFilePath);

		return _wtoi64(szRet.c_str());
	}

	static int SetText(wstring xPath, wstring szValue, wstring szFilePath)
	{
		if(szFilePath.find(L":") == -1)
		{
			wstring szName = szFilePath;
			PathHelper::GetModulePathW(szFilePath, szName, NULL);
		}
		vector<wstring> secList;
		String2Vector(xPath, secList);

		if(secList.size() != 2)
			return 0;

		return WritePrivateProfileStringW(secList[0].c_str(), secList[1].c_str(), szValue.c_str(), szFilePath.c_str());

	}

	static int SetInt(wstring xPath, int nValue, wstring szFilePath)
	{
		wchar_t szValue[64]={0};
		wsprintfW(szValue, L"%d", nValue);
		return SetText(xPath, szValue, szFilePath);
	}

	static int SetInt64(wstring xPath, __int64 nValue, wstring szFilePath)
	{
		wchar_t szValue[64]={0};
		wsprintfW(szValue, L"%I64d", nValue);
		return SetText(xPath, szValue, szFilePath);
	}

	//	获取所有小节
	static int GetSectionNameList(wstring szConfigFile, vector<wstring>& sections)
	{
		DWORD bufsize=2047;
		WCHAR buf[2048]={0};

		DWORD cnt=::GetPrivateProfileSectionNamesW(buf, bufsize, szConfigFile.c_str());

		if (buf[0]==0)
			return 0;

		const WCHAR* pSecName = buf;
		while(pSecName && wcslen(pSecName)>0)
		{
			sections.push_back(pSecName);
			pSecName += (wcslen(pSecName)+1);
		}

		return sections.size();
	}
	//	获取小节中所有的key
	static int GetKeyNameList(wstring szConfigFile, wstring sectionName, vector<wstring>& keys)
	{
		DWORD bufsize=2047;
		WCHAR buf[2048]={0};

		DWORD cnt=::GetPrivateProfileSectionW(sectionName.c_str(), buf, bufsize, szConfigFile.c_str());

		if (buf[0]==0)
			return 0;

		const WCHAR* pKeyName = buf;
		while(pKeyName && wcslen(pKeyName)>0)
		{
			wstring str(pKeyName);
			int n = str.find(L"=");
			if(n != -1)
				keys.push_back(trim(str.substr(0, n)));

			pKeyName += (wcslen(pKeyName)+1);
		}

		return keys.size();
	}
protected:
	static int String2Vector(wstring str, vector<wstring>& ret)
	{
		wstring temp;
		
		for(size_t i=0;i<str.size();i++)
		{
			if(str[i] == L'\\')
			{
				ret.push_back(temp);
				temp.clear();
			}
			else
			{
				temp += str[i];
			}
		}
		if(!temp.empty())
			ret.push_back(temp);

		return ret.size();
	}

	//	剔除两端空格
	static wstring& trim(wstring &s) 
	{
		if (s.empty()) 
		{
			return s;
		}
		s.erase(0,s.find_first_not_of(L" "));
		s.erase(s.find_last_not_of(L" ") + 1);
		return s;
	}
};

#endif