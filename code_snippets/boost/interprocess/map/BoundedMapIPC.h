#pragma once

#include <assert.h>
#include <functional>
#include <string>
#include <vector>
#include <utility>

#include "BIType.h"

namespace Helper {
namespace BI {

class IPMapWarp {
 public:
  typedef SSMultimap::const_iterator CItr;
  typedef SSMultimap::size_type size_type;

  IPMapWarp() : IPMapWarp(std::string("zyq"), 65536) {}

  explicit IPMapWarp(const std::string &shmName, int sharedMemSize = 65536, bool isAutoRemove = false)
      : segment_(bi::open_or_create, shmName.c_str(), sharedMemSize),
        mutex_(segment_.find_or_construct<bi::interprocess_mutex>((std::string("mtx4") + shmName).c_str())()),
        char_alloc_(segment_.get_segment_manager()),
        sspair_alloc_(segment_.get_segment_manager()),
        map_(segment_.find_or_construct<SSMultimap>("multimap")(std::less<KeyStrType>(), sspair_alloc_)),
        isAutoRemove_(isAutoRemove),
        shmName_(shmName) {}

  ~IPMapWarp() {
    if (isAutoRemove_) {
      bi::shared_memory_object::remove(shmName_.c_str());
    }
  }

  void insert(const std::string &key, const std::string &val) {
    bi::scoped_lock<bi::interprocess_mutex> lock(*mutex_);
    map_->emplace(BIString(key.c_str(), char_alloc_), BIString(val.c_str(), char_alloc_));
  }

  std::vector<std::string> equal_range(const std::string &key) {
    bi::scoped_lock<bi::interprocess_mutex> lock(*mutex_);
    std::pair<CItr, CItr> range = map_->equal_range(BIString(key.c_str(), char_alloc_));
	std::vector<std::string> ret;
    for (auto i = range.first; i != range.second; ++i) {
      ret.emplace_back(i->second.c_str());
    }

	return ret;
  }

  std::string find(const std::string &key) {
	std::string ret;
    bi::scoped_lock<bi::interprocess_mutex> lock(*mutex_);
    CItr value = map_->find(BIString(key.c_str(), char_alloc_));
    if (value != map_->end()) ret = value->second.c_str();

	return ret;
  }

  size_type erase(const std::string &key) {
    bi::scoped_lock<bi::interprocess_mutex> lock(*mutex_);
    return map_->erase(BIString(key.c_str(), char_alloc_));
  }

  size_t size() const {
    bi::scoped_lock<bi::interprocess_mutex> lock(*mutex_);
    return map_->size();
  }

  void clear() {
    bi::scoped_lock<bi::interprocess_mutex> lock(*mutex_);
    map_->clear();
  }

 private:
  bi::managed_shared_memory segment_;
  mutable bi::interprocess_mutex *mutex_;
  CharAlloc char_alloc_;
  SSPairAlloc sspair_alloc_;
  SSMultimap *map_;
  bool isAutoRemove_;
  std::string shmName_;
};

}  // namespace BI

}  // namespace Helper
