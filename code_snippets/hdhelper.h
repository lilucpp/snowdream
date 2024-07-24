#pragma once

#include <set>
#include <string>
#include <cctype>
#include <cstdlib>
#include "nlohmann/json.hpp"
#include "ssystem.h"

namespace Helper {
namespace detail {
inline nlohmann::json parseString(const std::string &str) {
  nlohmann::json j;

  if (str.empty()) return j;
  try {
    j = nlohmann::json::parse(str);
  } catch (nlohmann::json::exception &e) {
    // balabala
  } catch (...) {
    // balabala
  }

  return j;
}

inline bool isValidSN(const std::string& sn) {
  return std::all_of(sn.begin(), sn.end(), [](char c) {
     //1-9 a-zA-Z \s \t - _ /
     if(isalnum(c) || isblank(c) || c == '-' || c== '_' || c == '/') 
      return true;
     
     return false;
    }
  );
}

inline bool isMasterHDImpl(nlohmann::json j) {
  if (j.is_object() && j.contains("mountpoint")) {
    if (j["mountpoint"].is_string())
      if (j["mountpoint"].get<std::string>().compare("/") == 0) {
        return true;
      }
  }

  return false;
}

inline bool isMasterHD(nlohmann::json j) {
  if (isMasterHDImpl(j)) return true;

  if (j.contains("children")) {
    for (auto it = j["children"].begin(); it != j["children"].end(); ++it) {
      if (isMasterHDImpl(*it)) return true;
    }
  }

  return false;
}

inline std::string getHDCodeDirect(nlohmann::json j) {
  std::string ret;

  if (j.is_object() && j.contains("serial") && j["serial"].is_string()) ret = j["serial"].get<std::string>();

  if(isValidSN(ret)) {
    return ret;
  } 

  return "";
}

inline bool isSkipHDType(nlohmann::json j) {
  static std::set<std::string> container{"rom"};

  if (j.is_object() && j.contains("type")) {
    if (j["type"].is_string())
      if (container.find(j["type"].get<std::string>()) != container.end()) {
        return true;
      }
  }

  return false;
}

inline std::string getHDCodeImpl(bool onlymaster, bool other, std::string delim = ";") {
  std::string ret;

  std::string command("lsblk -J -no NAME,SERIAL,MOUNTPOINT,TYPE");
  std::string str = Helper::ssystem(command.c_str());
  nlohmann::json j = parseString(str);

  try {
    nlohmann::json bds = j["blockdevices"];
    for (auto it = bds.begin(); it != bds.end(); ++it) {
      if (isSkipHDType(*it)) continue;

      bool isMaster = isMasterHD(*it);
      if (isMaster) {
        if (onlymaster) return getHDCodeDirect(*it);
        if (other) continue;
      }

      if (!ret.empty()) ret += delim;

      ret += getHDCodeDirect(*it);
    }
  } catch (...) {
    // balabala
  }

  return ret;
}  // namespace detail
}  // namespace detail

inline std::string getMasterHDCode() { return detail::getHDCodeImpl(true, false); }

inline std::string getOtherHDCode(std::string delim = ";") { return detail::getHDCodeImpl(false, true, delim); }

inline std::string getAllHDCode(std::string delim = ";") { return detail::getHDCodeImpl(false, false, delim); }

}  // namespace Helper
