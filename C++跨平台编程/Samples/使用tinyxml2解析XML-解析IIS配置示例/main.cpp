#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include "tinyxml2.h"
using namespace std;

// IIS网站最大数
#define SITE_NUM_MAX     1000
#define IIS_CONFIG_FILE  "C:/Windows/System32/inetsrv/config/applicationHost.config"

struct SiteInfo
{
	int id;              // 网站ID
	string name;         // 网站名称
	string physicalPath; // 网站根路径
	string protocol;     // 协议
	string bindingInfo;  // 绑定信息
	string logFileDir;      // 日志文件路径
};


int main(int argc, char **argv)
{
	if (Wow64EnableWow64FsRedirection(0))//关闭重定向
	{
		// 模拟读取文件
		/*
		FILE *fp = fopen("C:/Windows/System32/inetsrv/config/applicationHost.config", "r");
		if (!fp)
		{
			printf("打开失败!!\n");
			printf("error: %s\n", strerror(errno));
			getchar();
		}
		printf("打开成功!!\n");
		fclose(fp);
		*/

		// 读取xml文件, 并解析
		const char* xml_file = IIS_CONFIG_FILE;
		tinyxml2::XMLDocument doc;
		doc.LoadFile(xml_file);

		cout << "doc.ErrorID():"<< doc.ErrorID() << endl;
		cout << "-----------------------------------------------" << endl;
		tinyxml2::XMLElement* configurationNode_1 = doc.FirstChildElement("configuration");
		tinyxml2::XMLElement* sysAppHostNode_2 = configurationNode_1->FirstChildElement("system.applicationHost");
		tinyxml2::XMLElement* sitesNode_3 = sysAppHostNode_2->FirstChildElement("sites");
		tinyxml2::XMLElement* sites = sitesNode_3; // 网站列表节点开始

		// 获取网站日志目录(默认)
		string logFileDir_df = sites
			->FirstChildElement("siteDefaults")
			->FirstChildElement("logFile")
			->Attribute("directory");

		// 网站信息数组
		SiteInfo site_info[SITE_NUM_MAX];

		// 网站个数统计
		int site_count = 0;

		tinyxml2::XMLElement* site = sites->FirstChildElement("site");
		while (site)
		{
			// 读取网站ID
			site_info[site_count].id = atoi(site->Attribute("id"));
			// 读取网站名称
			site_info[site_count].name = site->Attribute("name");
			// 读取网站根路径
			site_info[site_count].physicalPath = site
				->FirstChildElement("application")
				->FirstChildElement("virtualDirectory")
				->Attribute("physicalPath");
			// 获得绑定信息节点
			tinyxml2::XMLElement* binding = site
				->FirstChildElement("bindings")
				->FirstChildElement("binding");
			// 读取网站访问协议
			site_info[site_count].protocol = binding->Attribute("protocol");
			// 读取网站绑定信息
			site_info[site_count].bindingInfo = binding->Attribute("bindingInformation");
			// 读取网站日志路径
			tinyxml2::XMLElement* logFile = site->FirstChildElement("logFile");
			if (logFile)
				site_info[site_count].logFileDir = logFile->Attribute("directory");
			else
				site_info[site_count].logFileDir = logFileDir_df;


			// -----------------------------------
			++site_count;
			site = site->NextSiblingElement("site");
		}

		cout << "网站个数 : " << site_count << endl;
		cout << "-----------------------------------------------" << endl;
		for (int i = 0; i < site_count; i++)
		{
			cout << "网站ID:" << site_info[i].id << endl;
			cout << "网站名称:" << site_info[i].name << endl;
			cout << "网站根路径:" << site_info[i].physicalPath << endl;
			cout << "访问协议:" << site_info[i].protocol << endl;
			cout << "绑定信息:" << site_info[i].bindingInfo << endl;
			cout << "网站日志文件路径:" << site_info[i].logFileDir << endl;
			cout << "-----------------------------------------------" << endl;
		}

		
		
		


	}
	Wow64EnableWow64FsRedirection(1); //恢复重定向

	getchar();
	return 0;
}