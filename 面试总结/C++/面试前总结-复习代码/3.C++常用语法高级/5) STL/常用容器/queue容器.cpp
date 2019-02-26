#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<queue> // ����, �Ƚ��ȳ�
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

/*  queue���캯��

queue<T> queT;//queue����ģ����ʵ�֣�queue�����Ĭ�Ϲ�����ʽ��
queue(const queue &que);//�������캯��
*/
void test01()
{
	queue<int> q;
	queue<int> q2(q);
}

/*  queue��ȡ�������ɾ������

push(elem);//����β���Ԫ��
pop();//�Ӷ�ͷ�Ƴ���һ��Ԫ��
back();//�������һ��Ԫ��
front();//���ص�һ��Ԫ��
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

/*  queue��ֵ����

queue&operator=(const queue &que);//���صȺŲ�����
*/
void test03()
{
	queue<int> q;
	queue<int> q2;
	q2 = q;
}

/*  queue��С����

empty();//�ж϶����Ƿ�Ϊ��
size();//���ض��еĴ�С
*/
void test04()
{
	queue<int> q;
	if (q.empty())
	{
		cout << "�յ�" << endl;
	}
	cout << "q.size() = " << q.size() << endl;
}

int main(int argc, char **argv)
{
	test04();

	getchar();
	return 0;
}