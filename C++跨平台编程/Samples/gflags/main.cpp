#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gflags/gflags.h>
using namespace std;

#pragma comment(lib, "gflags_static.lib")
#pragma comment(lib, "shlwapi.lib")

/* 命令行默认参数定义
DEFINE_bool: 布尔类型
DEFINE_int32: 32 位整数
DEFINE_int64: 64 位整数
DEFINE_uint64: 无符号 64 位整数
DEFINE_double: 浮点类型 double
DEFINE_string: C++ string 类型
*/
DEFINE_string(host, "127.0.0.1", "the server host"); // 主机地址
DEFINE_int32(port, 9090, "the server port"); // 主机端口

/* 定义参数检查函数(谓词) */
static bool check_port(const char* name, int32_t value);

// 使用全局 static 变量来注册函数的目的是让该注册函数在 main 函数执行之前调用
static const bool reg_check_port = gflags::RegisterFlagValidator(&FLAGS_port, &check_port);

int main(int argc, char **argv)
{
	// 解析命令行参数 
	// gflags.exe --port=22 --host=127.0.0.1
	// gflags.exe -port 23 -host 127.0.0.2
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	// 调用参数变量 FLAGS_xxx
	cout << "host: " << FLAGS_host << " port: " << FLAGS_port << endl;

	// 修改参数
	gflags::SetCommandLineOption("host", "-1");
	cout << "host: " << FLAGS_host << " port: " << FLAGS_port << endl;


	//getchar();
	return 0;
}

/* 定义参数检查函数(谓词) */
static bool check_port(const char* name, int32_t value)
{
	if (value > 0 && value < 32768) {
		return true;
	}
	printf("Invalid value for --%s: %d\n", name, (int)value);
	return false;
}