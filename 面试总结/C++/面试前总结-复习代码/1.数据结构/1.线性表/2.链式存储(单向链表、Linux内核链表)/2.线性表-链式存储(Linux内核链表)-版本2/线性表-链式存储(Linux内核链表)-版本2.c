#include "Linklist.h"

// �û�������, �û������ݽṹ���һ����Ա����������������
typedef struct Student {
	Node node;
	char name[64];
	int age;
}Student;

// ��ӡ����
void print_r(void *_data)
{
	Student *data = (Student*)_data;
	printf("name:%s, age:%d\n", data->name, data->age);
}


int main()
{
	Student s1 = { NULL,"Dejan1",10 };
	Student s2 = { NULL,"Dejan2",20 };
	Student s3 = { NULL,"Dejan3",30 };
	Student s4 = { NULL,"Dejan4",40 };
	Student s5 = { NULL,"Dejan5",50 };

	void *list = init();
	// �����ݲ�������
	insert(list, 0, (Node*)&s1);
	insert(list, 0, (Node*)&s2);
	insert(list, 0, (Node*)&s3);
	insert(list, 0, (Node*)&s4);
	insert(list, 0, (Node*)&s5);

	each(list, print_r);

	printf("------ ָ��λ��3ɾ���ڵ�Ԫ�� ------\n");
	remove_by_pos(list, 3);
	each(list, print_r);

	Student *s2_data = get_node(list, 2);
	printf("ѧ��s2��name:%s, ѧ��s2��age:%d\n", s2_data->name, s2_data->age);
	printf("����ǰ�ڵ���size:%d\n", get_size(list));

	// ����
	destroy(list);

	system("pause");
	return 0;
}