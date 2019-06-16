#pragma once
#include "common.h"
#include "block_task_queue.h"
#include "task.h"

#include <thread>
#include <vector>
#include <future>

//fixed count thread pool
SimpleThreadPoolBegin

const unsigned int thread_pool_default_thread_count = std::thread::hardware_concurrency() * 2 + 2;

//#mark:非线程安全
class ThreadPool :public simple_thread_pool::SingletonT<ThreadPool>
{
  friend class simple_thread_pool::SingletonT<ThreadPool>;
  using Lock = std::unique_lock<std::mutex>;
  using Task = std::function<void()>;

  ThreadPool() = default;

  ~ThreadPool() { StopJoin(); }
public:
  template<class Fn, class... Args>
  auto Commit(Fn&& fn, Args&&... args)->std::future<typename std::result_of<Fn(Args...)>::type>
  {
    if (is_stop_) throw std::runtime_error("线程池已经销毁");
    if (!is_init_) throw std::runtime_error("线程池未初始化");

    using RetType = typename std::result_of<Fn(Args...)>::type;
    auto pt = std::make_shared<std::packaged_task<RetType()>>(std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...));
    auto ft = pt->get_future();
    block_task_queue_.Add([pt] {(*pt)(); });
    task_queue_is_not_empty_.notify_one();
    return std::move(ft);
  }

  ThreadPool* Init(unsigned int thread_count = thread_pool_default_thread_count)
  {
    if (is_stop_ || is_init_)return this;
    is_init_ = true;
    for (unsigned int i = 0; i < thread_count; i++) {
      thread_pool_.emplace_back([=] {
        while (true) {
          Task tk{};
          { //降低锁粒度
            Lock lk{ mtx_ };
            task_queue_is_not_empty_.wait(lk, [&] {return is_stop_ || block_task_queue_.Fetch(tk); });
            if (is_stop_) {
              break;
            }
          }
          tk();
        }
      });
    }

    return this;
  }

  void Stop()
  {
    if (is_stop_)return;
    is_stop_ = true;
    task_queue_is_not_empty_.notify_all();
  }

  void StopJoin()
  {
    if (is_stop_)return;
    Stop();
    std::for_each(thread_pool_.begin(), thread_pool_.end(), [](std::thread &td) {if (td.joinable())td.join(); });
    thread_pool_.clear();
  }
private:
  std::atomic<bool> is_stop_{ false };
  std::atomic<bool> is_init_{ false };

  std::condition_variable task_queue_is_not_empty_;
  std::mutex mtx_;

  std::vector<std::thread> thread_pool_;
  BlockTaskQueueT<Task> block_task_queue_;
};

SimpleThreadPoolEnd