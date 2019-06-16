#pragma once
#include "common.h"
#include <queue>
#include <mutex>

SimpleThreadPoolBegin

template<typename Task>
class BlockTaskQueueT
  :public NoCopiable
{
  using Lock = std::unique_lock<std::mutex>;
public:
  void Add(Task const&task)
  {
    Lock lk{ mtx_ };
    task_block_queue_.emplace(task);
  }

  bool Fetch(Task &task_out)
  {
    Lock lk{ mtx_ };
    if (task_block_queue_.size() == 0) return false;
    std::swap(task_block_queue_.front(), task_out);
    task_block_queue_.pop();
    return true;
  }

  bool IsEmpty()
  {
    Lock lk{ mtx_ };
    return task_block_queue_.empty();
  }
private:
  std::mutex mtx_;
  std::queue<Task> task_block_queue_;
};

SimpleThreadPoolEnd