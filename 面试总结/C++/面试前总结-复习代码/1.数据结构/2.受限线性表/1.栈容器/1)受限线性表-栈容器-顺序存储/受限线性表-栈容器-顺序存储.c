#include "SeqStack.h"

// �û�������
typedef struct Student {
	char nane[64];
	int age;
}Student;

int main()
{
	Student s1 = { "Dejan1",10 };
	Student s2 = { "Dejan2",20 };
	Student s3 = { "Dejan3",30 };
	Student s4 = { "Dejan4",40 };
	Student s5 = { "Dejan5",50 };

	// ��ʼ��ջ
	void *stack = init();

	// ������ջ
	push(stack, &s1);
	push(stack, &s2);
	push(stack, &s3);
	push(stack, &s4);
	push(stack, &s5);

	// ���ջ�����е�Ԫ��
	while (get_size(stack) > 0)
	{
		Student *stu = (Student*)get_stack_top(stack);
		printf("name:%s, age:%d\n",stu->nane,stu->age);
		// ����ջ��Ԫ��
		pop(stack);
	}

	destroy(stack);

	system("pause");
	return 0;
}