#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
using namespace std;

// �ַ����ָ�
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
	// �ڴ�ӳ���������
	wchar_t* shareMemoryId = TEXT("Global\\IISIPFilter_92380000");
	string str = "192.168.1.8|192.168.1.9|192.168.3.18";
	LPVOID map_p; // �����ڴ�ָ��

	SECURITY_DESCRIPTOR sd;

	if (!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))
		printf("InitializeSecurityDescriptor failed %d\n", GetLastError());

	if (!SetSecurityDescriptorDacl(&sd, true, 0, false))
		printf("SetSecurityDescriptorDacl failed %d\n", GetLastError());

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = &sd;
	sa.bInheritHandle = false;


	// ��ͼ��һ���������ڴ�ӳ���ļ�����
	HANDLE hMap = ::OpenFileMapping(
		FILE_MAP_ALL_ACCESS, // read/write access
		true,          // !!!!! do inherit the name
		shareMemoryId  // name of mapping object
	);
	if (NULL == hMap) // �����ʧ�������´���
	{
		hMap = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,  // use paging file
			&sa,                   // default security
			PAGE_READWRITE,        // read / write access
			0,                     // maximum object size (high-order DWORD)
			strlen(str.c_str())+1, // maximum object size (low-order DWORD)
			(WCHAR*)shareMemoryId  // name of mapping object
		);

		// ӳ������һ����ͼ���õ�ָ�����ڴ��ָ�룬�������������
		map_p = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		strcpy((char*)map_p, str.c_str());
		cout << "д�빲���ڴ����ݣ�" << (char *)map_p << endl;
	}
	else
	{
		// �򿪳ɹ���ӳ������һ����ͼ���õ�ָ�����ڴ��ָ�룬��ʾ�����������
		map_p = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		cout << "��ȡ�����ڴ����ݣ�" << (char *)map_p << endl;
		string str((char*)map_p);

		vector<string> vec = split(str, "|");
		cout << vec[0] << endl;
		cout << vec[1] << endl;
		cout << vec[2] << endl;
		
	}


	getchar();

	// ����ļ�ӳ�䣬�ر��ڴ�ӳ���ļ�������
	::UnmapViewOfFile(map_p);
	::CloseHandle(hMap);


	getchar();
	return 0;
}