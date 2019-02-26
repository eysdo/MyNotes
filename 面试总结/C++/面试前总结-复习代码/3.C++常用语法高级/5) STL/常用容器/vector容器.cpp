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
	//�����Ĵ洢�ռ䣬
	//˫�����
}

void vecPrint(const vector<int> &vec)
{
	for (vector<int>::const_iterator it = vec.begin(); it != vec.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

/*  vector���캯��

vector<T> v; //����ģ��ʵ����ʵ�֣�Ĭ�Ϲ��캯��
vector(v.begin(), v.end());//��v[begin(), end())�����е�Ԫ�ؿ���������
vector(n, elem);//���캯����n��elem����������
vector(const vector &vec);//�������캯����
*/
void test01()
{
	int arr[] = { 2, 4, 1, 3, 9 };
	vector<int> v(arr, arr + sizeof(arr) / sizeof(int));
	vecPrint(v);

	vector<int> v2(10, 6);
	vecPrint(v2);
}

/*  vector���ø�ֵ����

assign(beg, end);//��[beg, end)�����е����ݿ�����ֵ������
assign(n, elem);//��n��elem������ֵ������
vector& operator=(const vector  &vec);//���صȺŲ�����
swap(vec);// ��vec�뱾���Ԫ�ػ�����
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

	v.swap(v2); // ����ָ��

	vecPrint(v);
	vecPrint(v2);
}

/*  vector��С����

size();//����������Ԫ�صĸ���
empty();//�ж������Ƿ�Ϊ��
resize(int num);//����ָ�������ĳ���Ϊnum���������䳤������Ĭ��ֵ�����λ�á����������̣���ĩβ�����������ȵ�Ԫ�ر�ɾ����
resize(int num, elem);//����ָ�������ĳ���Ϊnum���������䳤������elemֵ�����λ�á����������̣���ĩβ����������>�ȵ�Ԫ�ر�ɾ����
capacity();//����������
reserve(int len);//����Ԥ��len��Ԫ�س��ȣ�Ԥ��λ�ò���ʼ����Ԫ�ز��ɷ��ʡ�
*/
void test03()
{
	vector<int> v(8, 6);
	cout << "v.size = " << v.size() << endl;

	v.resize(5); // ����ָ�������ĳ��ȣ�ֵ���� 6
	cout << "v.size = " << v.size() << endl;
	vecPrint(v);

	v.reserve(20);

	vecPrint(v);
	cout << "v.size:" << v.size() << endl;
	cout << v.capacity() << endl;
}

/*  vector���ݴ�ȡ����

at(int idx); //��������idx��ָ�����ݣ����idxԽ�磬�׳�out_of_range�쳣��
operator[];//��������idx��ָ�����ݣ�Խ��ʱ������ֱ�ӱ���
front();//���������е�һ������Ԫ��
back();//�������������һ������Ԫ��
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

/*  vector�����ɾ������

insert(const_iterator pos, int count,ele);//������ָ��λ��pos����count��Ԫ��ele.
push_back(ele); //β������Ԫ��ele
pop_back();//ɾ�����һ��Ԫ��
erase(const_iterator start, const_iterator end);//ɾ����������start��end֮���Ԫ��
erase(const_iterator pos);//ɾ��������ָ���Ԫ��
clear();//ɾ������������Ԫ��
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