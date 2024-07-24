#include <iostream>
#include <windows.h>

// https://kingsamchen.github.io/2016/07/30/get-correct-system-version-on-windows-10/
#define DECLARE_DLL_FUNCTION(fn, type, dll) \
    auto fn = reinterpret_cast<type>(GetProcAddress(GetModuleHandleW(L##dll), #fn))

struct VersionNumber {
    unsigned long major_version;
    unsigned long minor_version;
    unsigned long build_version;
};

void GetSystemVersion(VersionNumber& version_number) noexcept
{
    constexpr NTSTATUS kStatusSuccess = 0L;
    DECLARE_DLL_FUNCTION(RtlGetVersion, NTSTATUS(WINAPI*)(PRTL_OSVERSIONINFOW), "ntdll.dll");
    if (!RtlGetVersion) {
        return;
    }

    RTL_OSVERSIONINFOW ovi = {};
    if (RtlGetVersion(&ovi) != kStatusSuccess) {
        return;
    }

    version_number.major_version = ovi.dwMajorVersion;
    version_number.minor_version = ovi.dwMinorVersion;
    version_number.build_version = ovi.dwBuildNumber;
}

int main()
{
  VersionNumber osversion;
  GetSystemVersion(osversion);

  std::cout << "major=" << osversion.major_version << ", minor=" 
    << osversion.minor_version << ", build=" << osversion.build_version << "\n";
  
  return 0;
}

/*
获取操作系统信息，可参考：QOperatingSystemVersion的代码实现：https://code.woboq.org/qt5/qtbase/src/corelib/global/qoperatingsystemversion_win.cpp.html
*/