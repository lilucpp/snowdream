#pragma once

#include <Wbemidl.h>
#include <comdef.h>
#include <wbemcli.h>
#include <winnls.h>

#include <codecvt>
#include <iomanip>
#include <string>
#include <sstream>
#include <map>
#include <vector>

#include "hardDeviceSerialNumber.h"

#pragma comment(lib, "wbemuuid.lib")

namespace peanut {

  namespace impl {

    std::string ConvertWstringToUtf8(const std::wstring& wstr) {
      std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
      return converter.to_bytes(wstr);
    }

    std::string Trim(const std::string& str) {
      size_t first = str.find_first_not_of(' ');
      if (std::string::npos == first)
        return str;
      size_t last = str.find_last_not_of(' ');
      return str.substr(first, (last - first + 1));
    }

    bool ConvertSerialNumber(std::string& serialNumber) {
      bool retCode = true;
      std::string tmp = serialNumber;

      std::stringstream ss;
      ss << std::hex << std::setw(2) << std::setfill('0');

      for (size_t i = 0; i < serialNumber.length(); i += 4) {
        std::string hexPair = serialNumber.substr(i, 4);
        long hexValue = std::stol(hexPair, nullptr, 16);

        if (hexValue < 32 || hexValue > 126) {
          retCode = false;
          break;
        }

        // Convert hex value to corresponding character
        ss << static_cast<char>(hexValue);
      }

      if (retCode)
        serialNumber = ss.str();
      else
        serialNumber = tmp;

      return retCode;
    }

  } // namespace impl

  class HardwareInfo {
  public:
    HardwareInfo() : initialized_(Initialize()) {}

    bool GetDiskSerialNumbers(std::map<int, std::string>& diskSerialNumbers) {
      if (!initialized_)
        return false;

      IWbemServices* pServices = nullptr;
      if (!ConnectServer(&pServices))
        return false;

      IEnumWbemClassObject* pEnumerator = nullptr;

      std::string query = "SELECT * FROM Win32_DiskDrive WHERE MediaType LIKE 'Fixed%hard disk%'";
      HRESULT result = pServices->ExecQuery(bstr_t("WQL"), bstr_t(query.c_str()),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        nullptr, &pEnumerator);
      if (FAILED(result)) {
        pServices->Release();
        return false;
      }

      result = ProcessDiskSerialNumbers(pEnumerator, diskSerialNumbers);

      pServices->Release();
      if (pEnumerator != nullptr)
        pEnumerator->Release();

      return SUCCEEDED(result);
    }

    bool GetCpuId(std::string& id) {
      if (!initialized_)
        return false;

      IWbemServices* pServices = nullptr;
      if (!ConnectServer(&pServices))
        return false;

      IEnumWbemClassObject* pEnumerator = nullptr;

      HRESULT result = pServices->ExecQuery(bstr_t("WQL"), bstr_t("SELECT * FROM win32_Processor"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        nullptr, &pEnumerator);
      if (FAILED(result)) {
        pServices->Release();
        return false;
      }

      result = ProcessHardwareId(pEnumerator, L"ProcessorId", id);

      pServices->Release();
      if (pEnumerator != nullptr)
        pEnumerator->Release();

      return SUCCEEDED(result);
    }

    bool GetSystemId(std::string& id) {
      if (!initialized_)
        return false;

      IWbemServices* pServices = nullptr;
      if (!ConnectServer(&pServices))
        return false;

      IEnumWbemClassObject* pEnumerator = nullptr;

      HRESULT result = pServices->ExecQuery(bstr_t("WQL"), bstr_t("SELECT * FROM win32_ComputerSystemProduct"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        nullptr, &pEnumerator);
      if (FAILED(result)) {
        pServices->Release();
        return false;
      }

      result = ProcessHardwareId(pEnumerator, L"UUID", id);

      pServices->Release();
      if (pEnumerator != nullptr)
        pEnumerator->Release();

      return SUCCEEDED(result);
    }

    bool GetDiskIds(std::vector<std::string>& diskIds) {
      std::map<int, std::string> diskSerialNumbers;
      if (GetDiskSerialNumbers(diskSerialNumbers)) {
        for (const auto& item : diskSerialNumbers) {
          std::string serialNum = item.second;
          if (serialNum.empty()) {
            char serialNo[1024] = { 0 };
            HARDDISKSERIAL::GetSerialNo(item.first, serialNo);
            if (strlen(serialNo))
              diskIds.push_back(serialNo);
          }
          else {
            if (serialNum.length() == 40) // Disk serial number in hexadecimal representation
              impl::ConvertSerialNumber(serialNum);
            diskIds.push_back(impl::Trim(serialNum));
          }
        }
        return true;
      }

      return false;
    }

  private:
    bool initialized_;

    bool Initialize() {
      CoUninitialize();
      HRESULT result = CoInitializeEx(0, COINIT_MULTITHREADED);
      if (FAILED(result))
        return false;
      result = CoInitializeSecurity(nullptr, -1, nullptr, nullptr,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        nullptr, EOAC_NONE, nullptr);
      if (FAILED(result)) {
        CoUninitialize();
        return false;
      }
      return true;
    }

    bool ConnectServer(IWbemServices** pServices) {
      *pServices = nullptr;
      IWbemLocator* pLocator = nullptr;
      HRESULT result = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&pLocator);
      if (FAILED(result))
        return false;

      result = pLocator->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), nullptr, nullptr, 0,
        0, 0, 0, pServices);
      pLocator->Release();
      if (FAILED(result))
        return false;

      result = CoSetProxyBlanket(*pServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE);
      if (FAILED(result)) {
        (*pServices)->Release();
        return false;
      }

      return true;
    }

    HRESULT ProcessDiskSerialNumbers(IEnumWbemClassObject* pEnumerator, std::map<int, std::string>& diskSerialNumbers) {
      IWbemClassObject* pclsObj;
      ULONG uReturn = 0;
      while (pEnumerator) {
        HRESULT result = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (0 == uReturn)
          break;
        VARIANT vtPropId;
        result = pclsObj->Get(L"Index", 0, &vtPropId, 0, 0);
        if (vtPropId.vt == VT_I4) {
          VARIANT vtPropNum;
          result = pclsObj->Get(L"SerialNumber", 0, &vtPropNum, 0, 0);
          if (vtPropNum.vt == VT_BSTR && vtPropNum.bstrVal != nullptr)
            diskSerialNumbers[vtPropId.intVal] = impl::ConvertWstringToUtf8(vtPropNum.bstrVal);
          else
            diskSerialNumbers[vtPropId.intVal] = "";
          VariantClear(&vtPropNum);
        }
        VariantClear(&vtPropId);
        pclsObj->Release();
      }
      return S_OK;
    }

    HRESULT ProcessHardwareId(IEnumWbemClassObject* pEnumerator, const wchar_t* propertyName, std::string& id) {
      IWbemClassObject* pclsObj;
      ULONG uReturn = 0;
      while (pEnumerator) {
        HRESULT result = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (0 == uReturn)
          break;
        VARIANT vtProp;
        result = pclsObj->Get(propertyName, 0, &vtProp, 0, 0);
        if (vtProp.vt == VT_NULL)
          id = "";
        else if (vtProp.vt == VT_BSTR && vtProp.bstrVal != nullptr)
          id = impl::ConvertWstringToUtf8(vtProp.bstrVal);
        else
          id = "";
        VariantClear(&vtProp);
        pclsObj->Release();
      }
      return S_OK;
    }
  };

  HardwareInfo gHardwareInfo;

} // namespace peanut
