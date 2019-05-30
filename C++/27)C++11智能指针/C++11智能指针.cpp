#include <iostream>
#include <memory>
#include <string>
using namespace std;

/* shared_ptr */
void Shared_ptr()
{
	{
		int a = 10;
		shared_ptr<int> ptra = make_shared<int>(a);
		shared_ptr<int> ptra2(ptra); // copy , ptra���ü���+1
		cout << "ptra�����ü���:" << ptra.use_count() << endl; // ���Ǳ� ���� ���ü�������+1

		int b = 20;
		int *pb = &a;
		//shared_ptr<int> ptrb = pb; // error
		shared_ptr<int> ptrb = make_shared<int>(b);
		ptra2 = ptrb; // assign , ptrb���ü���+1, ptra���ü���-1
		pb = ptrb.get(); // ��ȡԭʼָ��

		cout << "ptra�����ü���:" << ptra.use_count() << endl;
		cout << "ptrb�����ü���:" << ptrb.use_count() << endl;
		cout << "*pb = " << *pb << endl;
	}
	// ����ָ�������������Զ��ڴ�����ͷ�
}
void Shared_ptr2()
{
	{
		shared_ptr<string> ptr;
		if (!ptr)
		{
			cout << "����ָ��ptrĬ�ϳ�ʼ��ΪNULL" << endl;
		}

		// һ��ĳ�ʼ����ʽ
		shared_ptr<string> ptr1(new string("Hello!!"));
		cout << "*ptr1 = " << *ptr1 << endl;

		// �Ƽ��İ�ȫ�ĳ�ʼ����ʽ
		shared_ptr<string> ptr2 = make_shared<string>("Hello!!");
		cout << "*ptr2 = " << *ptr2 << endl;
	}
	// ����ָ�������������Զ��ڴ�����ͷ�
}

/* weak_ptr */
void Weak_ptr()
{
	{
		/* weak_ptr��Ϊ�����shared_ptr�������һ������ָ��, �����������Э��shared_ptr������
		ʹ��weak_ptr�ĳ�Ա����use_count()���Թ۲���Դ�����ü�������һ����Ա����expired()�Ĺ��ܵȼ���use_count()==0,
		�����죬��ʾ���۲����Դ(Ҳ����shared_ptr�Ĺ������Դ)�Ѿ��������ڡ�
		weak_ptr����ʹ��һ���ǳ���Ҫ�ĳ�Ա����lock()�ӱ��۲��shared_ptr���һ�����õ�shared_ptr���󣬴Ӷ�������Դ��
		����expired()==true��ʱ��lock()����������һ���洢��ָ���shared_ptr��
		*/
		shared_ptr<int> ptr = make_shared<int>(10);
		cout << "ptr�����ü���:" << ptr.use_count() << endl;

		weak_ptr<int> wp(ptr);
		cout << "wp�����ü���:" << wp.use_count() << endl;

		if (!wp.expired())
		{
			shared_ptr<int> ptr2 = wp.lock(); // get another shared_ptr
			*ptr = 100;
			cout << "wp�����ü���:" << wp.use_count() << endl;
			cout << "*ptr = " << *ptr << endl;
		}
	}
	// ����ָ�������������Զ��ڴ�����ͷ�
}

/* unique_ptr */
void Unique_ptr()
{
	{
		unique_ptr<int> ptr(new int(10)); // �󶨶�̬����
		//unique_ptr<int> ptr2 = ptr; // error, ���ɸ�ֵ
		//unique_ptr<int> ptr2(ptr); // error, ���ɿ���
		unique_ptr<int> ptr2 = move(ptr); // ת��
		//ptr2.release(); // �ͷ�

		cout << *ptr2 << endl;
	}
	// ����ָ�������������Զ��ڴ�����ͷ�
}

int main(int argc, char **argv)
{
	Shared_ptr2();



	getchar();
	return 0;
}