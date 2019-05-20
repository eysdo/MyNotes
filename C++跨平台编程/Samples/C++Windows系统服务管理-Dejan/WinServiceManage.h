#pragma once

namespace sc 
{
// Start Type
#define SERVICE_BOOT_START             0x00000000
#define SERVICE_SYSTEM_START           0x00000001
#define SERVICE_AUTO_START             0x00000002
#define SERVICE_DEMAND_START           0x00000003
#define SERVICE_DISABLED               0x00000004
typedef const wchar_t* LPCWSTR;
typedef const wchar_t* PWSTR;
typedef unsigned long  DWORD;

	class WinServiceManage
	{
	public:
		/* �������� */
		static WinServiceManage* Get();

		/* �������� */
		virtual bool Run(LPCWSTR service_name) = 0;

		/* ��ͣ���� */
		virtual bool Stop(LPCWSTR service_name) = 0;

		/* �������� */
		virtual bool Create(PWSTR szPath, // �������·��
			PWSTR pszServiceName, // ��������
			PWSTR pszDisplayName, // ��ʾ����
			DWORD dwStartType,    // ��������  SERVICE_AUTO_START,SERVICE_DEMAND_START...
			PWSTR pszDependencies,// List of service dependencies - "dep1\0dep2\0\0", һ����� L""
			PWSTR pszAccount,     // ��¼�û�, L"NT AUTHORITY\\LocalService" or L"NT AUTHORITY\\SYSTEM" or L"NT AUTHORITY\\NETWORK SERVICE"
			PWSTR pszPassword     // �û�����, һ����� NULL
		) = 0;

		/* ɾ������ */
		virtual bool Delete(LPCWSTR service_name) = 0;

		/* �жϷ����Ƿ���� */
		virtual bool Has(LPCWSTR service_name) = 0;



		virtual ~WinServiceManage();

	protected:
		WinServiceManage();
	};

	/* �������� */
	#define sc_run WinServiceManage::Get()->Run
	/* ��ͣ���� */
	#define sc_stop WinServiceManage::Get()->Stop
	/* �жϷ����Ƿ���� */
	#define sc_has WinServiceManage::Get()->Has
	/* ɾ������ */
	#define sc_remove WinServiceManage::Get()->Delete
	/* �������� */
	#define sc_create WinServiceManage::Get()->Create

}



