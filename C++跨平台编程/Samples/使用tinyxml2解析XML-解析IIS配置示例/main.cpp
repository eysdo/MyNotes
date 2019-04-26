#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include "tinyxml2.h"
using namespace std;

// IIS��վ�����
#define SITE_NUM_MAX     1000
#define IIS_CONFIG_FILE  "C:/Windows/System32/inetsrv/config/applicationHost.config"

struct SiteInfo
{
	int id;              // ��վID
	string name;         // ��վ����
	string physicalPath; // ��վ��·��
	string protocol;     // Э��
	string bindingInfo;  // ����Ϣ
	string logFileDir;      // ��־�ļ�·��
};


int main(int argc, char **argv)
{
	if (Wow64EnableWow64FsRedirection(0))//�ر��ض���
	{
		// ģ���ȡ�ļ�
		/*
		FILE *fp = fopen("C:/Windows/System32/inetsrv/config/applicationHost.config", "r");
		if (!fp)
		{
			printf("��ʧ��!!\n");
			printf("error: %s\n", strerror(errno));
			getchar();
		}
		printf("�򿪳ɹ�!!\n");
		fclose(fp);
		*/

		// ��ȡxml�ļ�, ������
		const char* xml_file = IIS_CONFIG_FILE;
		tinyxml2::XMLDocument doc;
		doc.LoadFile(xml_file);

		cout << "doc.ErrorID():"<< doc.ErrorID() << endl;
		cout << "-----------------------------------------------" << endl;
		tinyxml2::XMLElement* configurationNode_1 = doc.FirstChildElement("configuration");
		tinyxml2::XMLElement* sysAppHostNode_2 = configurationNode_1->FirstChildElement("system.applicationHost");
		tinyxml2::XMLElement* sitesNode_3 = sysAppHostNode_2->FirstChildElement("sites");
		tinyxml2::XMLElement* sites = sitesNode_3; // ��վ�б�ڵ㿪ʼ

		// ��ȡ��վ��־Ŀ¼(Ĭ��)
		string logFileDir_df = sites
			->FirstChildElement("siteDefaults")
			->FirstChildElement("logFile")
			->Attribute("directory");

		// ��վ��Ϣ����
		SiteInfo site_info[SITE_NUM_MAX];

		// ��վ����ͳ��
		int site_count = 0;

		tinyxml2::XMLElement* site = sites->FirstChildElement("site");
		while (site)
		{
			// ��ȡ��վID
			site_info[site_count].id = atoi(site->Attribute("id"));
			// ��ȡ��վ����
			site_info[site_count].name = site->Attribute("name");
			// ��ȡ��վ��·��
			site_info[site_count].physicalPath = site
				->FirstChildElement("application")
				->FirstChildElement("virtualDirectory")
				->Attribute("physicalPath");
			// ��ð���Ϣ�ڵ�
			tinyxml2::XMLElement* binding = site
				->FirstChildElement("bindings")
				->FirstChildElement("binding");
			// ��ȡ��վ����Э��
			site_info[site_count].protocol = binding->Attribute("protocol");
			// ��ȡ��վ����Ϣ
			site_info[site_count].bindingInfo = binding->Attribute("bindingInformation");
			// ��ȡ��վ��־·��
			tinyxml2::XMLElement* logFile = site->FirstChildElement("logFile");
			if (logFile)
				site_info[site_count].logFileDir = logFile->Attribute("directory");
			else
				site_info[site_count].logFileDir = logFileDir_df;


			// -----------------------------------
			++site_count;
			site = site->NextSiblingElement("site");
		}

		cout << "��վ���� : " << site_count << endl;
		cout << "-----------------------------------------------" << endl;
		for (int i = 0; i < site_count; i++)
		{
			cout << "��վID:" << site_info[i].id << endl;
			cout << "��վ����:" << site_info[i].name << endl;
			cout << "��վ��·��:" << site_info[i].physicalPath << endl;
			cout << "����Э��:" << site_info[i].protocol << endl;
			cout << "����Ϣ:" << site_info[i].bindingInfo << endl;
			cout << "��վ��־�ļ�·��:" << site_info[i].logFileDir << endl;
			cout << "-----------------------------------------------" << endl;
		}

		
		
		


	}
	Wow64EnableWow64FsRedirection(1); //�ָ��ض���

	getchar();
	return 0;
}