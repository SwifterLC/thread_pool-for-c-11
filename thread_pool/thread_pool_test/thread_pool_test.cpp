#include "pch.h"
#include <algorithm>
#include <iostream>
#include <windows.h>
#include "../thread_pool/thread_pool.h"

using namespace std;
using namespace simple_thread_pool;


//模拟耗时操作
int func(int val)
{
  //sleep 1s-3s
  Sleep(rand() % 2000 + 1000);

  //#mark: 如果要采用异步通知形式,在此处填入通知代码即可
  //...

  //模拟运算
  return val * 2;
}

int main()
{
  //线程池初始化
  ThreadPool* thread_pool = ThreadPool::GetInstance()->Init();

  std::vector<std::future<int>> results{};

  int total = 0;
  for (int i = 0; i < 5; i++) {
    results.push_back(thread_pool->Commit(func, i));
  }

  //完成其他事情....

  //在某个时候主动获取结果
  for (int i = 0; i < results.size(); i++) {
    total += results[i].get();
  }

  cout << "total:" << total;

  ThreadPool::DelInstance();
  return 0;
}