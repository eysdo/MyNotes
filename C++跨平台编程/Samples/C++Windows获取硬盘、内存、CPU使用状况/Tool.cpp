#include "Tool.h"
#include <Windows.h>
using namespace std;

//��PULARGE_INTEGER���͵��ֽ�(B)��ת��Ϊ(GB)��λ
#define GB(x) (x.HighPart << 2) + (x.LowPart >> 20) / 1024.0

class CTool : public Tool
{
public:
	/* ��ȡ����ʹ����� */
	vector<DiskUsage> GetDiskUsage()
	{
		vector<DiskUsage> vec_DiskInfo;

		int driveCount = 0; // �����豸��
		char szDriveInfo[16 + 1] = { 0 };
		/*
		��ȡ���������е��߼���������ĸ����BitMap����ʽ����
		DWORD GetLogicalDrives();
		return һ��DWORD���͵�ֵ����һλ��ʾ����Ӧ���������Ƿ���ڡ�һ�������DWORD�����ݳ�����32λ�������DWORD�У�ÿһλ��Ӧ��һ���߼��������Ƿ���ڡ��ڶ�λ����ǡ�1�����ʾ��������B:�����ڣ�����λ����ǡ�1�����ʾ��������D:�����ڣ��Դ�����
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
		��ȡһ���ִ������а����˵�ǰ�����߼��������ĸ�������·����
		DWORD GetLogicalDriveStrings(
		DWORD nBufferLength,    // size of buffer, ����Ǵ˲���Ϊ0 �����lpBuffer��ʹ��
		LPTSTR lpBuffer         // drive strings buffer, ����װ���߼����������Ƶ��ִ���ÿ�����ֶ���һ��NULL�ַ��ָ��������һ�����ֺ���������NULL��ʾ��ֹ������ֹ�� ����A:\<null>C:\<null><null>
		);
		return
		���������ϵĻ������ַ��ĵĸ���..���ǳ���..char��TCHAR ������Ŀ����һ����..
		һ����4�ı���...(����C:\ ����֮���һ�����ַ�0)..
		���������������,�򷵻�ֵ����nBufferLength
		���ʧ��..��������0..�����ø���Ĵ�����Ϣ,��ʹ��GetLastError����
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
			��ȡ��������
			UINT GetDriveType(
			_In_opt_ LPCWSTR lpRootPathName //�����������Ĵ��̸�·����
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
			��ȡ��һ�����̵������Լ�ʣ��ռ������йص���Ϣ
			BOOL GetDiskFreeSpaceExW(
			_In_opt_ LPCWSTR lpDirectoryName,                       //�����������Ĵ��̸�·����
			_Out_opt_ PULARGE_INTEGER lpFreeBytesAvailableToCaller, //���̵Ŀ����ֽ���
			_Out_opt_ PULARGE_INTEGER lpTotalNumberOfBytes,         //���̵����ֽ���
			_Out_opt_ PULARGE_INTEGER lpTotalNumberOfFreeBytes      //���̵�ʣ���ֽ���
			);
			*/
			BOOL fResult = GetDiskFreeSpaceEx(lpDriveStr, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes);
			//printf("GetLogicalDriveStrings -> %ls\n", lpDriveStr);
			//printf("GetDriveType -> %d=%s\n", driveType, szDriveTypeArr[driveType]);
			//printf("GetDiskFreeSpaceEx -> Available=%f, Total=%f, free=%f\n\n", GB(freeBytesAvailableToCaller), GB(totalNumberOfBytes), GB(totalNumberOfFreeBytes));
			if (fResult) // ���õĴ����豸
			{
				DiskUsage diskUsageInfo;
				strcpy(diskUsageInfo.driveName, (char*)lpDriveStr); // ������ C:
				diskUsageInfo.driveType = driveType; // ��������(int)
				strcpy(diskUsageInfo.driveTypeStr, (char*)szDriveTypeArr[driveType]); // ��������(string)
				diskUsageInfo.available = GB(freeBytesAvailableToCaller); // ���ÿռ�
				diskUsageInfo.total = GB(totalNumberOfBytes); // �ܴ�С
				diskUsageInfo.free = GB(totalNumberOfFreeBytes); // ʣ��ռ�
				diskUsageInfo.used = diskUsageInfo.total - diskUsageInfo.available; // ���ÿռ�
				vec_DiskInfo.push_back(diskUsageInfo);
			}
			else
			{
				//printf("�豸δ׼����\n");
			}

			lpDriveStr += 4;
		}
		delete driveStr;
		return vec_DiskInfo;
	}

	/* ��ȡ�����ڴ�ʹ����� */
	unsigned long GetMemUsage()
	{
		MEMORYSTATUS ms;
		::GlobalMemoryStatus(&ms);
		return ms.dwMemoryLoad;
	}

	/* ��ȡCPUʹ����� */
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

		hEvent = CreateEventA(NULL, FALSE, FALSE, NULL); // ��ʼֵΪ nonsignaled ������ÿ�δ������Զ�����Ϊnonsignaled

		int i = 0;
		while (true) {
			WaitForSingleObject(hEvent, 1000);
			res = GetSystemTimes(&idleTime, &kernelTime, &userTime);

			__int64 idle = CompareFileTime(preidleTime, idleTime);
			__int64 kernel = CompareFileTime(prekernelTime, kernelTime);
			__int64 user = CompareFileTime(preuserTime, userTime);

			__int64 cpu = (kernel + user - idle) * 100 / (kernel + user);
			__int64 cpuidle = (idle) * 100 / (kernel + user);
			//cout << "CPU������:" << cpu << "%" << " CPU������:" << cpuidle << "%" << endl;
			if(i == 1) return cpu;

			preidleTime = idleTime;
			prekernelTime = kernelTime;
			preuserTime = userTime;
			++i;
		}
		return 0;
	}
};


/* �������� */
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