#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<deque>
using namespace std;

void test()
{
	deque<int>::iterator it;
	it++;
	it--;
	it + 2;
	// ���ݽṹ�ǣ������ģ�ͨ���п����ļ��������ڴ�ռ䣩
	// ˫�����
}

void deqPrint(const deque<int> &deq)
{
	for (deque<int>::const_iterator it = deq.begin(); it != deq.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

/*  deque���캯��

deque<T> deqT;//Ĭ�Ϲ�����ʽ
deque(beg, end);//���캯����[beg, end)�����е�Ԫ�ؿ���������
deque(n, elem);//���캯����n��elem����������
deque(const deque &deq);//�������캯����
*/
void test01()
{
	int arr[] = { 1, 2, 3, 4, 5 };
	deque<int> d(arr, arr + sizeof(arr) / sizeof(int));
	deqPrint(d);

	deque<int> d2(5, 6);
	deqPrint(d2);
}

/*  deque��ֵ����

assign(beg, end);//��[beg, end)�����е����ݿ�����ֵ������
assign(n, elem);//��n��elem������ֵ������
deque&operator=(const deque &deq); //���صȺŲ�����
swap(deq);// ��deq�뱾���Ԫ�ػ���
*/
void test02()
{
	int arr[] = { 1, 2, 3, 4, 5 };
	deque<int> d;

	d.assign(arr, arr + sizeof(arr) / sizeof(int));
	deqPrint(d);

	deque<int> d2;
	d2.assign(5, 6);
	deqPrint(d2);

	cout << "-----------------" << endl;
	d2.swap(d);
	deqPrint(d);
	deqPrint(d2);
}

/*  deque��С����

deque.size();//����������Ԫ�صĸ���
deque.empty();//�ж������Ƿ�Ϊ��
deque.resize(num);//����ָ�������ĳ���Ϊnum,�������䳤������Ĭ��ֵ�����λ�á����������̣���ĩβ�����������ȵ�Ԫ�ر�ɾ����
deque.resize(num, elem); //����ָ�������ĳ���Ϊnum,�������䳤������elemֵ�����λ��,���������̣���ĩβ�����������ȵ�Ԫ�ر�ɾ����
*/
void test03()
{
	deque<int> d;
	cout << d.size() << endl;
	if (d.empty())
	{
		cout << "��" << endl;
	}

	d.resize(10, 6);
	deqPrint(d);
}

/*  deque˫�˲����ɾ������

push_back(elem);//������β�����һ������
push_front(elem);//������ͷ������һ������
pop_back();//ɾ���������һ������
pop_front();//ɾ��������һ������
*/
void test04()
{
	deque<int> d;
	d.push_back(10);
	d.push_front(20);
	d.push_front(30);
	deqPrint(d);

	d.pop_back();
	d.pop_front();
	deqPrint(d);
}

/*  deque���ݴ�ȡ����

at(idx);//��������idx��ָ�����ݣ����idxԽ�磬�׳�out_of_range��
operator[];//��������idx��ָ�����ݣ����idxԽ�磬���׳��쳣��ֱ�ӳ���
front();//���ص�һ�����ݡ�
back();//�������һ������

*/
void test05()
{
	int arr[] = { 1, 2, 3, 4, 5, 6 };
	deque<int> d(arr, arr + sizeof(arr) / sizeof(int));

	for (int i = d.size() - 1; i >= 0; i--)
	{
		cout << d.at(i) << " ";
	}
	cout << endl;

	d[2] = 200;
	deqPrint(d);

	cout << d.front() << " " << d.back() << endl;
}

/*  deque�������

insert(pos,elem);//��posλ�ò���һ��elemԪ�صĿ��������������ݵ�λ�á�
insert(pos,n,elem);//��posλ�ò���n��elem���ݣ��޷���ֵ��
insert(pos,beg,end);//��posλ�ò���[beg,end)��������ݣ��޷���ֵ��
*/
void test06()
{
	deque<int> d(8, 6);
	d.insert(d.begin() + 1, 100);
	deqPrint(d);

	d.insert(d.begin() + 1, 2, 200);
	deqPrint(d);

	deque<int> d2(2, 9);
	cout << "-------------" << endl;
	d2.insert(d2.begin() + 1, d.begin(), d.end());
	deqPrint(d2);
}

/*  dequeɾ������

clear();//�Ƴ���������������
erase(beg,end);//ɾ��[beg,end)��������ݣ�������һ�����ݵ�λ�á�
erase(pos);//ɾ��posλ�õ����ݣ�������һ�����ݵ�λ�á�
*/
void test07()
{
	deque<int> d(5, 8);
	d.erase(d.begin(), d.end());
	deqPrint(d);
}

int main(int argc, char **argv)
{
	test07();

	getchar();
	return 0;
}