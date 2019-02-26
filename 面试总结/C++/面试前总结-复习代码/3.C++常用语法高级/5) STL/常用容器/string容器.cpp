#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

void test()
{
	// 数据结构：连续的，超过16个字节就从堆区申请空间
	// 迭代器
	string::iterator it;
	it++;
	it--;
	it + 2;
	// 双向，随机
}

void test01()
{
	string str = "Hello";
	for (string::iterator it = str.begin(); it != str.end(); it++)
	{
		cout << *it;
	}
}

/*  string 构造函数

string();//创建一个空的字符串 例如: string str;
string(const string& str);//使用一个string对象初始化另一个string对象
string(const char* s);//使用字符串s初始化
string(int n, char c);//使用n个字符c初始化
*/
void test02()
{
	// 是类模版，但是不要<>，是因为取了别名
	string s1;
	string s2(s1);
	string s3("hello");
	string s4(5, 'a');
}

/*  string基本赋值操作

string&operator=(const char* s);//char*类型字符串 赋值给当前的字符串
string&operator=(const string&s);//把字符串s赋给当前的字符串
string&operator=(char c);//字符赋值给当前的字符串
string& assign(const char *s);//把字符串s赋给当前的字符串
string& assign(const char *s, int n);//把字符串s的前n个字符赋给当前的字符串
string& assign(const string &s);//把字符串s赋给当前字符串
string& assign(int n, char c);//用n个字符c赋给当前字符串
string& assign(const string &s, int start, int n);//将s从start开始n个字符赋值给字符串,如s=hello,那么n=3,start=1，那么是hel中从e开始赋值3-1个字符
*/
void test03()
{
	string s = "hello";
	string s2 = s;
	s = 'c';

	s2.assign("world"); // 等同于 s2 = "world";
	cout << s2 << endl;
	s2.assign("hello world!", 5);
	cout << s2 << endl;
	s2.assign(s); // 等同于 s2 = s;
	cout << s2 << endl;
	s2.assign(5, 'x');
	cout << s2 << endl;
	s2.assign(string("hello world!"), 2, 5);
	cout << s2 << endl;
}

/*  string存取字符操作

char&operator[](int n); //通过[]方式取字符
char& at(int n);        //通过at方法获取字符
*/
void test04()
{
	string s = "hello";
	cout << s[1] << endl;
	cout << s.at(1) << endl;

	//[]和at的区别：
	//[]访问越界不抛异常，直接挂
	//at越界抛异常
}

/*  string拼接操作

string& operator+=(const string& str);//重载+=操作符
string& operator+=(const char* str);//重载+=操作符
string& operator+=(const char c);//重载+=操作符
string& append(const char *s);//把字符串s连接到当前字符串结尾
string& append(const char *s, int n);//把字符串s的前n个字符连接到当前字符串结尾
string& append(const string& s);//同operator+=()
string& append(const string& s, int pos, int n);//把字符串s中从pos开始的n个字符连接到当前字符串结尾
string& append(int n, char c);//在当前字符串结尾添加n个字符c
*/
void test05()
{
	string s = "abc";
	s += string("def");
	s += "ghi";
	cout << s << endl;
	s.append(" hello");
	s.append(" 12345", 3);
	s.append(string(" world!"));
	s.append(string("string"), 1, 3);
	s.append(5, 'c');
	cout << s << endl;
}

/*  string查找和替换

int find(const string& str, int pos = 0) const; //查找str第一次出现位置,从pos开始查找
int find(const char* s, int pos = 0) const;  //查找s第一次出现位置,从pos开始查找
int find(const char* s, int pos, int n) const;  //从pos位置查找s的前n个字符第一次位置
int find(const char c, int pos = 0) const;  //查找字符c第一次出现位置
int rfind(const string& str, int pos = npos) const;//查找str最后一次位置,从pos开始查找
int rfind(const char* s, int pos = npos) const;//查找s最后一次出现位置,从pos开始查找
int rfind(const char* s, int pos, int n) const;//从pos查找s的前n个字符最后一次位置
int rfind(const char c, int pos = 0) const; //查找字符c最后一次出现位置
string& replace(int pos, int n, const string& str); //替换从pos开始n个字符为字符串str
string& replace(int pos, int n, const char* s); //替换从pos开始的n个字符为字符串s
*/
void test06()
{
	string s = "abcdefghelloworld";

	cout << s.find("he") << endl; // 7
	cout << s.find('d') << endl;  // 3
	cout << s.rfind('d') << endl; // 16, 查找是从右到左，但是数字是从左到右

	cout << s.find("kkk") << endl; // 不是返回-1

	s.replace(2, 3, "AAAAB"); // 从2的位置（下标从0开始），替换4个字符
	// 如果没有4个字符来替换，那么剩余的字符会被消除,如果大于4个字符，也会插入到字符串中
	cout << s << endl;
}

/*  string比较操作

compare函数在>时返回 1，<时返回 -1，==时返回 0。
比较区分大小写，比较时参考字典顺序，排越前面的越小。
大写的A比小写的a小。

int compare(conststring& s) const;//与字符串s比较
int compare(constchar *s) const;//与字符串s比较
*/
void test07()
{
	string s1 = "hello";
	string s2 = "hello";

	const char* s = "world";

	if (s1.compare(s2) == 0)
		cout << "s1 == s2" << endl;
	if (s1.compare(s) == 0)
		cout << "s1 == s" << endl;
}

/*  string子串

string substr(int pos = 0, int n = npos) const;//返回由pos开始的n个字符组成的字符串
*/
void test08()
{
	string emaill = "dejan@honey58.com";
	int pos = emaill.find('@');

	string username = emaill.substr(0, pos);
	cout << username << endl;

	string prex = emaill.substr(pos + 1);
	cout << prex << endl;
}

/*  string插入和删除操作

string& insert(int pos, const char* s); //插入字符串
string& insert(int pos, const string& str); //插入字符串
string& insert(int pos, int n, char c);//在指定位置插入n个字符c
string& erase(int pos, int n = npos);//删除从Pos开始的n个字符
*/
void test09()
{
	string s = "aaaaa";
	s.insert(3, "AAAA"); // 从3的位置插入字符串"AAAA"
	cout << s << endl;

	s.insert(3, 5, 'M'); // 从3的位置插入5个M字符
	cout << s << endl;

	s.erase(2, 3);
	cout << s << endl;
}

/*  string和c-style字符串转换  */
void test10()
{
	//string 转 char*
	string str = "dejan";
	const char* cstr = str.c_str();

	//char* 转 string 
	char* s = "dejan";
	string sstr(s);

	cout << cstr << "  " << sstr << endl;
}


int main(int argc, char **argv)
{
	test10();

	getchar();
	return 0;
}