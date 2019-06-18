#ifndef TOOL_H
#define TOOL_H
#include <vector>


// 磁盘使用情况
struct DiskUsage
{
	char driveName[3] = { 0 };  // 驱动名
	int  driveType = 0; // 驱动类型
	char driveTypeStr[32] = { 0 }; // 驱动类型
	double available = 0.0; // 可用空间
	double total = 0.0; // 总大小
	double free = 0.0;  // 剩余空间
	double used = 0.0;  // 已用空间
};
// 内存使用情况
struct MemUsage
{

};
// CPU使用情况
struct CpuUsage
{

};


class Tool
{
public:
	/* 工厂方法 */
	static Tool* Get();

	/* 获取磁盘使用情况 */
	virtual std::vector<DiskUsage> GetDiskUsage() = 0;

	/* 获取内存使用情况 */
	virtual unsigned long GetMemUsage() = 0;

	/* 获取CPU使用情况 */
	virtual __int64 GetCpuUsage() = 0;



protected:
	Tool() {}
};





#endif // !TOOL_H

