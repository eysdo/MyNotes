#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Tool.h"
using namespace std;

int main(int argc, char **argv)
{
	// 获取磁盘使用情况
	vector<DiskUsage> disk(Tool::Get()->GetDiskUsage());
	for (vector<DiskUsage>::iterator it = disk.begin(); it != disk.end(); it++)
	{
		printf("驱动名称: %s\n", it->driveName);
		printf("驱动类型: %d => %s\n", it->driveType, it->driveTypeStr);
		printf("已用空间: %f\n", it->used);
		printf("可用空间: %f\n", it->available);
		printf("剩余空间: %f\n", it->free);
		printf("总大小(容量): %f\n", it->total);
		printf("----------------------------------------\n");
	}
	cout << "磁盘数量:" << disk.size() << endl;


	printf("物理内存: %d%%\n", Tool::Get()->GetMemUsage());
	printf("CPU使用率: %d%%\n", Tool::Get()->GetCpuUsage());


	getchar();
	return 0;
}