#include <iostream>
#include <memory>
#include <string>
using namespace std;

/* shared_ptr */
void Shared_ptr()
{
	{
		int a = 10;
		shared_ptr<int> ptra = make_shared<int>(a);
		shared_ptr<int> ptra2(ptra); // copy , ptra引用计数+1
		cout << "ptra的引用计数:" << ptra.use_count() << endl; // 凡是被 拷贝 引用计数都会+1

		int b = 20;
		int *pb = &a;
		//shared_ptr<int> ptrb = pb; // error
		shared_ptr<int> ptrb = make_shared<int>(b);
		ptra2 = ptrb; // assign , ptrb引用计数+1, ptra引用计数-1
		pb = ptrb.get(); // 获取原始指针

		cout << "ptra的引用计数:" << ptra.use_count() << endl;
		cout << "ptrb的引用计数:" << ptrb.use_count() << endl;
		cout << "*pb = " << *pb << endl;
	}
	// 智能指针在作用域外自动内存回收释放
}
void Shared_ptr2()
{
	{
		shared_ptr<string> ptr;
		if (!ptr)
		{
			cout << "智能指针ptr默认初始化为NULL" << endl;
		}

		// 一般的初始化方式
		shared_ptr<string> ptr1(new string("Hello!!"));
		cout << "*ptr1 = " << *ptr1 << endl;

		// 推荐的安全的初始化方式
		shared_ptr<string> ptr2 = make_shared<string>("Hello!!");
		cout << "*ptr2 = " << *ptr2 << endl;
	}
	// 智能指针在作用域外自动内存回收释放
}

/* weak_ptr */
void Weak_ptr()
{
	{
		/* weak_ptr是为了配合shared_ptr而引入的一种智能指针, 最大作用在于协助shared_ptr工作。
		使用weak_ptr的成员函数use_count()可以观测资源的引用计数，另一个成员函数expired()的功能等价于use_count()==0,
		但更快，表示被观测的资源(也就是shared_ptr的管理的资源)已经不复存在。
		weak_ptr可以使用一个非常重要的成员函数lock()从被观测的shared_ptr获得一个可用的shared_ptr对象，从而操作资源。
		但当expired()==true的时候，lock()函数将返回一个存储空指针的shared_ptr。
		*/
		shared_ptr<int> ptr = make_shared<int>(10);
		cout << "ptr的引用计数:" << ptr.use_count() << endl;

		weak_ptr<int> wp(ptr);
		cout << "wp的引用计数:" << wp.use_count() << endl;

		if (!wp.expired())
		{
			shared_ptr<int> ptr2 = wp.lock(); // get another shared_ptr
			*ptr = 100;
			cout << "wp的引用计数:" << wp.use_count() << endl;
			cout << "*ptr = " << *ptr << endl;
		}
	}
	// 智能指针在作用域外自动内存回收释放
}

/* unique_ptr */
void Unique_ptr()
{
	{
		unique_ptr<int> ptr(new int(10)); // 绑定动态对象
		//unique_ptr<int> ptr2 = ptr; // error, 不可赋值
		//unique_ptr<int> ptr2(ptr); // error, 不可拷贝
		unique_ptr<int> ptr2 = move(ptr); // 转移
		//ptr2.release(); // 释放

		cout << *ptr2 << endl;
	}
	// 智能指针在作用域外自动内存回收释放
}

int main(int argc, char **argv)
{
	Shared_ptr2();



	getchar();
	return 0;
}