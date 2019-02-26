#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<map>  // ӳ���
#include<string>
using namespace std;

void test()
{
	//���ݽṹ�Ƕ�����
	map<int, int>::iterator it;
	it++;
	it--;
	//it + 2; // �������
	//˫��
}

template<class T>
void mapPrint(T &m)
{
	for (T::iterator it = m.begin(); it != m.end(); it++)
	{
		cout << "key:" << it->first << " value:" << it->second << endl;
	}
}

/*  map���캯��

map<T1, T2> mapTT;//mapĬ�Ϲ��캯��:
map(const map &mp);//�������캯��
*/
void test01()
{
	map<string, string> m;
	map<string, string> m2(m);
}

/*  map��ֵ����

map&operator=(const map &mp);//���صȺŲ�����
swap(mp);//����������������
*/
void test02()
{
	map<string, string> m;
	map<string, string> m2;
	m = m2;
	m.swap(m2);
}

/*  map��С����

size();//����������Ԫ�ص���Ŀ
empty();//�ж������Ƿ�Ϊ��
*/
void test03()
{
	map<string, string> m;
	if (m.empty())
	{
		cout << "�յ�" << endl;
	}
	cout << "m.size() = " << m.size() << endl;
}

/*  map��������Ԫ�ز���

map.insert(...); //����������Ԫ�أ�����pair<iterator,bool>
map<int, string> mapStu;
// ��һ�� ͨ��pair�ķ�ʽ�������
mapStu.insert(pair<int, string>(3, "С��"));
// �ڶ��� ͨ��pair�ķ�ʽ�������
mapStu.inset(make_pair(-1, "У��"));
// ������ ͨ��value_type�ķ�ʽ�������
mapStu.insert(map<int, string>::value_type(1, "С��"));
// ������ ͨ������ķ�ʽ����ֵ
mapStu[3] = "С��";
mapStu[5] = "С��";
*/
void test04()
{
	// 1.��һ�ֲ�������
	map<string, string> m;
	m.insert(pair<string, string>("name", "Dejan"));
	mapPrint(m);

	// 2.�ڶ��ֲ�������
	map<int, string> m2;
	m2.insert(make_pair(0, "Dejan"));
	mapPrint(m2);

	// 3.�����ֲ�������
	cout << "-------------------" << endl;
	m2.insert(map<int, string>::value_type(1, "AAAAA"));
	mapPrint(m2);

	// 4.�����ֲ�������
	cout << "-------------------" << endl;
	m2[2] = "BBBBB";
	mapPrint(m2);

	cout << "m.size() = " << m2.size() << endl;
}

/*  mapɾ������

clear();//ɾ������Ԫ��
erase(pos);//ɾ��pos��������ָ��Ԫ�أ�������һ��Ԫ�صĵ�������
erase(beg,end);//ɾ������[beg,end)������Ԫ�� ��������һ��Ԫ�صĵ�������
erase(keyElem);//ɾ��������keyΪkeyElem�Ķ��顣
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

/*  map���Ҳ���

find(key);//���Ҽ�key�Ƿ����,�����ڣ����ظü���Ԫ�صĵ�������/�������ڣ�����map.end();
count(keyElem);//����������keyΪkeyElem�Ķ����������map��˵��Ҫô��0��Ҫô��1����multimap��˵��ֵ���ܴ���1��
lower_bound(keyElem);//���ص�һ��key>=keyElemԪ�صĵ�������
upper_bound(keyElem);//���ص�һ��key>keyElemԪ�صĵ�������
equal_range(keyElem);//����������key��keyElem��ȵ������޵�������������
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
		cout << "�Ҳ���!" << endl;
	}
	else
	{
		cout <<"�ҵ���ֵ: "<< it->first << " " << it->second << endl;
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
		cout << "�Ҳ���!" << endl;
	}
	else
	{
		cout << "�ҵ���ֵ: " << it->first << " " << it->second << endl;
	}

	it = m.upper_bound(3);
	if (it == m.end())
	{
		cout << "�Ҳ���!" << endl;
	}
	else
	{
		cout << "�ҵ���ֵ: " << it->first << " " << it->second << endl;
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