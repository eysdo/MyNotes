#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<map>  // 映射表
#include<string>
using namespace std;

void test()
{
	//数据结构是二叉树
	map<int, int>::iterator it;
	it++;
	it--;
	//it + 2; // 不可随机
	//双向
}

template<class T>
void mapPrint(T &m)
{
	for (T::iterator it = m.begin(); it != m.end(); it++)
	{
		cout << "key:" << it->first << " value:" << it->second << endl;
	}
}

/*  map构造函数

map<T1, T2> mapTT;//map默认构造函数:
map(const map &mp);//拷贝构造函数
*/
void test01()
{
	map<string, string> m;
	map<string, string> m2(m);
}

/*  map赋值操作

map&operator=(const map &mp);//重载等号操作符
swap(mp);//交换两个集合容器
*/
void test02()
{
	map<string, string> m;
	map<string, string> m2;
	m = m2;
	m.swap(m2);
}

/*  map大小操作

size();//返回容器中元素的数目
empty();//判断容器是否为空
*/
void test03()
{
	map<string, string> m;
	if (m.empty())
	{
		cout << "空的" << endl;
	}
	cout << "m.size() = " << m.size() << endl;
}

/*  map插入数据元素操作

map.insert(...); //往容器插入元素，返回pair<iterator,bool>
map<int, string> mapStu;
// 第一种 通过pair的方式插入对象
mapStu.insert(pair<int, string>(3, "小张"));
// 第二种 通过pair的方式插入对象
mapStu.inset(make_pair(-1, "校长"));
// 第三种 通过value_type的方式插入对象
mapStu.insert(map<int, string>::value_type(1, "小李"));
// 第四种 通过数组的方式插入值
mapStu[3] = "小刘";
mapStu[5] = "小王";
*/
void test04()
{
	// 1.第一种插入数据
	map<string, string> m;
	m.insert(pair<string, string>("name", "Dejan"));
	mapPrint(m);

	// 2.第二种插入数据
	map<int, string> m2;
	m2.insert(make_pair(0, "Dejan"));
	mapPrint(m2);

	// 3.第三种插入数据
	cout << "-------------------" << endl;
	m2.insert(map<int, string>::value_type(1, "AAAAA"));
	mapPrint(m2);

	// 4.第四种插入数据
	cout << "-------------------" << endl;
	m2[2] = "BBBBB";
	mapPrint(m2);

	cout << "m.size() = " << m2.size() << endl;
}

/*  map删除操作

clear();//删除所有元素
erase(pos);//删除pos迭代器所指的元素，返回下一个元素的迭代器。
erase(beg,end);//删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
erase(keyElem);//删除容器中key为keyElem的对组。
*/
void test05()
{
	map<int, string> m;
	m.insert(make_pair(0, "AAAAA"));
	m.insert(make_pair(1, "BBBBB"));
	m.insert(make_pair(2, "CCCCC"));
	m.insert(make_pair(3, "DDDDD"));
	m.insert(make_pair(4, "EEEEE"));
	mapPrint(m);
	cout << "----------------" << endl;
	m.erase(2);
	mapPrint(m);
	cout << "----------------" << endl;
	m.erase(m.begin());
	mapPrint(m);
	cout << "----------------" << endl;
	m.erase(m.begin(), m.end());
	mapPrint(m);
}

/*  map查找操作

find(key);//查找键key是否存在,若存在，返回该键的元素的迭代器；/若不存在，返回map.end();
count(keyElem);//返回容器中key为keyElem的对组个数。对map来说，要么是0，要么是1。对multimap来说，值可能大于1。
lower_bound(keyElem);//返回第一个key>=keyElem元素的迭代器。
upper_bound(keyElem);//返回第一个key>keyElem元素的迭代器。
equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器。
*/
void test06()
{
	map<int, string> m;
	m[0] = "AAAAA";
	m[1] = "BBBBB";
	m[2] = "CCCCC";
	m[3] = "DDDDD";
	m[4] = "EEEEE";
	map<int, string>::iterator it = m.find(20);
	if (it == m.end())
	{
		cout << "找不到!" << endl;
	}
	else
	{
		cout <<"找到的值: "<< it->first << " " << it->second << endl;
	}
	
	multimap<int, string> m2;
	m2.insert(make_pair(1, "AAAAA"));
	m2.insert(make_pair(2, "AAAAA"));
	m2.insert(make_pair(2, "AAAAA"));
	m2.insert(make_pair(2, "AAAAA"));
	m2.insert(make_pair(1, "AAAAA"));
	cout << m2.count(2) << endl;

	it = m.lower_bound(8);
	if (it == m.end())
	{
		cout << "找不到!" << endl;
	}
	else
	{
		cout << "找到的值: " << it->first << " " << it->second << endl;
	}

	it = m.upper_bound(3);
	if (it == m.end())
	{
		cout << "找不到!" << endl;
	}
	else
	{
		cout << "找到的值: " << it->first << " " << it->second << endl;
	}
	cout << "-------------------------------" << endl;
	pair<map<int, string>::iterator, map<int, string>::iterator> ret = m.equal_range(3);
	if (ret.first != m.end())
	{
		cout << "key:" << ret.first->first << " value:" << ret.first->second << endl;
	}
	if (ret.second != m.end())
	{
		cout << "key:" << ret.second->first << " value:" << ret.second->second << endl;
	}
}

int main(int argc, char **argv)
{
	test06();

	getchar();
	return 0;
}