#include "cpp_qt_dump_test.h"
#include <QtWidgets/QApplication>

#pragma comment(lib,"Shell32.lib")

/// DUMP
#include <windows.h>
#include <DbgHelp.h>
#pragma comment(lib,"Dbghelp.lib")

long  __stdcall CrashInfocallback(_EXCEPTION_POINTERS *pexcp)
{
	//创建 Dump 文件
	HANDLE hDumpFile = ::CreateFile(
		L"MEMORY.DMP",
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hDumpFile != INVALID_HANDLE_VALUE)
	{
		//Dump信息
		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
		dumpInfo.ExceptionPointers = pexcp;
		dumpInfo.ThreadId = GetCurrentThreadId();
		dumpInfo.ClientPointers = TRUE;
		//写入Dump文件内容
		::MiniDumpWriteDump(
			GetCurrentProcess(),
			GetCurrentProcessId(),
			hDumpFile,
			MiniDumpNormal,
			&dumpInfo,
			NULL,
			NULL
		);
	}
	return 0;
}
/// DUMP END

/*
VS2015 项目配置 - Release设置

1.生成.pdb文件, 属性页->链接器->调试->生成调试信息，这里得选true（默认值是false）
2.属性页->C/C++->常规->调试信息格式，这里得选“程序数据库(/Zi)”
3.属性页->C/C++->优化->优化，这里得选“已禁用(/Od)”
*/

int main(int argc, char *argv[])
{
	::SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CrashInfocallback);

	QApplication a(argc, argv);
	cpp_qt_dump_test w;
	w.show();
	return a.exec();
}
