#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
using namespace std;

mutex mtx; // 全局互斥锁
condition_variable cv; // 条件变量
bool flags = false; // 标志位
int thread_used_count = 0; // 线程调度次数

// 任务列表
queue<int> taskList;

// 主线程 - 分发
void master_thr()
{
	for (int i = 1; i <= 9000; i++)
	{
		unique_lock<mutex> lock(mtx);
		taskList.push(i);
		lock.unlock();
		cv.notify_one();
		//cv.notify_all();
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		cout << "发布任务: " << i << endl;
		//this_thread::yield();
		this_thread::sleep_for(chrono::microseconds(1)); // 让CPU休息会
	}
}

// 工作线程
void work_thr()
{
	while (true)
	{
		unique_lock<mutex> lock(mtx);
		while (taskList.empty()) cv.wait(lock); // 注意一定要用 while
		cout << "thread ID: " << taskList.front() << endl;
		taskList.pop();
		thread_used_count++;
		lock.unlock();
		//this_thread::yield();
		this_thread::sleep_for(chrono::microseconds(1)); // 让CPU休息会
	}
}

int main()
{
	thread master_thrs(master_thr);
	thread work_thrs[4];
	for (size_t i = 0; i < 4; i++)
	{
		work_thrs[i] = move(thread(work_thr));
	}
	for (size_t i = 0; i < 4; i++)
	{
		work_thrs[i].join();
	}
	master_thrs.join();

	getchar();
	return 0;
}