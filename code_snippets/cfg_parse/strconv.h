#ifndef STR_CONV_H
#define STR_CONV_H


#define ERROR_STRCONV_OK						0	//	转换成功

#define ERROR_STRCONV_NULL						-2	//	转换结果为空
#define ERROR_STRCONV_BUFFER_OVERFLOW			-3	//	缓冲区不足
#define ERROR_STRCONV_NOT_COMPLETE 				-4	//	没有完全转换成功，计算的大小与实际转换空间不一样
#define ERROR_STRCONV_BAD_ARGS					-5	//	参数错误
#define ERROR_STRCONV_INVALID_UTF8_STRING		-6	//	无效UTF8字符串
#define ERROR_STRCONV_INVALID_UNICODE_STRING	-7	//	无效Unicode字符串
#define ERROR_STRCONV_INVALID_ASCII_STRING		-8	//	无效Ascii字符串

EXTERN_C int _stdcall SRStrUtf82W(IN LPCSTR szUtf8, OUT LPWSTR szUnicode, IN int nBufSize);	//	Utf8->Unicode
EXTERN_C int _stdcall SRStrW2A(IN LPCWSTR szUnicode, OUT LPSTR szAscii, IN int nBufSize);	//	Unicode->Ascii
EXTERN_C int _stdcall SRStrUtf82A(IN LPCSTR szUtf8, OUT LPSTR szAscii, IN int nBufSize);	//	Utf8->Ascii
EXTERN_C int _stdcall SRStrA2W(IN LPCSTR szAscii, OUT LPWSTR szUnicode, IN int nBufSize);	//	Ascii->Unicode
EXTERN_C int _stdcall SRStrW2Utf8(IN LPCWSTR szUnicode, OUT LPSTR szUtf8, IN int nBufSize);	//	Unicode->Utf8
EXTERN_C int _stdcall SRStrA2Utf8(IN LPCSTR szAscii, OUT LPSTR szUtf8, IN int nBufSize);	//	Ascii->Utf8

typedef int (_stdcall *SRSTRUTF82W)(LPCSTR, LPWSTR, int);
typedef int (_stdcall *SRSTRW2A)(LPCWSTR, LPSTR, int);
typedef int (_stdcall *SRSTRUTF82A)(LPCSTR, LPSTR, int);
typedef int (_stdcall *SRSTRA2W)(LPCSTR, LPWSTR, int);
typedef int (_stdcall *SRSTRW2UTF8)(LPCWSTR, LPSTR, int);
typedef int (_stdcall *SRSTRA2UTF8)(LPCSTR, LPSTR, int);

#define STR_CONV_FUNNAME_UTF82W "SRStrUtf82W"
#define STR_CONV_FUNNAME_W2A	"SRStrW2A"
#define STR_CONV_FUNNAME_UTF82A	"SRStrUtf82A"
#define STR_CONV_FUNNAME_A2W	"SRStrA2W"
#define STR_CONV_FUNNAME_W2UTF8	"SRStrW2Utf8"
#define STR_CONV_FUNNAME_A2UTF8	"SRStrA2Utf8"

#ifdef __cplusplus
#include <string>
#include <vector>
//	C++ STL辅助函数
//	如果使用C++开发，也可以用下面的StringHelper方法

class StringHelper
{
public:
#ifdef STRING_ERROR_CODE
	static int g_nStringHelperLastErrorCode = 0;
#endif
	//	Utf8->Unicode
	static std::wstring SRStrUtf82W(IN std::string szUtf8)
	{
		int nSize = MultiByteToWideChar (CP_UTF8, 0, szUtf8.c_str(), -1, NULL, 0);
		if (nSize == 0)
		{
			return L"";
		}
		std::vector<wchar_t> resultstring(nSize);
		int nConverted = MultiByteToWideChar (CP_UTF8, 0, szUtf8.c_str(), -1, &resultstring[0], nSize);

		if (nConverted != nSize)
		{
			return L"";
		}
		return std::wstring(&resultstring[0]);
	}

	//	Unicode->Ascii
	static std::string SRStrW2A(IN std::wstring szUnicode)
	{
		int nSize = ::WideCharToMultiByte(CP_OEMCP, 0, szUnicode.c_str(), -1, NULL, 0, NULL, NULL);
		if (nSize == 0)
		{
			return "";
		}
		std::vector<char> resultstring(nSize);
		int nConverted = ::WideCharToMultiByte(CP_OEMCP, 0, szUnicode.c_str(), -1, &resultstring[0], nSize, NULL, NULL);

		if (nConverted != nSize)
		{
			return "";
		}
		return std::string(&resultstring[0]);
	}

	//	Utf8->Ascii
	static std::string SRStrUtf82A(IN std::string szUtf8)
	{
		//先把 utf8 转为 unicode
		//然后把 unicode 转为 ascii
		return SRStrW2A(SRStrUtf82W(szUtf8));
	}

	//	Ascii -> Unicode
	static std::wstring SRStrA2W(IN std::string szAscii)
	{
		int nSize = MultiByteToWideChar (CP_ACP, 0, szAscii.c_str(), -1, NULL, 0);
		std::vector<wchar_t> resultstring(nSize);
		int nConverted = MultiByteToWideChar (CP_ACP, 0, szAscii.c_str(), -1, &resultstring[0], nSize);

		if (nConverted != nSize)
		{
			return L"";
		}

		return std::wstring(&resultstring[0]);
	}

	//	Unicode -> Utf8
	static std::string SRStrW2Utf8(IN std::wstring szUnicode)
	{
		int nSize = ::WideCharToMultiByte(CP_UTF8, 0, szUnicode.c_str(), -1, NULL, 0, NULL, NULL);
		if (nSize == 0)
		{
			return "";
		}

		std::vector<char> resultstring(nSize);

		int nConverted = ::WideCharToMultiByte(CP_UTF8, 0, szUnicode.c_str(), -1, &resultstring[0], nSize, NULL, NULL);

		if (nConverted != nSize)
		{
			return "";
		}

		return std::string(&resultstring[0]);
	}

	//	Ascii -> Utf8
	static std::string SRStrA2Utf8(IN std::string szAscii)
	{
		return SRStrW2Utf8(SRStrA2W(szAscii));
	}
#ifdef STRING_ERROR_CODE
	static void SRStrResetLastError()
	{
		g_nStringHelperLastErrorCode = 0;
	}

	//	返回错误码
	static int SRStrGetLastError()
	{
		return g_nStringHelperLastErrorCode;
	}
#endif
};
#endif

#endif	//	STR_CONV_H
