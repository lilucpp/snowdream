#include "threadpool.h"
#include "boost/bind.hpp"
#include "plog/Log.h"

void work_thread(boost::shared_ptr<boost::asio::io_service> pio_service) {
  LOG_INFO << "[" << boost::this_thread::get_id() << "] Thread start...";
  while (!pio_service->stopped()) {
    try {
      boost::system::error_code ec;
      pio_service->run(ec);
      if (ec) {
        LOG_INFO << "[" << boost::this_thread::get_id() << "] Error:" << ec.message();
      }
      break;
    } catch (std::exception &e) {
      LOG_INFO << boost::this_thread::get_id() << " Exception:" << e.what();
    }
    LOG_INFO << "[" << boost::this_thread::get_id() << "] Recall pio_service->run()";
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
  }

  LOG_INFO << "[" << boost::this_thread::get_id() << "] Thread finish...";
}

ThreadPool::ThreadPool(int thread_nums)
    : thread_nums_(thread_nums),
      pio_service_(new boost::asio::io_service()),
      pwork_(new boost::asio::io_service::work(*pio_service_)),
      pstrand_(new boost::asio::io_service::strand(*pio_service_)) {
  for (int i = 0; i < thread_nums_; ++i) {
    thread_group_.create_thread(boost::bind(work_thread, pio_service_));
  }
}

ThreadPool::~ThreadPool() {
  stop();
  thread_group_.join_all();
}

void ThreadPool::stop() {
  if (pwork_) pwork_.reset();
}

ThreadPool &get_thread_pool() {
  static ThreadPool pool(4);
  return pool;
}
