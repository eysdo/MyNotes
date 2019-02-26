#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stack>  // ջ, �Ƚ����
using namespace std;

void staPrint(stack<int> sta)
{
	while (!sta.empty())
	{
		cout << sta.top();
		sta.pop();
	}
}

/*  stack���캯��

stack<T> stkT;//stack����ģ����ʵ�֣� stack�����Ĭ�Ϲ�����ʽ��
stack(const stack &stk);//�������캯��
*/
void test01()
{
	stack<int> s;
	stack<int> s2(s);
}

/*  stack��ֵ����

stack&operator=(const stack &stk);//���صȺŲ�����
*/
void test02()
{
	stack<int> s;
	stack<int> s2;

	s = s2;
}

/*  stack���ݴ�ȡ����

push(elem);//��ջ�����Ԫ��
pop();//��ջ���Ƴ���һ��Ԫ��
top();//����ջ��Ԫ��
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

	cout << "ջ��Ԫ��:" << s.top() << endl;
	s.pop();
	staPrint(s);
}

/*  stack��С����

empty();//�ж϶�ջ�Ƿ�Ϊ��
size();//���ض�ջ�Ĵ�С
*/
void test04()
{
	stack<int> s;
	if (s.empty())
	{
		cout << "�յ�" << endl;
	}
	cout << "s.size() = " << s.size() << endl;
}

int main(int argc, char **argv)
{
	test04();

	getchar();
	return 0;
}