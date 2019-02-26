#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<set>
#include<string>
using namespace std;

void test()
{
	set<int>::iterator it;
	it--;
	it++;
	//it + 2; error: 不是随机
	// 双向
	// 数据结构是二叉树
}

void setPrint(const set<int> &s)
{
	for (set<int>::iterator it = s.begin(); it != s.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}
void msetPrint(const multiset<int> &s)
{
	for (multiset<int>::iterator it = s.begin(); it != s.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

/*  set构造函数

set<T> st;//set默认构造函数：
mulitset<T> mst; //multiset默认构造函数:
set(const set &st);//拷贝构造函数
*/
void test01()
{
	set<int> st;
	multiset<int> mset;
	set<int> st1(st);
}

/*  set赋值操作

set&operator=(const set &st);//重载等号操作符
swap(st);//交换两个集合容器
*/
void test02()
{
	set<int> st;
	set<int> st2;
	st2 = st;
}

/*  set大小操作

size();//返回容器中元素的数目
empty();//判断容器是否为空
*/
void test03()
{
	set<int> st;
	if (st.empty())
	{
		cout << "空的" << endl;
	}
	cout << "st.size() = " << st.size() << endl;
}

/*  set插入和删除操作

insert(elem);//在容器中插入元素。
clear();//清除所有元素
erase(pos);//删除pos迭代器所指的元素，返回下一个元素的迭代器。
erase(beg, end);//删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
erase(elem);//删除容器中值为elem的元素。
*/
void test04()
{
	set<int> st;
	st.insert(2);
	st.insert(5);
	st.insert(1);
	st.insert(9);
	st.insert(8);
	setPrint(st); // 1 2 5 8 9

	set<int>::iterator it = st.begin();
	st.erase(++it);
	setPrint(st); // 1 5 8 9

	st.erase(5);
	setPrint(st); // 1 8 9

	st.erase(st.begin(), st.end());
	setPrint(st); // 空
}

/*  set查找操作

find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；若不存在，返回set.end();
1.	count(key);//查找键key的元素个数
2.	lower_bound(keyElem);//返回第一个key>=keyElem元素的迭代器。
3.	upper_bound(keyElem);//返回第一个key>keyElem元素的迭代器。
equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器。
*/
void test05()
{
	set<int> s;
	s.insert(1);
	s.insert(8);
	s.insert(3);
	s.insert(4);
	s.insert(6);
	s.insert(5);
	s.insert(5);
	set<int>::iterator it = s.find(2);
	if (it == s.end())
	{
		cout << "找不到!" << endl;
	}
	else
	{
		cout << "找到的值是: " << *it << endl;
	}
	cout << "-----------------------------" << endl;
	it = s.lower_bound(2);
	if (it == s.end())
	{
		cout << "找不到!" << endl;
	}
	else
	{
		cout << "找到的值是: " << *it << endl;
	}
	it = s.upper_bound(2);
	if (it == s.end())
	{
		cout << "找不到!" << endl;
	}
	else
	{
		cout << "找到的值是: " << *it << endl;
	}
	setPrint(s);
	cout << "-----------------------------" << endl;

	multiset<int> ms;
	ms.insert(1);
	ms.insert(2);
	ms.insert(2);
	ms.insert(2);
	ms.insert(1);
	msetPrint(ms);
	cout << "ms.count(2) = " << ms.count(2) << endl;
}

/* set插入操作返回值 */
void test06()
{
	set<int> s;
	pair<set<int>::iterator, bool> ret = s.insert(10);
	if (ret.second)
	{
		cout << "插入成功:" << *ret.first << endl;
	}
	else
	{
		cout << "插入失败:" << *ret.first << endl;
	}
	s.insert(20);
	ret = s.insert(10);
	if (ret.second)
	{
		cout << "插入成功:" << *ret.first << endl;
	}
	else
	{
		cout << "插入失败:" << *ret.first << endl;
	}
}

/* set从大到小(倒序) */
struct DESC
{
	bool operator()(int v1, int v2)
	{
		return v1 > v2;
	}
};
void test07()
{
	set<int, DESC> s;
	s.insert(1);
	s.insert(8);
	s.insert(3);
	s.insert(4);
	s.insert(6);
	s.insert(5);
	for (set<int, DESC>::iterator it = s.begin(); it != s.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

/* 对组(pair) */
void test08()
{
	// 第一种方法创建一个对组
	pair<string, int> pair1(string("name"), 20);
	cout << pair1.first << " ";  // 访问pair第一个值
	cout << pair1.second << endl; // 访问pair第二个值

	// 第二种
	pair<string, int> pair2 = make_pair("name", 30);
	cout << pair2.first << " ";
	cout << pair2.second << endl;

	// pair = 赋值
	pair<string, int> pair3 = pair2;
	cout << pair3.first << " ";
	cout << pair3.second << endl;
}

int main(int argc, char **argv)
{
	test08();

	getchar();
	return 0;
}