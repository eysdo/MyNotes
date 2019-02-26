#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void printfArr(int arr[], int len)
{
	for (int i = 0; i < len; i++)
	{
		printf("%d ", arr[i]);
	}

	printf("\n");
}
//1.把数据分为有序数和无序数
//2.把第一个数当成有序数
//3.遍历无序数,
//4.把无序数和有序数进行比较
//5.遍历有序数，把无序数给临时变量
//6.临时变量中的数要小于arr[j]才往右移
//7.arr[j + 1] = tmp;//因为for循环中的j--,所有要j+1

//升序2 3 4 8 
void InsertSort(int arr[], int len)
{
	//遍历无序数,
	for (int i = 1; i < len; i++)
	{
		//条件达到，表示要交换
		if (arr[i] < arr[i - 1])//把无序数和有序数进行比较
		{
			int tmp = arr[i];//3

			//遍历有序数
			int j = i - 1;//i=3-1==j=2
			//临时变量中的数要小于arr[j]才往右一点
			for (; j >= 0 && tmp<arr[j]; j--) // 3<2
			{

				arr[j + 1] = arr[j];//arr[1]=arr[0]
				
			}
			arr[j + 1] = tmp;//因为for循环中的j--,所有要j+1

		}
	}
}

int main()
{

	int arr[] = { 4, 2, 8, 3 };
	int len = sizeof(arr) / sizeof(int);

	printfArr(arr, len);

	InsertSort(arr, len);

	printfArr(arr, len);

	system("pause");
	return EXIT_SUCCESS;
}

