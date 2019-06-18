#include "Tool.h"
#include <Windows.h>
using namespace std;

//将PULARGE_INTEGER类型的字节(B)数转化为(GB)单位
#define GB(x) (x.HighPart << 2) + (x.LowPart >> 20) / 1024.0

class CTool : public Tool
{
public:
	/* 获取磁盘使用情况 */
	vector<DiskUsage> GetDiskUsage()
	{
		vector<DiskUsage> vec_DiskInfo;

		int driveCount = 0; // 驱动设备数
		char szDriveInfo[16 + 1] = { 0 };
		/*
		获取主机中所有的逻辑驱动器字母，以BitMap的形式返回
		DWORD GetLogicalDrives();
		return 一个DWORD类型的值，第一位表示所对应的驱动器是否存在。一般情况下DWORD的数据长度是32位，在这个DWORD中，每一位对应了一个逻辑驱动器是否存在。第二位如果是“1”则表示驱动器“B:”存在，第四位如果是“1”则表示驱动器“D:”存在，以此类推
		*/
		DWORD driveInfo = GetLogicalDrives();
		_itoa_s(driveInfo, szDriveInfo, 2);
		//printf("GetLogicalDrives -> %016s\n", szDriveInfo);
		while (driveInfo) {
			if (driveInfo & 1) {
				driveCount++;
			}
			driveInfo >>= 1;
		}
		//printf("LogicalDriveCount = %d\n", driveCount);

		/*
		获取一个字串，其中包含了当前所有逻辑驱动器的根驱动器路径。
		DWORD GetLogicalDriveStrings(
		DWORD nBufferLength,    // size of buffer, 如果是此参数为0 后面的lpBuffer则不使用
		LPTSTR lpBuffer         // drive strings buffer, 用于装载逻辑驱动器名称的字串。每个名字都用一个NULL字符分隔，在最后一个名字后面用两个NULL表示中止（空中止） 例如A:\<null>C:\<null><null>
		);
		return
		返回填充完毕的缓冲区字符的的个数..不是长度..char和TCHAR 返回数目都是一样的..
		一般是4的倍数...(形如C:\ 包括之后的一个空字符0)..
		如果缓冲区不够大,则返回值大于nBufferLength
		如果失败..函数返回0..若想获得更多的错误信息,请使用GetLastError函数
		*/
		int driveStrLen = GetLogicalDriveStrings(0, NULL);
		//printf("GetLogicalDriveStrings -> %d\n\n", driveStrLen);

		//exception must have a constant value
		//char driveStr[driveStrLen];
		TCHAR *driveStr = new TCHAR[driveStrLen];
		GetLogicalDriveStrings(driveStrLen, driveStr);
		TCHAR *lpDriveStr = driveStr;
		for (int i = 0; i < driveCount; i++) {
			/*
			获取磁盘类型
			UINT GetDriveType(
			_In_opt_ LPCWSTR lpRootPathName //不包括卷名的磁盘根路径名
			);

			#define DRIVE_UNKNOWN     0
			#define DRIVE_NO_ROOT_DIR 1
			#define DRIVE_REMOVABLE   2
			#define DRIVE_FIXED       3
			#define DRIVE_REMOTE      4
			#define DRIVE_CDROM       5
			#define DRIVE_RAMDISK     6
			*/
			UINT driveType = GetDriveType(lpDriveStr);
			char *szDriveTypeArr[] = {
				"DRIVE_UNKNOWN",
				"DRIVE_NO_ROOT_DIR",
				"DRIVE_REMOVABLE",
				"DRIVE_FIXED",
				"DRIVE_REMOTE",
				"DRIVE_CDROM",
				"DRIVE_RAMDISK"
			};

			ULARGE_INTEGER freeBytesAvailableToCaller;
			ULARGE_INTEGER totalNumberOfBytes;
			ULARGE_INTEGER totalNumberOfFreeBytes;
			/*
			获取与一个磁盘的类型以及剩余空间容量有关的信息
			BOOL GetDiskFreeSpaceExW(
			_In_opt_ LPCWSTR lpDirectoryName,                       //不包括卷名的磁盘根路径名
			_Out_opt_ PULARGE_INTEGER lpFreeBytesAvailableToCaller, //磁盘的可用字节数
			_Out_opt_ PULARGE_INTEGER lpTotalNumberOfBytes,         //磁盘的总字节数
			_Out_opt_ PULARGE_INTEGER lpTotalNumberOfFreeBytes      //磁盘的剩余字节数
			);
			*/
			BOOL fResult = GetDiskFreeSpaceEx(lpDriveStr, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes);
			//printf("GetLogicalDriveStrings -> %ls\n", lpDriveStr);
			//printf("GetDriveType -> %d=%s\n", driveType, szDriveTypeArr[driveType]);
			//printf("GetDiskFreeSpaceEx -> Available=%f, Total=%f, free=%f\n\n", GB(freeBytesAvailableToCaller), GB(totalNumberOfBytes), GB(totalNumberOfFreeBytes));
			if (fResult) // 可用的磁盘设备
			{
				DiskUsage diskUsageInfo;
				strcpy(diskUsageInfo.driveName, (char*)lpDriveStr); // 驱动名 C:
				diskUsageInfo.driveType = driveType; // 驱动类型(int)
				strcpy(diskUsageInfo.driveTypeStr, (char*)szDriveTypeArr[driveType]); // 驱动类型(string)
				diskUsageInfo.available = GB(freeBytesAvailableToCaller); // 可用空间
				diskUsageInfo.total = GB(totalNumberOfBytes); // 总大小
				diskUsageInfo.free = GB(totalNumberOfFreeBytes); // 剩余空间
				diskUsageInfo.used = diskUsageInfo.total - diskUsageInfo.available; // 已用空间
				vec_DiskInfo.push_back(diskUsageInfo);
			}
			else
			{
				//printf("设备未准备好\n");
			}

			lpDriveStr += 4;
		}
		delete driveStr;
		return vec_DiskInfo;
	}

	/* 获取物理内存使用情况 */
	unsigned long GetMemUsage()
	{
		MEMORYSTATUS ms;
		::GlobalMemoryStatus(&ms);
		return ms.dwMemoryLoad;
	}

	/* 获取CPU使用情况 */
	__int64 CompareFileTime(FILETIME time1, FILETIME time2)
	{
		__int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime;
		__int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime;

		return (b - a);
	}
	__int64 GetCpuUsage()
	{
		HANDLE hEvent;
		BOOL res;
		FILETIME preidleTime;
		FILETIME prekernelTime;
		FILETIME preuserTime;
		FILETIME idleTime;
		FILETIME kernelTime;
		FILETIME userTime;

		res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
		preidleTime = idleTime;
		prekernelTime = kernelTime;
		preuserTime = userTime;

		hEvent = CreateEventA(NULL, FALSE, FALSE, NULL); // 初始值为 nonsignaled ，并且每次触发后自动设置为nonsignaled

		int i = 0;
		while (true) {
			WaitForSingleObject(hEvent, 1000);
			res = GetSystemTimes(&idleTime, &kernelTime, &userTime);

			__int64 idle = CompareFileTime(preidleTime, idleTime);
			__int64 kernel = CompareFileTime(prekernelTime, kernelTime);
			__int64 user = CompareFileTime(preuserTime, userTime);

			__int64 cpu = (kernel + user - idle) * 100 / (kernel + user);
			__int64 cpuidle = (idle) * 100 / (kernel + user);
			//cout << "CPU利用率:" << cpu << "%" << " CPU空闲率:" << cpuidle << "%" << endl;
			if(i == 1) return cpu;

			preidleTime = idleTime;
			prekernelTime = kernelTime;
			preuserTime = userTime;
			++i;
		}
		return 0;
	}
};


/* 工厂方法 */
Tool* Tool::Get()
{
	static Tool* tool = 0;
	if (tool == 0)
	{
		static CTool c_tool;
		tool = &c_tool;
	}
	return tool;
}