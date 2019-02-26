#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stack>  // 栈, 先进后出
using namespace std;

void staPrint(stack<int> sta)
{
	while (!sta.empty())
	{
		cout << sta.top();
		sta.pop();
	}
}

/*  stack构造函数

stack<T> stkT;//stack采用模板类实现， stack对象的默认构造形式：
stack(const stack &stk);//拷贝构造函数
*/
void test01()
{
	stack<int> s;
	stack<int> s2(s);
}

/*  stack赋值操作

stack&operator=(const stack &stk);//重载等号操作符
*/
void test02()
{
	stack<int> s;
	stack<int> s2;

	s = s2;
}

/*  stack数据存取操作

push(elem);//向栈顶添加元素
pop();//从栈顶移除第一个元素
top();//返回栈顶元素
*/
void test03()
{
	stack<int> s;
	s.push(1);
	s.push(2);
	s.push(3);
	s.push(4);
	s.push(5);

	staPrint(s);

	cout << "栈顶元素:" << s.top() << endl;
	s.pop();
	staPrint(s);
}

/*  stack大小操作

empty();//判断堆栈是否为空
size();//返回堆栈的大小
*/
void test04()
{
	stack<int> s;
	if (s.empty())
	{
		cout << "空的" << endl;
	}
	cout << "s.size() = " << s.size() << endl;
}

int main(int argc, char **argv)
{
	test04();

	getchar();
	return 0;
}