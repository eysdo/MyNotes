#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <future>  // std::future std::promise std::packaged_task 
using namespace std;

/* 
std::future可以从异步任务中获取结果，一般与std::async配合使用，std::async用于创建异步任务，实际上就是创建一个线程执行相应任务。
std::async就是异步编程的高级封装，封装了std::future的操作，基本上可以代替std::thread 的所有事情。
std::async的操作，其实相当于封装了std::promise、std::packaged_task加上std::thread。
*/
void test()
{
	/*
	deferred：异步操作还没开始
	ready：   异步操作已经完成
	timeout： 异步操作超时
	*/

	future<bool> fut = async([]()->bool {
		this_thread::sleep_for(chrono::seconds(5));
		cout << "睡完5秒了!" << endl;
		return true;
	});
	//wait()只是等待异步操作完成
	//fut.wait();
	future_status fut_status;
	do {
		//fut_status = fut.wait_for(std::chrono::seconds(1)); // 模拟超时
		fut_status = fut.wait_for(std::chrono::seconds(10)); // 异步等待10秒
		if (fut_status == std::future_status::deferred) {
			std::cout << "=";
		}
		else if (fut_status == std::future_status::timeout) {
			std::cout << "异步操作超时!" << endl;
			break;
		}
		else if (fut_status == std::future_status::ready) {
			std::cout << "异步操作已经完成!" << endl;
			break;
		}
	} while (fut_status != std::future_status::ready);

	bool ret = fut.get();
	cout << "final result: " << ret << endl;
}

/*
future对象是std::async、std::promise、std::packaged_task的底层对象，用来传递其他线程中操作的数据结果。
std::promise的作用就是提供一个不同线程之间的数据同步机制，它可以存储一个某种类型的值，并将其传递给对应的future， 即使这个future不在同一个线程中也可以安全的访问到这个值。
说白了: 当promise存储的变量值发生改变就会触发其内部的future为完成状态.
std::ref 用于包装按引用传递的值。
std::cref 用于包装按const引用传递的值。
*/
#include <functional> // std::ref
#include <thread>     // std::thread
void test2()
{
	std::promise<int> prom; // create promise
	std::future<int> fut = prom.get_future(); // engagement with future
	
	// thr 线程1会一直的阻塞等待future的完成后再输出value
	std::thread thr([](std::future<int>& fut) {
		int x = fut.get(); // future未完成会一直的阻塞
		std::cout << "value: " << x << endl;
	}, std::ref(fut)); // send future to new thread

	// thr2 线程2操作完后修改prom的值会触发 future完成事件
	std::thread thr2([](std::promise<int>& prom) {
		this_thread::sleep_for(chrono::seconds(5));
		prom.set_value(10);
	}, std::ref(prom));

	thr.join();
	thr2.join();
}

/*
std::packaged_task的作用就是提供一个不同线程之间的数据同步机制，它可以存储一个函数操作，并将其返回值传递给对应的future， 而这个future在另外一个线程中也可以安全的访问到这个值。
说白了: 当packaged_task存储的function执行完成将会触发其内部future为完成状态.
*/
void test3()
{
	std::packaged_task<int()> tsk([]()->int {
		this_thread::sleep_for(chrono::seconds(5));
		cout << "睡完5秒了!" << endl;
		return 3;
	});
	std::future<int> fut = tsk.get_future();

	// thr 线程1会一直的阻塞等待future的完成后再输出函数的返回值
	std::thread thr([](std::future<int>& fut) {
		int x = fut.get(); // future未完成会一直的阻塞
		std::cout << "function return: " << x << endl;
	}, std::ref(fut)); // send future to new thread

	// thr2 线程2绑定了tsk
	std::thread thr2(std::move(tsk));

	thr.join();
	thr2.join();
}

int main()
{
	test3();


	getchar();
	return 0;
}

