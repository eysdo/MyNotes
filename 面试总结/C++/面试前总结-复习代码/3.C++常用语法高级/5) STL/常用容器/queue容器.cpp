#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<queue> // 队列, 先进先出
using namespace std;

void quePrint(queue<int> q)
{
	while (!q.empty())
	{
		cout << q.front() << " ";
		q.pop();
	}
	cout << endl;
}

/*  queue构造函数

queue<T> queT;//queue采用模板类实现，queue对象的默认构造形式：
queue(const queue &que);//拷贝构造函数
*/
void test01()
{
	queue<int> q;
	queue<int> q2(q);
}

/*  queue存取、插入和删除操作

push(elem);//往队尾添加元素
pop();//从队头移除第一个元素
back();//返回最后一个元素
front();//返回第一个元素
*/
void test02()
{
	queue<int> q;
	q.push(1);
	q.push(2);
	q.push(3);
	q.push(4);
	q.push(5);
	q.push(6);
	quePrint(q);

	cout << q.back() << " " << q.front() << endl;
}

/*  queue赋值操作

queue&operator=(const queue &que);//重载等号操作符
*/
void test03()
{
	queue<int> q;
	queue<int> q2;
	q2 = q;
}

/*  queue大小操作

empty();//判断队列是否为空
size();//返回队列的大小
*/
void test04()
{
	queue<int> q;
	if (q.empty())
	{
		cout << "空的" << endl;
	}
	cout << "q.size() = " << q.size() << endl;
}

int main(int argc, char **argv)
{
	test04();

	getchar();
	return 0;
}