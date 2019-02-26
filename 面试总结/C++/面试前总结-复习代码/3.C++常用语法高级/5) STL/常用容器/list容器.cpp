#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<list> // 双向链表
using namespace std;

void test()
{
	list<int> mylist;

	for (int i = 0; i < 10; i++)
	{
		mylist.push_back(i);
	}

	// 证明是循环的  vs 2015 2017   _Myhead()
	list<int>::_Nodeptr node = mylist._Myhead()->_Next;//获取头节点的下一个节点
	for (int i = 0; i < mylist._Mysize() * 2; i++) // 2015 2017 _Mysize()
	{
		cout << "node:" << node->_Myval << endl; // 获取数据
		node = node->_Next;
		// 跳过头节点
		if (node == mylist._Myhead())
		{
			node = node->_Next;
		}
	}

	// 证明是双向
	list<int>::iterator it;
	it++;
	it--;

	//it + 2; error: 不是随机
}

void lisPrint(const list<int> &mylist)
{
	for (list<int>::const_iterator it = mylist.begin(); it != mylist.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

/*  list构造函数

list<T> lstT;//list采用采用模板类实现,对象的默认构造形式：
list(beg,end);//构造函数将[beg, end)区间中的元素拷贝给本身。
list(n,elem);//构造函数将n个elem拷贝给本身。
list(const list &lst);//拷贝构造函数。
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

/*  list数据元素插入和删除操作

push_back(elem);//在容器尾部加入一个元素
pop_back();//删除容器中最后一个元素
push_front(elem);//在容器开头插入一个元素
pop_front();//从容器开头移除第一个元素
insert(pos,elem);//在pos位置插elem元素的拷贝，返回新数据的位置。
insert(pos,n,elem);//在pos位置插入n个elem数据，无返回值。
insert(pos,beg,end);//在pos位置插入[beg,end)区间的数据，无返回值。
clear();//移除容器的所有数据
erase(beg,end);//删除[beg,end)区间的数据，返回下一个数据的位置。
erase(pos);//删除pos位置的数据，返回下一个数据的位置。
remove(elem);//删除容器中所有与elem值匹配的元素。
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
	lisPrint(mylist); // 空

	lisPrint(mylist2); // 8 7 6 6 6 8 8 8 8
	mylist2.remove(7);
	lisPrint(mylist2); // 8 6 6 6 8 8 8 8

	mylist2.remove_if(Mycompare()); // 谓词
	lisPrint(mylist2); // 6 6 6
}

/*  list大小操作

size();//返回容器中元素的个数
empty();//判断容器是否为空
resize(num);//重新指定容器的长度为num，若容器变长，则以默认值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。
resize(num, elem);//重新指定容器的长度为num，若容器变长，则以elem值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。
*/
void test03()
{
	list<int> mylist;
	if (mylist.empty())
	{
		cout << "空的" << endl;
	}
	cout << "mylist.size() = "<< mylist.size() << endl;
	mylist.resize(5, 8);
	lisPrint(mylist);
}

/*  list赋值操作

assign(beg, end);//将[beg, end)区间中的数据拷贝赋值给本身。
assign(n, elem);//将n个elem拷贝赋值给本身。
list&operator=(const list &lst);//重载等号操作符
swap(lst);//将lst与本身的元素互换。
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

/*  list数据的存取

front();//返回第一个元素。
back();//返回最后一个元素。
*/
void test05()
{
	int arr[] = { 1,2,3,4,5 };
	list<int> mylist(arr, arr + sizeof(arr) / sizeof(int));
	lisPrint(mylist);

	cout << mylist.front() << " " << mylist.back();
}

/*  list反转排序

reverse();//反转链表，比如lst包含1,3,5元素，运行此方法后，lst就包含5,3,1元素。
sort(); //list排序
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

	mylist.sort(DESC()); // 二元谓词
	lisPrint(mylist);  // DESC
}

int main(int argc, char **argv)
{
	test06();

	getchar();
	return 0;
}