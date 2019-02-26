#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

void test()
{
	// ���ݽṹ�������ģ�����16���ֽھʹӶ�������ռ�
	// ������
	string::iterator it;
	it++;
	it--;
	it + 2;
	// ˫�����
}

void test01()
{
	string str = "Hello";
	for (string::iterator it = str.begin(); it != str.end(); it++)
	{
		cout << *it;
	}
}

/*  string ���캯��

string();//����һ���յ��ַ��� ����: string str;
string(const string& str);//ʹ��һ��string�����ʼ����һ��string����
string(const char* s);//ʹ���ַ���s��ʼ��
string(int n, char c);//ʹ��n���ַ�c��ʼ��
*/
void test02()
{
	// ����ģ�棬���ǲ�Ҫ<>������Ϊȡ�˱���
	string s1;
	string s2(s1);
	string s3("hello");
	string s4(5, 'a');
}

/*  string������ֵ����

string&operator=(const char* s);//char*�����ַ��� ��ֵ����ǰ���ַ���
string&operator=(const string&s);//���ַ���s������ǰ���ַ���
string&operator=(char c);//�ַ���ֵ����ǰ���ַ���
string& assign(const char *s);//���ַ���s������ǰ���ַ���
string& assign(const char *s, int n);//���ַ���s��ǰn���ַ�������ǰ���ַ���
string& assign(const string &s);//���ַ���s������ǰ�ַ���
string& assign(int n, char c);//��n���ַ�c������ǰ�ַ���
string& assign(const string &s, int start, int n);//��s��start��ʼn���ַ���ֵ���ַ���,��s=hello,��ôn=3,start=1����ô��hel�д�e��ʼ��ֵ3-1���ַ�
*/
void test03()
{
	string s = "hello";
	string s2 = s;
	s = 'c';

	s2.assign("world"); // ��ͬ�� s2 = "world";
	cout << s2 << endl;
	s2.assign("hello world!", 5);
	cout << s2 << endl;
	s2.assign(s); // ��ͬ�� s2 = s;
	cout << s2 << endl;
	s2.assign(5, 'x');
	cout << s2 << endl;
	s2.assign(string("hello world!"), 2, 5);
	cout << s2 << endl;
}

/*  string��ȡ�ַ�����

char&operator[](int n); //ͨ��[]��ʽȡ�ַ�
char& at(int n);        //ͨ��at������ȡ�ַ�
*/
void test04()
{
	string s = "hello";
	cout << s[1] << endl;
	cout << s.at(1) << endl;

	//[]��at������
	//[]����Խ�粻���쳣��ֱ�ӹ�
	//atԽ�����쳣
}

/*  stringƴ�Ӳ���

string& operator+=(const string& str);//����+=������
string& operator+=(const char* str);//����+=������
string& operator+=(const char c);//����+=������
string& append(const char *s);//���ַ���s���ӵ���ǰ�ַ�����β
string& append(const char *s, int n);//���ַ���s��ǰn���ַ����ӵ���ǰ�ַ�����β
string& append(const string& s);//ͬoperator+=()
string& append(const string& s, int pos, int n);//���ַ���s�д�pos��ʼ��n���ַ����ӵ���ǰ�ַ�����β
string& append(int n, char c);//�ڵ�ǰ�ַ�����β���n���ַ�c
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

/*  string���Һ��滻

int find(const string& str, int pos = 0) const; //����str��һ�γ���λ��,��pos��ʼ����
int find(const char* s, int pos = 0) const;  //����s��һ�γ���λ��,��pos��ʼ����
int find(const char* s, int pos, int n) const;  //��posλ�ò���s��ǰn���ַ���һ��λ��
int find(const char c, int pos = 0) const;  //�����ַ�c��һ�γ���λ��
int rfind(const string& str, int pos = npos) const;//����str���һ��λ��,��pos��ʼ����
int rfind(const char* s, int pos = npos) const;//����s���һ�γ���λ��,��pos��ʼ����
int rfind(const char* s, int pos, int n) const;//��pos����s��ǰn���ַ����һ��λ��
int rfind(const char c, int pos = 0) const; //�����ַ�c���һ�γ���λ��
string& replace(int pos, int n, const string& str); //�滻��pos��ʼn���ַ�Ϊ�ַ���str
string& replace(int pos, int n, const char* s); //�滻��pos��ʼ��n���ַ�Ϊ�ַ���s
*/
void test06()
{
	string s = "abcdefghelloworld";

	cout << s.find("he") << endl; // 7
	cout << s.find('d') << endl;  // 3
	cout << s.rfind('d') << endl; // 16, �����Ǵ��ҵ��󣬵��������Ǵ�����

	cout << s.find("kkk") << endl; // ���Ƿ���-1

	s.replace(2, 3, "AAAAB"); // ��2��λ�ã��±��0��ʼ�����滻4���ַ�
	// ���û��4���ַ����滻����ôʣ����ַ��ᱻ����,�������4���ַ���Ҳ����뵽�ַ�����
	cout << s << endl;
}

/*  string�Ƚϲ���

compare������>ʱ���� 1��<ʱ���� -1��==ʱ���� 0��
�Ƚ����ִ�Сд���Ƚ�ʱ�ο��ֵ�˳����Խǰ���ԽС��
��д��A��Сд��aС��

int compare(conststring& s) const;//���ַ���s�Ƚ�
int compare(constchar *s) const;//���ַ���s�Ƚ�
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

/*  string�Ӵ�

string substr(int pos = 0, int n = npos) const;//������pos��ʼ��n���ַ���ɵ��ַ���
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

/*  string�����ɾ������

string& insert(int pos, const char* s); //�����ַ���
string& insert(int pos, const string& str); //�����ַ���
string& insert(int pos, int n, char c);//��ָ��λ�ò���n���ַ�c
string& erase(int pos, int n = npos);//ɾ����Pos��ʼ��n���ַ�
*/
void test09()
{
	string s = "aaaaa";
	s.insert(3, "AAAA"); // ��3��λ�ò����ַ���"AAAA"
	cout << s << endl;

	s.insert(3, 5, 'M'); // ��3��λ�ò���5��M�ַ�
	cout << s << endl;

	s.erase(2, 3);
	cout << s << endl;
}

/*  string��c-style�ַ���ת��  */
void test10()
{
	//string ת char*
	string str = "dejan";
	const char* cstr = str.c_str();

	//char* ת string 
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