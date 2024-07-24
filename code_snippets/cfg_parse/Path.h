#ifndef PATH_EFABC_H
#define PATH_EFABC_H

#include <windows.h>
#include <string>
#include "strconv.h"
#include <vector>
using namespace std;

class PathHelper
{
	public:
	static BOOL GetModulePathW(wstring& ret, wstring fileName, HMODULE hModule = NULL)
	{
		WCHAR szBuff[MAX_PATH*3] = {0};
		GetModuleFileNameW(hModule, szBuff, _MAX_PATH) ;	 
		WCHAR * pdest = NULL;
		pdest = wcsrchr( szBuff, '\\' );
		int len = (int)wcslen(pdest);
		for(int i=1;i<len;i++)
		{
			pdest[i]=0;
			break;
		}
		ret = szBuff;
		ret += fileName;
		return TRUE;
	}
	
	static BOOL GetModulePathA(string& ret, string fileName, HMODULE hModule = NULL)
	{
		CHAR szBuff[MAX_PATH*3] = {0};
		GetModuleFileNameA(hModule, szBuff, _MAX_PATH) ;	 
		CHAR * pdest = NULL;
		pdest = strrchr( szBuff, '\\' );
		int len = (int)strlen(pdest);
		for(int i=1;i<len;i++)
		{
			pdest[i]=0;
			break;
		}
		ret = szBuff;
		ret += fileName;
		return TRUE;
	}
	static BOOL SetTextFileContentW(wstring fileName, wstring content, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL)
	{
		const wchar_t* text = content.c_str();
		BYTE *buf = new BYTE[wcslen(text)*sizeof(WCHAR)+2];
		buf[0] = 0xff;
		buf[1] = 0xfe;
		memcpy(buf +2, text, wcslen(text)*sizeof(WCHAR));
		BOOL bRet = SetFileContentW(fileName, buf, wcslen(text)*sizeof(WCHAR)+2, lpSecurityAttributes);
		delete[] buf;
	
		return bRet;
	}
	
	static BOOL GetTextFileContentW(wstring fileName, wstring& content)
	{
	
		vector<BYTE> bin;
		if(GetFileContentW(fileName, bin) == FALSE)
			return FALSE;
		bin.push_back(0);
		bin.push_back(0);

		if(bin.size()>=2 && bin[0] == 0xff && bin[1] == 0xfe)
		{
			wstring str((const wchar_t*)(&(bin.front()) + 2), (bin.size()-2)/sizeof(wchar_t));
			content = str.c_str();
		}
		else if(bin.size()>=3 && bin[0] == 0xEF && bin[1] == 0xBB && bin[2] == 0xBF)
		{  
			wstring str=StringHelper::SRStrUtf82W((const char*)(&(bin.front()) + 3));
			content = str.c_str();
		}
		else if(bin.size()>0) 
		{
			content = StringHelper::SRStrA2W((const char*)&(bin.front()));
		}
		else
		{
			//	empty file!!!!
		}
		return TRUE;
	}
	
	static BOOL SetFileContentW(wstring fileName, const void* content, int nLength, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL)
	{
		HANDLE hFile = CreateFileW(fileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, lpSecurityAttributes, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			DWORD dwWritten = 0;
			WriteFile(hFile, content, nLength, &dwWritten, NULL);
			CloseHandle(hFile);
			return TRUE;
		}
		return FALSE;
	}

	static BOOL GetFileContentW(wstring fileName, vector<BYTE>& content)
	{
		HANDLE hFile = CreateFileW(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			DWORD dwSize = GetFileSize(hFile, NULL);
			if(dwSize>0)
			{
				content.resize(dwSize);
				SetFilePointer(hFile, 0, 0, FILE_BEGIN);
				DWORD dwRead = 0;
				ReadFile(hFile, &content[0], dwSize, &dwRead, 0);
			}
			CloseHandle(hFile);
			return TRUE;
		}
		return FALSE;
	}
};

#endif //PATH_EFABC_H