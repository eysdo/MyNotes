#ifndef REMOTETHREAD_H
#define REMOTETHREAD_H
#include <vector>

#define MAX_PATH          260

namespace injector
{
	/* 远程注入 */
	class RemoteThread
	{
	public:
		/* 构造函数 */
		static RemoteThread* Get();

		/* 通过进程名获取进程ID */
		virtual std::vector<unsigned long> GetProcessIDByName(const wchar_t* pName) = 0;

		/* 启用调试权限 */
		virtual bool EnableDebugPrivilege() = 0;

		/* 远程注入DLL到进程 */
		virtual bool InjectDllByRemoteThread(unsigned int ulTargetProcessID, wchar_t* wzDllFullPath) = 0;


		virtual ~RemoteThread() {}

	protected:
		RemoteThread() {};
	};

	/* 启用调试权限 */
	#define enableDebugPrivilege RemoteThread::Get()->EnableDebugPrivilege

	/* 远程注入DLL到进程 */
	#define injectDllByRemoteThread RemoteThread::Get()->InjectDllByRemoteThread

	/* 通过进程名获取进程ID */
	#define getProcessIDByName RemoteThread::Get()->GetProcessIDByName
}



#endif // !REMOTETHREAD_H




