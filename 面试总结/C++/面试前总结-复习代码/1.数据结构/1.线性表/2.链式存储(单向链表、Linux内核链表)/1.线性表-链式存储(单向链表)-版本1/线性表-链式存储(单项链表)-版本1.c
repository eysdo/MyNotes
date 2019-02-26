#include "Linklist.h"

// �û�������
typedef struct Student {
	char name[64];
	int age;
}Student;

void print_r(void *data)
{
	Student *stu = (Student*)data;
	printf("name:%s, age:%d\n", stu->name, stu->age);
}

int _compare(void *_d1, void *_d2)
{
	Student *d1 = (Student*)_d1;
	Student *d2 = (Student*)_d2;
	return (strcmp(d1->name, d2->name) == 0) && d1->age == d2->age;
}

int main()
{
	Student s01 = { "Dejan01", 11 };
	Student s02 = { "Dejan02", 12 };
	Student s03 = { "Dejan03", 13 };
	Student s04 = { "Dejan04", 14 };
	Student s05 = { "Dejan05", 15 };
	Student s06 = { "Dejan06", 16 };
	Student s07 = { "Dejan07", 17 };
	Student s08 = { "Dejan08", 18 };
	Student s09 = { "Dejan09", 19 };
	Student s10 = { "Dejan10", 20 };

	// ��ʼ��
	List list = init();

	// �����ݲ�������
	insert(list, 0, &s01);
	insert(list, 0, &s02);
	insert(list, 0, &s03);

	// ������ӡ
	each(list, print_r);

	printf("------ ͷ��β��֮�� ------\n");
	push_front(list, &s04);
	push_back(list, &s05);
	each(list, print_r);

	printf("------ ��s03ǰ�����s06 ------\n");
	insert_by_val(list, &s03, &s06, _compare);
	each(list, print_r);

	printf("------ ɾ��λ��Ϊ4������ ------\n");
	remove_by_pos(list, 4);
	each(list, print_r);

	printf("------ ɾ��s05 ------\n");
	remove_by_val(list, &s05, _compare);
	each(list, print_r);

	printf("------ ͷɾ��βɾ֮�� ------\n");
	pop_front(list);
	pop_back(list);
	each(list,print_r);

	// ����
	destroy(list);

	system("pause");
	return 0;
}