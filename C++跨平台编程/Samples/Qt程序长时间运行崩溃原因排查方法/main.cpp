#include "cpp_qt_dump_test.h"
#include <QtWidgets/QApplication>

#pragma comment(lib,"Shell32.lib")

/// DUMP
#include <windows.h>
#include <DbgHelp.h>
#pragma comment(lib,"Dbghelp.lib")

long  __stdcall CrashInfocallback(_EXCEPTION_POINTERS *pexcp)
{
	//���� Dump �ļ�
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
		//Dump��Ϣ
		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
		dumpInfo.ExceptionPointers = pexcp;
		dumpInfo.ThreadId = GetCurrentThreadId();
		dumpInfo.ClientPointers = TRUE;
		//д��Dump�ļ�����
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
VS2015 ��Ŀ���� - Release����

1.����.pdb�ļ�, ����ҳ->������->����->���ɵ�����Ϣ�������ѡtrue��Ĭ��ֵ��false��
2.����ҳ->C/C++->����->������Ϣ��ʽ�������ѡ���������ݿ�(/Zi)��
3.����ҳ->C/C++->�Ż�->�Ż��������ѡ���ѽ���(/Od)��
*/

int main(int argc, char *argv[])
{
	::SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CrashInfocallback);

	QApplication a(argc, argv);
	cpp_qt_dump_test w;
	w.show();
	return a.exec();
}
