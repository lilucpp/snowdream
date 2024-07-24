//
// Author: lu li (lilucpp at gmail dot com)

#pragma once

#include <assert.h>
#include <deque>
#include <mutex>
#include <thread>

#include "noncopyable.h"

template <typename T>
class BlockingQueue : noncopyable {
 public:
  BlockingQueue() : mutex_(), notEmpty_(), queue_() {}

  void put(const T &x) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push_back(x);
    notEmpty_.notify_one();  // wait morphing saves us
                             // http://www.domaigne.com/blog/computing/condvars-signal-with-mutex-locked-or-not/
  }

  T take() {
    std::lock_guard<std::mutex> lock(mutex_);
    // always use a while-loop, due to spurious wakeup
    while (queue_.empty()) {
      notEmpty_.wait(mutex_);
    }
    assert(!queue_.empty());
    T front(queue_.front());

    queue_.pop_front();

    return front;
  }

  size_t size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
  }

 private:
  mutable std::mutex mutex_;
  std::condition_variable_any notEmpty_;
  std::deque<T> queue_;
};