#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

#define CPR_DEFAULT_THREAD_POOL_MAX_THREAD_NUM std::thread::hardware_concurrency()

constexpr size_t CPR_DEFAULT_THREAD_POOL_MIN_THREAD_NUM = 1;
constexpr std::chrono::milliseconds CPR_DEFAULT_THREAD_POOL_MAX_IDLE_TIME{ 60000 };

class ThreadPool {
public:
	using Task = std::function<void()>;

	explicit ThreadPool(size_t min_threads = CPR_DEFAULT_THREAD_POOL_MIN_THREAD_NUM, size_t max_threads = CPR_DEFAULT_THREAD_POOL_MAX_THREAD_NUM, std::chrono::milliseconds max_idle_ms = CPR_DEFAULT_THREAD_POOL_MAX_IDLE_TIME);

	virtual ~ThreadPool();

	void SetMinThreadNum(size_t min_threads) {
		min_thread_num = min_threads;
	}
	void SetMaxThreadNum(size_t max_threads) {
		max_thread_num = max_threads;
	}
	void SetMaxIdleTime(std::chrono::milliseconds ms) {
		max_idle_time = ms;
	}
	size_t GetCurrentThreadNum() {
		return cur_thread_num;
	}
	size_t GetIdleThreadNum() {
		return idle_thread_num;
	}
	bool IsStarted() {
		return status != STOP;
	}
	bool IsStopped() {
		return status == STOP;
	}

	int Start(size_t start_threads = 0);
	int Stop();
	int Pause();
	int Resume();
	int Wait();

	/**
	 * Return a future, calling future.get() will wait task done and return RetType.
	 * Submit(fn, args...)
	 * Submit(std::bind(&Class::mem_fn, &obj))
	 * Submit(std::mem_fn(&Class::mem_fn, &obj))
	 **/
	template <class Fn, class... Args>
	auto Submit(Fn&& fn, Args&&... args) {
		if (status == STOP) {
			Start();
		}
		if (idle_thread_num <= 0 && cur_thread_num < max_thread_num) {
			CreateThread();
		}
		using RetType = decltype(fn(args...));
		auto task = std::make_shared<std::packaged_task<RetType()> >(std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...));
		std::future<RetType> future = task->get_future();
		{
			std::lock_guard<std::mutex> locker(task_mutex);
			tasks.emplace([task] { (*task)(); });
		}

		task_cond.notify_one();
		return future;
	}

private:
	bool CreateThread();
	void AddThread(std::thread* thread);
	void DelThread(std::thread::id id);

public:
	size_t min_thread_num;
	size_t max_thread_num;
	std::chrono::milliseconds max_idle_time;

private:
	enum Status {
		STOP,
		RUNNING,
		PAUSE,
	};

	struct ThreadData {
		std::shared_ptr<std::thread> thread;
		std::thread::id id;
		Status status;
		time_t start_time;
		time_t stop_time;
	};

	std::atomic<Status> status;
	std::atomic<size_t> cur_thread_num;
	std::atomic<size_t> idle_thread_num;
	std::list<ThreadData> threads;
	std::mutex thread_mutex;
	std::queue<Task> tasks;
	std::mutex task_mutex;
	std::condition_variable task_cond;
};

inline ThreadPool::ThreadPool(size_t min_threads, size_t max_threads, std::chrono::milliseconds max_idle_ms) : min_thread_num(min_threads), max_thread_num(max_threads), max_idle_time(max_idle_ms), status(STOP), cur_thread_num(0), idle_thread_num(0) {}

inline ThreadPool::~ThreadPool() {
	Stop();
}

inline int ThreadPool::Start(size_t start_threads) {
	if (status != STOP) {
		return -1;
	}
	status = RUNNING;
	if (start_threads < min_thread_num) {
		start_threads = min_thread_num;
	}
	if (start_threads > max_thread_num) {
		start_threads = max_thread_num;
	}
	for (size_t i = 0; i < start_threads; ++i) {
		CreateThread();
	}
	return 0;
}

inline int ThreadPool::Stop() {
	if (status == STOP) {
		return -1;
	}
	status = STOP;
	task_cond.notify_all();
	for (auto& i : threads) {
		if (i.thread->joinable()) {
			i.thread->join();
		}
	}
	threads.clear();
	cur_thread_num = 0;
	idle_thread_num = 0;
	return 0;
}

inline int ThreadPool::Pause() {
	if (status == RUNNING) {
		status = PAUSE;
	}
	return 0;
}

inline int ThreadPool::Resume() {
	if (status == PAUSE) {
		status = RUNNING;
	}
	return 0;
}

inline int ThreadPool::Wait() {
	while (true) {
		if (status == STOP || (tasks.empty() && idle_thread_num == cur_thread_num)) {
			break;
		}
		std::this_thread::yield();
	}
	return 0;
}

inline bool ThreadPool::CreateThread() {
	if (cur_thread_num >= max_thread_num) {
		return false;
	}
	std::thread* thread = new std::thread([this] {
		bool initialRun = true;
		while (status != STOP) {
			while (status == PAUSE) {
				std::this_thread::yield();
			}

			Task task;
			{
				std::unique_lock<std::mutex> locker(task_mutex);
				task_cond.wait_for(locker, std::chrono::milliseconds(max_idle_time), [this]() { return status == STOP || !tasks.empty(); });
				if (status == STOP) {
					return;
				}
				if (tasks.empty()) {
					if (cur_thread_num > min_thread_num) {
						DelThread(std::this_thread::get_id());
						return;
					}
					continue;
				}
				if (!initialRun) {
					--idle_thread_num;
				}
				task = std::move(tasks.front());
				tasks.pop();
			}
			if (task) {
				task();
				++idle_thread_num;
			}
			else if (initialRun) {
				++idle_thread_num;
				initialRun = false;
			}
		}
		});
	AddThread(thread);
	return true;
}

inline void ThreadPool::AddThread(std::thread* thread) {
	thread_mutex.lock();
	++cur_thread_num;
	ThreadData data;
	data.thread = std::shared_ptr<std::thread>(thread);
	data.id = thread->get_id();
	data.status = RUNNING;
	data.start_time = time(nullptr);
	data.stop_time = 0;
	threads.emplace_back(data);
	thread_mutex.unlock();
}

inline void ThreadPool::DelThread(std::thread::id id) {
	time_t now = time(nullptr);
	thread_mutex.lock();
	--cur_thread_num;
	--idle_thread_num;
	auto iter = threads.begin();
	while (iter != threads.end()) {
		if (iter->status == STOP && now > iter->stop_time) {
			if (iter->thread->joinable()) {
				iter->thread->join();
				iter = threads.erase(iter);
				continue;
			}
		}
		else if (iter->id == id) {
			iter->status = STOP;
			iter->stop_time = time(nullptr);
		}
		++iter;
	}
	thread_mutex.unlock();
}
