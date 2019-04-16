#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
using namespace std;

// 字符串分割
vector<string> split(const string& str, const string& pattern)
{
	vector<string> ret;
	if (pattern.empty()) return ret;
	size_t start = 0, index = str.find_first_of(pattern, 0);
	while (index != str.npos)
	{
		if (start != index)
			ret.push_back(str.substr(start, index - start));
		start = index + 1;
		index = str.find_first_of(pattern, start);
	}
	if (!str.substr(start).empty())
		ret.push_back(str.substr(start));
	return ret;
}

int main()
{
	// 内存映射对象名称
	wchar_t* shareMemoryId = TEXT("Global\\IISIPFilter_92380000");
	string str = "192.168.1.8|192.168.1.9|192.168.3.18";
	LPVOID map_p; // 共享内存指针

	SECURITY_DESCRIPTOR sd;

	if (!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))
		printf("InitializeSecurityDescriptor failed %d\n", GetLastError());

	if (!SetSecurityDescriptorDacl(&sd, true, 0, false))
		printf("SetSecurityDescriptorDacl failed %d\n", GetLastError());

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = &sd;
	sa.bInheritHandle = false;


	// 试图打开一个命名的内存映射文件对象
	HANDLE hMap = ::OpenFileMapping(
		FILE_MAP_ALL_ACCESS, // read/write access
		true,          // !!!!! do inherit the name
		shareMemoryId  // name of mapping object
	);
	if (NULL == hMap) // 如果打开失败则重新创建
	{
		hMap = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,  // use paging file
			&sa,                   // default security
			PAGE_READWRITE,        // read / write access
			0,                     // maximum object size (high-order DWORD)
			strlen(str.c_str())+1, // maximum object size (low-order DWORD)
			(WCHAR*)shareMemoryId  // name of mapping object
		);

		// 映射对象的一个视图，得到指向共享内存的指针，设置里面的数据
		map_p = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		strcpy((char*)map_p, str.c_str());
		cout << "写入共享内存数据：" << (char *)map_p << endl;
	}
	else
	{
		// 打开成功，映射对象的一个视图，得到指向共享内存的指针，显示出里面的数据
		map_p = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		cout << "读取共享内存数据：" << (char *)map_p << endl;
		string str((char*)map_p);

		vector<string> vec = split(str, "|");
		cout << vec[0] << endl;
		cout << vec[1] << endl;
		cout << vec[2] << endl;
		
	}


	getchar();

	// 解除文件映射，关闭内存映射文件对象句柄
	::UnmapViewOfFile(map_p);
	::CloseHandle(hMap);


	getchar();
	return 0;
}