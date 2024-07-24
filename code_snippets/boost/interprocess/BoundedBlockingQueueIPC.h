#ifndef PEANUT_BASE_MPMCQUEUE_H
#define PEANUT_BASE_MPMCQUEUE_H

#include <assert.h>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/deque.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <string>

namespace peanut {
using namespace boost::interprocess;

class MPMCQueue {
 public:
  typedef allocator<char, managed_shared_memory::segment_manager> CharAlloc;
  typedef basic_string<char, std::char_traits<char>, CharAlloc> SharedString;
  typedef allocator<SharedString, managed_shared_memory::segment_manager> StringAlloc;
  typedef deque<SharedString, StringAlloc> InterprocessQueue;

  explicit MPMCQueue(int maxSize) : MPMCQueue(maxSize, false, std::string("peanutTest")) {}
  explicit MPMCQueue(int maxSize, bool isAutoRemove, const std::string &sharedMemoryName)
      : segment_(open_or_create, sharedMemoryName.c_str(), 65536),
        mutex_(segment_.find_or_construct<interprocess_mutex>("myMutex")()),
        notEmpty_(segment_.find_or_construct<interprocess_condition>("myconditionEmpty")()),
        notFull_(segment_.find_or_construct<interprocess_condition>("myconditionFull")()),
        char_alloc_(segment_.get_segment_manager()),
        string_alloc_(segment_.get_segment_manager()),
        queue_(segment_.find_or_construct<InterprocessQueue>("peanutQueue")(string_alloc_)),
        maxSize_(maxSize),
        isAutoRemove_(isAutoRemove),
        shmName_(sharedMemoryName) {}

  ~MPMCQueue() {
    if (isAutoRemove_) {
      shared_memory_object::remove(shmName_.c_str());
    }
  }

  void put(const std::string &x) {
    scoped_lock<interprocess_mutex> lock(*mutex_);

    while (queue_->size() == maxSize_) {
      notFull_->wait(lock);
    }

    assert(!(queue_->size() == maxSize_));
    queue_->push_back(SharedString(x.c_str(), char_alloc_));
    notEmpty_->notify_one();
  }

  std::string take() {
    scoped_lock<interprocess_mutex> lock(*mutex_);

    while (queue_->empty()) {
      notEmpty_->wait(lock);
    }
    assert(!queue_->empty());
    std::string front(queue_->front().c_str());
    queue_->pop_front();
    notFull_->notify_one();
    return front;
  }

  bool empty() const {
    scoped_lock<interprocess_mutex> lock(*mutex_);

    return queue_->empty();
  }

  bool full() const {
    scoped_lock<interprocess_mutex> lock(*mutex_);

    return queue_->size() == maxSize_;
  }

  size_t size() const {
    scoped_lock<interprocess_mutex> lock(*mutex_);

    return queue_->size();
  }

  size_t capacity() const {
    scoped_lock<interprocess_mutex> lock(*mutex_);

    return queue_->max_size();
  }

  void unlock() { mutex_->unlock(); }

 private:
  managed_shared_memory segment_;
  mutable interprocess_mutex *mutex_;
  interprocess_condition *notEmpty_;
  interprocess_condition *notFull_;
  CharAlloc char_alloc_;
  StringAlloc string_alloc_;
  InterprocessQueue *queue_;
  int maxSize_;
  bool isAutoRemove_;
  std::string shmName_;
};

}  // namespace peanut

#endif  // PEANUT_BASE_MPMCQUEUE_H
