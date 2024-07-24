#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "boost/asio.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/thread.hpp"

class ThreadPool {
 public:
  ThreadPool(int thread_nums = 4);
  ~ThreadPool();

  template <typename Handler>
  void post(Handler handler) {
    pio_service_->post(handler);
  }

  template <typename Handler>
  void strand_post(Handler handler) {
    pstrand_->post(handler);
  }

  void stop();

 private:
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;

 private:
  int thread_nums_;
  boost::shared_ptr<boost::asio::io_service> pio_service_;
  boost::shared_ptr<boost::asio::io_service::work> pwork_;
  boost::shared_ptr<boost::asio::io_service::strand> pstrand_;
  boost::thread_group thread_group_;
};

ThreadPool &get_thread_pool();

#endif  // THREADPOOL_H
