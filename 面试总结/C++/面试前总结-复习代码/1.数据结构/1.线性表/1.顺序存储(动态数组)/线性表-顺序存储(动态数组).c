#include "DynamicArray.h"

// �û�������
typedef struct Student {
	char name[64];
	int age;
}Student;

// �����ӡ
void arr_print(void *data) 
{
	Student *student = (Student*)data;
	printf("name=%s,age=%d\n", student->name, student->age);
}

// ���ݱȶ�
void arr_compare(void *_s1, void *_s2)
{
	Student *s1 = (Student*)_s1;
	Student *s2 = (Student*)_s2;

	return (strcmp(s1->name, s2->name) == 0) && (s1->age == s2->age);
}

int main()
{
	// �û�����
	Student s1 = { "Dejan01", 11 };
	Student s2 = { "Dejan02", 12 };
	Student s3 = { "Dejan03", 13 };
	Student s4 = { "Dejan04", 14 };
	Student s5 = { "Dejan05", 15 };
	Student s6 = { "Dejan06", 16 };
	Student s7 = { "Dejan07", 17 };
	Student s8 = { "Dejan08", 18 };
	Student s9 = { "Dejan09", 19 };
	Student s10 = { "Dejan10", 20 };

	// ��ʼ������
	void *arr = init_array();

	printf("------- ����Ԫ�� -------\n");
	insert(arr, 0, &s1);
	insert(arr, 0, &s2);
	insert(arr, 0, &s3);
	each(arr, arr_print);
	printf("------- ͷ�� -------\n");
	push_front(arr,&s4);
	each(arr, arr_print);
	printf("------- β�� -------\n");
	push_back(arr, &s5);
	each(arr, arr_print);
	printf("���鵱ǰ����(capacity): %d,��ǰԪ�ظ���:%d\n", get_capacity(arr), get_size(arr));
	printf("------- ָ��λ�ò���Ԫ�� -------\n");
	insert(arr, 2, &s6); // �嵽�м�
	each(arr, arr_print);
	printf("------- ͷɾ�� -------\n");
	pop_front(arr);
	each(arr, arr_print);
	printf("------- βɾ�� -------\n");
	pop_back(arr);
	each(arr, arr_print);
	printf("------- ָ��λ��2ɾ�� -------\n");
	remove_by_pos(arr,2);
	each(arr, arr_print);
	printf("------- ָ��ֵɾ�� name=Dejan06,age=16 -------\n");
	remove_by_val(arr,&s6, arr_compare);
	each(arr, arr_print);

	// ����,�ͷſռ�
	destroy_array(arr);

	system("pause");
	return 0;
}