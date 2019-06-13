#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gflags/gflags.h>
using namespace std;

#pragma comment(lib, "gflags_static.lib")
#pragma comment(lib, "shlwapi.lib")

/* ������Ĭ�ϲ�������
DEFINE_bool: ��������
DEFINE_int32: 32 λ����
DEFINE_int64: 64 λ����
DEFINE_uint64: �޷��� 64 λ����
DEFINE_double: �������� double
DEFINE_string: C++ string ����
*/
DEFINE_string(host, "127.0.0.1", "the server host"); // ������ַ
DEFINE_int32(port, 9090, "the server port"); // �����˿�

/* ���������麯��(ν��) */
static bool check_port(const char* name, int32_t value);

// ʹ��ȫ�� static ������ע�ắ����Ŀ�����ø�ע�ắ���� main ����ִ��֮ǰ����
static const bool reg_check_port = gflags::RegisterFlagValidator(&FLAGS_port, &check_port);

int main(int argc, char **argv)
{
	// ���������в��� 
	// gflags.exe --port=22 --host=127.0.0.1
	// gflags.exe -port 23 -host 127.0.0.2
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	// ���ò������� FLAGS_xxx
	cout << "host: " << FLAGS_host << " port: " << FLAGS_port << endl;

	// �޸Ĳ���
	gflags::SetCommandLineOption("host", "-1");
	cout << "host: " << FLAGS_host << " port: " << FLAGS_port << endl;


	//getchar();
	return 0;
}

/* ���������麯��(ν��) */
static bool check_port(const char* name, int32_t value)
{
	if (value > 0 && value < 32768) {
		return true;
	}
	printf("Invalid value for --%s: %d\n", name, (int)value);
	return false;
}