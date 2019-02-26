#include "LinkStack.h"

// �û�������
typedef struct Student {
	Node node;
	char name[64];
	int age;
}Student;

int main()
{
	Student s1 = { NULL,"Dejan1",10 };
	Student s2 = { NULL,"Dejan2",20 };
	Student s3 = { NULL,"Dejan3",30 };
	Student s4 = { NULL,"Dejan4",40 };
	Student s5 = { NULL,"Dejan5",50 };

	// ��ʼ������
	void *stack = init();

	// ��ջ
	push(stack, &s1);
	push(stack, &s2);
	push(stack, &s3);
	push(stack, &s4);
	push(stack, &s5);

	while (get_size(stack) > 0)
	{
		// ��ȡջ��Ԫ��
		Student *stu = get_stack_top(stack);
		printf("name:%s, age:%d\n", stu->name, stu->age);
		// ����ջ��Ԫ��
		pop(stack);
		printf("size:%d\n", get_size(stack));
	}

	destroy(stack);

	system("pause");
	return 0;
}