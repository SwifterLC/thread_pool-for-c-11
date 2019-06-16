#pragma once
#include "common.h"
#include <future>

SimpleThreadPoolBegin

//class Task;
//using TaskPtr = std::shared_ptr<Task>;
//
//class Task :
//	public std::enable_shared_from_this<Task>,
//	public NoCopiable,
//	public std::function<void()>
//{
//private:
//	using Parent = std::function<void()>;
//	using Parent::Parent;
//public:
//	template<typename Fn, typename...Args>
//	static decltype(auto) WrapTask(Fn&&fn, Args&&...args)
//	{
//		using RetType = typename std::result_of<Fn(Args...)>::type;
//		auto pt = std::make_shared<std::packaged_task<RetType()>>(std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...));
//		return std::make_pair(std::make_shared<Task>([pt] {(*pt)(); }), pt);
//	}
//
//	//template<typename Notify, typename Fn, typename...Args>
//	//static decltype(auto) WrapTaskAndNotify(Notify&& notify, Fn&&fn, Args&&...args)
//	//{
//	//	using RetType = typename std::result_of<Fn(Args...)>::type;
//	//	auto pt = std::make_shared<std::packaged_task<RetType()>>([&]
//	//	{
//	//		// TODO
//	//	});
//	//	return std::make_pair(std::make_shared<Task>([pt] {(*pt)(); }), pt);
//	//}
//};


SimpleThreadPoolEnd