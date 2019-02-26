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
	//it + 2; error: �������
	// ˫��
	// ���ݽṹ�Ƕ�����
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

/*  set���캯��

set<T> st;//setĬ�Ϲ��캯����
mulitset<T> mst; //multisetĬ�Ϲ��캯��:
set(const set &st);//�������캯��
*/
void test01()
{
	set<int> st;
	multiset<int> mset;
	set<int> st1(st);
}

/*  set��ֵ����

set&operator=(const set &st);//���صȺŲ�����
swap(st);//����������������
*/
void test02()
{
	set<int> st;
	set<int> st2;
	st2 = st;
}

/*  set��С����

size();//����������Ԫ�ص���Ŀ
empty();//�ж������Ƿ�Ϊ��
*/
void test03()
{
	set<int> st;
	if (st.empty())
	{
		cout << "�յ�" << endl;
	}
	cout << "st.size() = " << st.size() << endl;
}

/*  set�����ɾ������

insert(elem);//�������в���Ԫ�ء�
clear();//�������Ԫ��
erase(pos);//ɾ��pos��������ָ��Ԫ�أ�������һ��Ԫ�صĵ�������
erase(beg, end);//ɾ������[beg,end)������Ԫ�� ��������һ��Ԫ�صĵ�������
erase(elem);//ɾ��������ֵΪelem��Ԫ�ء�
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
	setPrint(st); // ��
}

/*  set���Ҳ���

find(key);//���Ҽ�key�Ƿ����,�����ڣ����ظü���Ԫ�صĵ��������������ڣ�����set.end();
1.	count(key);//���Ҽ�key��Ԫ�ظ���
2.	lower_bound(keyElem);//���ص�һ��key>=keyElemԪ�صĵ�������
3.	upper_bound(keyElem);//���ص�һ��key>keyElemԪ�صĵ�������
equal_range(keyElem);//����������key��keyElem��ȵ������޵�������������
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
		cout << "�Ҳ���!" << endl;
	}
	else
	{
		cout << "�ҵ���ֵ��: " << *it << endl;
	}
	cout << "-----------------------------" << endl;
	it = s.lower_bound(2);
	if (it == s.end())
	{
		cout << "�Ҳ���!" << endl;
	}
	else
	{
		cout << "�ҵ���ֵ��: " << *it << endl;
	}
	it = s.upper_bound(2);
	if (it == s.end())
	{
		cout << "�Ҳ���!" << endl;
	}
	else
	{
		cout << "�ҵ���ֵ��: " << *it << endl;
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

/* set�����������ֵ */
void test06()
{
	set<int> s;
	pair<set<int>::iterator, bool> ret = s.insert(10);
	if (ret.second)
	{
		cout << "����ɹ�:" << *ret.first << endl;
	}
	else
	{
		cout << "����ʧ��:" << *ret.first << endl;
	}
	s.insert(20);
	ret = s.insert(10);
	if (ret.second)
	{
		cout << "����ɹ�:" << *ret.first << endl;
	}
	else
	{
		cout << "����ʧ��:" << *ret.first << endl;
	}
}

/* set�Ӵ�С(����) */
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

/* ����(pair) */
void test08()
{
	// ��һ�ַ�������һ������
	pair<string, int> pair1(string("name"), 20);
	cout << pair1.first << " ";  // ����pair��һ��ֵ
	cout << pair1.second << endl; // ����pair�ڶ���ֵ

	// �ڶ���
	pair<string, int> pair2 = make_pair("name", 30);
	cout << pair2.first << " ";
	cout << pair2.second << endl;

	// pair = ��ֵ
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