#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Tool.h"
using namespace std;

int main(int argc, char **argv)
{
	// ��ȡ����ʹ�����
	vector<DiskUsage> disk(Tool::Get()->GetDiskUsage());
	for (vector<DiskUsage>::iterator it = disk.begin(); it != disk.end(); it++)
	{
		printf("��������: %s\n", it->driveName);
		printf("��������: %d => %s\n", it->driveType, it->driveTypeStr);
		printf("���ÿռ�: %f\n", it->used);
		printf("���ÿռ�: %f\n", it->available);
		printf("ʣ��ռ�: %f\n", it->free);
		printf("�ܴ�С(����): %f\n", it->total);
		printf("----------------------------------------\n");
	}
	cout << "��������:" << disk.size() << endl;


	printf("�����ڴ�: %d%%\n", Tool::Get()->GetMemUsage());
	printf("CPUʹ����: %d%%\n", Tool::Get()->GetCpuUsage());


	getchar();
	return 0;
}