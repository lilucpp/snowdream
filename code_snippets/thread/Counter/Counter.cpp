#include "Counter.h"
#include "boost/thread/locks.hpp"

Counter::Counter(int count) : mutex_(), count_(count) {}

void Counter::wait() {
  boost::mutex::scoped_lock lock(mutex_);
  while (count_ > 0) {
    condition_.wait(lock);
  }
}

void Counter::countDown() {
  boost::mutex::scoped_lock lock(mutex_);
  --count_;
  if (count_ == 0) {
    condition_.notify_all();
  }
}

void Counter::countUp() {
  boost::mutex::scoped_lock lock(mutex_);
  ++count_;
}

int Counter::getCount() const {
  boost::mutex::scoped_lock lock(mutex_);
  return count_;
}

ScopeCounter::ScopeCounter(Counter &counter) : counter_(counter) { counter_.countUp(); }

ScopeCounter::~ScopeCounter() { counter_.countDown(); }
