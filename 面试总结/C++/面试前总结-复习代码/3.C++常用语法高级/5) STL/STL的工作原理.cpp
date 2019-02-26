#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

// ����
template<class T>
class Dejan
{
public:
	typedef T* iterator; // ��װһ��ԭ��ָ��
	Dejan()
	{
		Capacity = 10;
		Size = 10;

		p = new T[Capacity];

		for (int i = 0; i < 10; i++)
		{
			p[i] = i + 1;
		}
	}

	// �����ṩ������

	// �ṩ��ʼλ�õĵ�����
	T* begin()
	{
		return p;
	}

	// �ṩ����λ�õĵ�����
	T* end()
	{
		return p + Size;
	}

public:
	T *p;
	int Capacity;
	int Size;
};

// �㷨
template<class T>
void Myforeach(T begin, T end)
{
	for (; begin != end; ++begin)
	{
		cout << *begin << " ";
	}
	cout << endl;
}

void test()
{
	// ʵ����һ����������һ������
	Dejan<int> dejan;

	// ��ȡ�����ṩ�ĵ�����
	// T*=dejan.begin();
	Dejan<int>::iterator begin = dejan.begin();
	// T*=dejan.end();
	Dejan<int>::iterator end = dejan.end();

	// �㷨ͨ����������ʹ�������е�����
	Myforeach(begin, end);
}

int main()
{
	test();

	system("pause");
	return 0;
}