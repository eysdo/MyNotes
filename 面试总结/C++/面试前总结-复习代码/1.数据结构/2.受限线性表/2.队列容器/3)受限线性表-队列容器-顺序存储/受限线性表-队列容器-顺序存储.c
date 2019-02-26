#include "SeqQueue.h"

// �û�������
typedef struct Student {
	char name[64];
	int age;
}Student;

int main()
{
	Student s1 = { "Dejan1",10 };
	Student s2 = { "Dejan2",20 };
	Student s3 = { "Dejan3",30 };
	Student s4 = { "Dejan4",40 };
	Student s5 = { "Dejan5",50 };

	// ��ʼ������
	void *queue = init();

	// ���
	push(queue, &s1);
	push(queue, &s2);
	push(queue, &s3);
	push(queue, &s4);
	push(queue, &s5);

	// �����ͷ�Ͷ�βԪ��
	Student *stu1 = (Student*)get_queue_head(queue);
	Student *stu2 = (Student*)get_queue_tail(queue);
	printf("���ڶ�����ǰ��ͬѧstu1->name:%s, stu1->age:%d\n", stu1->name, stu1->age);
	printf("���ڶ�������ͬѧstu2->name:%s, stu2->age:%d\n", stu2->name, stu2->age);

	printf("-------------------------------\n");

	// ����������е�Ԫ��
	while (get_size(queue) > 0)
	{
		// ��ȡ��ͷԪ��, ��Ϊ��ͷԪ�ز������
		Student *stu = (Student*)get_queue_head(queue);
		printf("name:%s, age:%d\n", stu->name, stu->age);

		// ������ͷԪ��
		pop(queue);
	}

	printf("size:%d\n", get_size(queue));

	destroy(queue);

	system("pause");
	return 0;
}