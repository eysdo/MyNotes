#ifndef REMOTETHREAD_H
#define REMOTETHREAD_H
#include <vector>

#define MAX_PATH          260

namespace injector
{
	/* Զ��ע�� */
	class RemoteThread
	{
	public:
		/* ���캯�� */
		static RemoteThread* Get();

		/* ͨ����������ȡ����ID */
		virtual std::vector<unsigned long> GetProcessIDByName(const wchar_t* pName) = 0;

		/* ���õ���Ȩ�� */
		virtual bool EnableDebugPrivilege() = 0;

		/* Զ��ע��DLL������ */
		virtual bool InjectDllByRemoteThread(unsigned int ulTargetProcessID, wchar_t* wzDllFullPath) = 0;


		virtual ~RemoteThread() {}

	protected:
		RemoteThread() {};
	};

	/* ���õ���Ȩ�� */
	#define enableDebugPrivilege RemoteThread::Get()->EnableDebugPrivilege

	/* Զ��ע��DLL������ */
	#define injectDllByRemoteThread RemoteThread::Get()->InjectDllByRemoteThread

	/* ͨ����������ȡ����ID */
	#define getProcessIDByName RemoteThread::Get()->GetProcessIDByName
}



#endif // !REMOTETHREAD_H




