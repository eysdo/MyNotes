#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
using namespace std;


void test()
{
	vector<int>::iterator it;
	it--;
	it++;
	it + 2;
	//连续的存储空间，
	//双向，随机
}

void vecPrint(const vector<int> &vec)
{
	for (vector<int>::const_iterator it = vec.begin(); it != vec.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

/*  vector构造函数

vector<T> v; //采用模板实现类实现，默认构造函数
vector(v.begin(), v.end());//将v[begin(), end())区间中的元素拷贝给本身。
vector(n, elem);//构造函数将n个elem拷贝给本身。
vector(const vector &vec);//拷贝构造函数。
*/
void test01()
{
	int arr[] = { 2, 4, 1, 3, 9 };
	vector<int> v(arr, arr + sizeof(arr) / sizeof(int));
	vecPrint(v);

	vector<int> v2(10, 6);
	vecPrint(v2);
}

/*  vector常用赋值操作

assign(beg, end);//将[beg, end)区间中的数据拷贝赋值给本身。
assign(n, elem);//将n个elem拷贝赋值给本身。
vector& operator=(const vector  &vec);//重载等号操作符
swap(vec);// 将vec与本身的元素互换。
*/
void test02()
{
	vector<int> v;
	vector<int> v2;
	v.assign(10, 6);
	v2.assign(10, 8);

	vecPrint(v);
	vecPrint(v2);

	cout << "---------------------------" << endl;

	v.swap(v2); // 交换指针

	vecPrint(v);
	vecPrint(v2);
}

/*  vector大小操作

size();//返回容器中元素的个数
empty();//判断容器是否为空
resize(int num);//重新指定容器的长度为num，若容器变长，则以默认值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。
resize(int num, elem);//重新指定容器的长度为num，若容器变长，则以elem值填充新位置。如果容器变短，则末尾超出容器长>度的元素被删除。
capacity();//容器的容量
reserve(int len);//容器预留len个元素长度，预留位置不初始化，元素不可访问。
*/
void test03()
{
	vector<int> v(8, 6);
	cout << "v.size = " << v.size() << endl;

	v.resize(5); // 重新指定容器的长度，值还是 6
	cout << "v.size = " << v.size() << endl;
	vecPrint(v);

	v.reserve(20);

	vecPrint(v);
	cout << "v.size:" << v.size() << endl;
	cout << v.capacity() << endl;
}

/*  vector数据存取操作

at(int idx); //返回索引idx所指的数据，如果idx越界，抛出out_of_range异常。
operator[];//返回索引idx所指的数据，越界时，运行直接报错
front();//返回容器中第一个数据元素
back();//返回容器中最后一个数据元素
*/
void test04()
{
	int arr[] = { 1,2,5,6,7,8,9 };
	vector<int> v(arr, arr + sizeof(arr) / sizeof(int));

	cout << v.front() << endl;
	cout << v.back() << endl;

	v.front() = 100;
	v.back() = 200;

	for (int i = v.size()-1; i >= 0 ; i--)
	{
		cout << v[i] << " ";
	}
	cout << endl;
}

/*  vector插入和删除操作

insert(const_iterator pos, int count,ele);//迭代器指向位置pos插入count个元素ele.
push_back(ele); //尾部插入元素ele
pop_back();//删除最后一个元素
erase(const_iterator start, const_iterator end);//删除迭代器从start到end之间的元素
erase(const_iterator pos);//删除迭代器指向的元素
clear();//删除容器中所有元素
*/
void test05()
{
	vector<int> v(6, 2);

	v.insert(v.begin() + 2, 100);
	vecPrint(v);

	v.pop_back();
	vecPrint(v);

	v.erase(v.begin());
	vecPrint(v);

	v.erase(v.begin(), v.end());
	vecPrint(v);
}


int main(int argc, char **argv)
{
	test05();

	getchar();
	return 0;
}