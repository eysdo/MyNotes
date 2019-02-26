#include "linkQueue.h"

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
	void *queue = init_link_queue();

	// ���
	push_link_queue(queue, &s1);
	push_link_queue(queue, &s2);
	push_link_queue(queue, &s3);
	push_link_queue(queue, &s4);
	push_link_queue(queue, &s5);

	// �����ͷ�Ͷ�βԪ��
	Student *stu1 = (Student*)get_link_queue_head(queue);
	Student *stu2 = (Student*)get_link_queue_tail(queue);
	printf("���ڶ�����ǰ��ͬѧstu1->name:%s, stu1->age:%d\n", stu1->name, stu1->age);
	printf("���ڶ�����ǰ��ͬѧstu2->name:%s, stu2->age:%d\n", stu2->name, stu2->age);

	printf("-------------------------------\n");

	// ����������е�Ԫ��
	while (get_link_queue_size(queue) > 0)
	{
		// ��ȡ��ͷԪ��, ��Ϊ��ͷԪ�ز������
		Student *stu = (Student*)get_link_queue_head(queue);
		printf("name:%s, age:%d\n", stu->name, stu->age);

		// ������ͷԪ��
		pop_link_queue(queue);
	}

	printf("size:%d\n", get_link_queue_size(queue));

	// ���ٶ���
	destroy_link_queue(queue);

	system("pause");
	return 0;
}