#ifndef TOOL_H
#define TOOL_H
#include <vector>


// ����ʹ�����
struct DiskUsage
{
	char driveName[3] = { 0 };  // ������
	int  driveType = 0; // ��������
	char driveTypeStr[32] = { 0 }; // ��������
	double available = 0.0; // ���ÿռ�
	double total = 0.0; // �ܴ�С
	double free = 0.0;  // ʣ��ռ�
	double used = 0.0;  // ���ÿռ�
};
// �ڴ�ʹ�����
struct MemUsage
{

};
// CPUʹ�����
struct CpuUsage
{

};


class Tool
{
public:
	/* �������� */
	static Tool* Get();

	/* ��ȡ����ʹ����� */
	virtual std::vector<DiskUsage> GetDiskUsage() = 0;

	/* ��ȡ�ڴ�ʹ����� */
	virtual unsigned long GetMemUsage() = 0;

	/* ��ȡCPUʹ����� */
	virtual __int64 GetCpuUsage() = 0;



protected:
	Tool() {}
};





#endif // !TOOL_H

