#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<list> // ˫������
using namespace std;

void test()
{
	list<int> mylist;

	for (int i = 0; i < 10; i++)
	{
		mylist.push_back(i);
	}

	// ֤����ѭ����  vs 2015 2017   _Myhead()
	list<int>::_Nodeptr node = mylist._Myhead()->_Next;//��ȡͷ�ڵ����һ���ڵ�
	for (int i = 0; i < mylist._Mysize() * 2; i++) // 2015 2017 _Mysize()
	{
		cout << "node:" << node->_Myval << endl; // ��ȡ����
		node = node->_Next;
		// ����ͷ�ڵ�
		if (node == mylist._Myhead())
		{
			node = node->_Next;
		}
	}

	// ֤����˫��
	list<int>::iterator it;
	it++;
	it--;

	//it + 2; error: �������
}

void lisPrint(const list<int> &mylist)
{
	for (list<int>::const_iterator it = mylist.begin(); it != mylist.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

/*  list���캯��

list<T> lstT;//list���ò���ģ����ʵ��,�����Ĭ�Ϲ�����ʽ��
list(beg,end);//���캯����[beg, end)�����е�Ԫ�ؿ���������
list(n,elem);//���캯����n��elem����������
list(const list &lst);//�������캯����
*/
void test01()
{
	int arr[] = { 1, 2, 3, 4, 5, 6 };
	list<int> mylist(arr, arr + sizeof(arr) / sizeof(int));
	lisPrint(mylist);

	list<int> mylist2(5, 8);
	lisPrint(mylist2);

	list<int> mylist3(mylist.begin(), mylist.end());
	lisPrint(mylist3);

	list<int> mylist5(mylist3);
	lisPrint(mylist5);
}

/*  list����Ԫ�ز����ɾ������

push_back(elem);//������β������һ��Ԫ��
pop_back();//ɾ�����������һ��Ԫ��
push_front(elem);//��������ͷ����һ��Ԫ��
pop_front();//��������ͷ�Ƴ���һ��Ԫ��
insert(pos,elem);//��posλ�ò�elemԪ�صĿ��������������ݵ�λ�á�
insert(pos,n,elem);//��posλ�ò���n��elem���ݣ��޷���ֵ��
insert(pos,beg,end);//��posλ�ò���[beg,end)��������ݣ��޷���ֵ��
clear();//�Ƴ���������������
erase(beg,end);//ɾ��[beg,end)��������ݣ�������һ�����ݵ�λ�á�
erase(pos);//ɾ��posλ�õ����ݣ�������һ�����ݵ�λ�á�
remove(elem);//ɾ��������������elemֵƥ���Ԫ�ء�
*/
struct Mycompare
{
	bool operator()(int val)
	{
		return val > 6;
	}
};
void test02()
{
	list<int> mylist(5, 8);
	mylist.push_front(9);
	mylist.push_back(5);
	lisPrint(mylist); // 9 8 8 8 8 8 5

	mylist.pop_front();
	mylist.pop_back();
	lisPrint(mylist); // 8 8 8 8 8

	list<int>::iterator it = mylist.begin();
	mylist.insert(++it, 7);
	lisPrint(mylist); // 8 7 8 8 8 8

	mylist.insert(it, 3, 6);
	lisPrint(mylist); // 8 7 6 6 6 8 8 8 8

	list<int> mylist2;
	mylist2.insert(mylist2.begin(), mylist.begin(), mylist.end());
	lisPrint(mylist); // 8 7 6 6 6 8 8 8 8

	mylist.erase(++mylist.begin());
	lisPrint(mylist); // 8 6 6 6 8 8 8 8

	mylist.erase(mylist.begin(), mylist.end());
	lisPrint(mylist); // ��

	lisPrint(mylist2); // 8 7 6 6 6 8 8 8 8
	mylist2.remove(7);
	lisPrint(mylist2); // 8 6 6 6 8 8 8 8

	mylist2.remove_if(Mycompare()); // ν��
	lisPrint(mylist2); // 6 6 6
}

/*  list��С����

size();//����������Ԫ�صĸ���
empty();//�ж������Ƿ�Ϊ��
resize(num);//����ָ�������ĳ���Ϊnum���������䳤������Ĭ��ֵ�����λ�á����������̣���ĩβ�����������ȵ�Ԫ�ر�ɾ����
resize(num, elem);//����ָ�������ĳ���Ϊnum���������䳤������elemֵ�����λ�á����������̣���ĩβ�����������ȵ�Ԫ�ر�ɾ����
*/
void test03()
{
	list<int> mylist;
	if (mylist.empty())
	{
		cout << "�յ�" << endl;
	}
	cout << "mylist.size() = "<< mylist.size() << endl;
	mylist.resize(5, 8);
	lisPrint(mylist);
}

/*  list��ֵ����

assign(beg, end);//��[beg, end)�����е����ݿ�����ֵ������
assign(n, elem);//��n��elem������ֵ������
list&operator=(const list &lst);//���صȺŲ�����
swap(lst);//��lst�뱾���Ԫ�ػ�����
*/
void test04()
{
	list<int> mylist;
	int arr[] = { 1,2,3,4,5 };
	mylist.assign(arr, arr + sizeof(arr) / sizeof(int));
	lisPrint(mylist); // 1 2 3 4 5

	mylist.assign(6, 6);
	lisPrint(mylist); // 6 6 6 6 6 6

	list<int> mylist2;
	mylist2 = mylist;
	lisPrint(mylist2); // 6 6 6 6 6 6

	mylist2.assign(6, 8);
	mylist2.swap(mylist);
	lisPrint(mylist); // 8 8 8 8 8 8
}

/*  list���ݵĴ�ȡ

front();//���ص�һ��Ԫ�ء�
back();//�������һ��Ԫ�ء�
*/
void test05()
{
	int arr[] = { 1,2,3,4,5 };
	list<int> mylist(arr, arr + sizeof(arr) / sizeof(int));
	lisPrint(mylist);

	cout << mylist.front() << " " << mylist.back();
}

/*  list��ת����

reverse();//��ת��������lst����1,3,5Ԫ�أ����д˷�����lst�Ͱ���5,3,1Ԫ�ء�
sort(); //list����
*/
struct DESC
{
	bool operator()(int v1, int v2)
	{
		return v1 > v2;
	}
};
void test06()
{
	int arr[] = { 8, 1, 9, 2, 3, 5 };
	list<int> mylist(arr, arr + sizeof(arr) / sizeof(int));

	mylist.reverse();
	lisPrint(mylist);

	mylist.sort();
	lisPrint(mylist);  // ASC

	mylist.sort(DESC()); // ��Ԫν��
	lisPrint(mylist);  // DESC
}

int main(int argc, char **argv)
{
	test06();

	getchar();
	return 0;
}