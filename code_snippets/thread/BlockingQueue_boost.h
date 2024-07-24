//
// Author: lu li (lilucpp at gmail dot com)

#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include <boost/thread/locks.hpp>
#include "boost/noncopyable.hpp"
#include "boost/thread/condition.hpp"
#include "boost/thread/mutex.hpp"

#include <assert.h>
#include <deque>

template <typename T>
class BlockingQueue : boost::noncopyable {
 public:
  BlockingQueue() : mutex_(), notEmpty_(), queue_() {}

  void put(const T &x) {
    boost::lock_guard<boost::mutex> lock(mutex_);
    queue_.push_back(x);
    notEmpty_.notify_one();  // wait morphing saves us
                             // http://www.domaigne.com/blog/computing/condvars-signal-with-mutex-locked-or-not/
  }

  T take() {
    boost::lock_guard<boost::mutex> lock(mutex_);
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
    boost::lock_guard<boost::mutex> lock(mutex_);
    return queue_.size();
  }

 private:
  mutable boost::mutex mutex_;
  boost::condition notEmpty_;
  std::deque<T> queue_;
};

#endif  // BLOCKINGQUEUE_H