#ifndef CITOSIS_COUNT_H
#define CITOSIS_COUNT_H

#include "boost/noncopyable.hpp"
#include "boost/thread/condition.hpp"
#include "boost/thread/mutex.hpp"

class Counter : boost::noncopyable {
 public:
  explicit Counter(int count);

  void wait();

  void countDown();

  void countUp();

  int getCount() const;

 private:
  mutable boost::mutex mutex_;
  boost::condition condition_;
  int count_;
};

class ScopeCounter : boost::noncopyable {
 public:
  explicit ScopeCounter(Counter &count);

  ~ScopeCounter();

 private:
  Counter &counter_;
};

#endif  // CITOSIS_COUNT_H
