#pragma once
#include <mutex>

#define SimpleThreadPoolBegin namespace simple_thread_pool{

#define SimpleThreadPoolEnd }


SimpleThreadPoolBegin
class NoCopiable
{
public:
  NoCopiable() = default;
  NoCopiable(NoCopiable const&) = delete;
  NoCopiable(NoCopiable &&) = delete;
  NoCopiable& operator=(NoCopiable const&) = delete;
  NoCopiable& operator=(NoCopiable &&) = delete;
};

template<typename T>
class SingletonT
  :public NoCopiable
{
  using Lock = std::unique_lock<std::mutex>;
protected:
  SingletonT() = default;
public:
  static T* GetInstance()
  {
    //double check
    if (instance_ == nullptr) {
      Lock lock(mtx_);
      if (instance_ == nullptr) {
        instance_ = new T;
      }
    }
    return instance_;
  }

  static void DelInstance()
  {
    Lock lock(mtx_);
    if (instance_) {
      delete instance_;
      instance_ = nullptr;
    }
  }
private:
  static T *instance_;
  static std::mutex mtx_;
};

template<typename T>
T* SingletonT<T>::instance_ = nullptr;

template<typename T>
std::mutex SingletonT<T>::mtx_;

SimpleThreadPoolEnd


#define DeclSingleton(className) public simple_thread_pool::SingletonT<className>\
{\
friend class simple_thread_pool::SingletonT<className>;