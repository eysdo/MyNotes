#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char(callback)(int,char*); // 函数类型

print_r(callback call)
{
	call(123, "hello");
}
char test(int num, char *str)
{
	printf("num=%d, str=%s\n", num, str);
}



int main(int argc, char **argv)
{

	/* 指针数组 */
	char *a[10] = {0};
	char **b[10] = {0};
	
	char* t = "零";
	a[0] = t;
	b[0] = &t;

	printf("a[0] = %s\n", a[0]);
	printf("*(b[0]) = %s\n", *(b[0]));


	/* 函数类型 */
	print_r(test);


	/* 函数指针 */
	typedef char(*func_p)(int,char*);
	func_p a_func = test;
	a_func(678, "world");

	char(*func_p2)(int, char*);
	func_p2 = test;
	func_p2(888, "nnnnnnnn");


	/* 数组类型 */
	typedef char ARRAY[];
	ARRAY array = "hello array!";
	printf("ARRAY array = %s\n", array);


	/* 数组指针 */
	char(*array_t)[];
	array_t = array;
	printf("array_t = %s\n", array_t);

	typedef char(*ARRAY2)[];
	ARRAY2 array_t2 = array_t;
	printf("ARRAY2 array_t2 = %s\n", array_t2);


	/* 指针法操作一维数组 */
	int arr_1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	for (int i = 0; i < 10; i++)
	{
		printf("%d  ", *(arr_1 + i));
	}
	printf("\n--------------------------------\n");

	
	/* 指针法操作二维数组 */
	int arr2[5][10] = {
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
	};
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			int temp = *(*(arr2 + i) + j);
			printf("%d  ", temp);
			if (temp == 9) printf("\n");
		}
	}


	getchar();
	return 0;
}