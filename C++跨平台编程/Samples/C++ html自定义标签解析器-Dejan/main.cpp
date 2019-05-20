#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "TagParser.h"
using namespace std;

int main(int argc, char **argv)
{
	/* �Զ����ǩ */
	map<string, string> tags;
	tags.insert(make_pair("[$title]",   gbk_to_utf8("403-title")));
	tags.insert(make_pair("[$cur_url]", gbk_to_utf8("��ǰURL")));
	tags.insert(make_pair("[$info]",    gbk_to_utf8("������Ϣ, ����..")));
	tags.insert(make_pair("[$admin_url]", gbk_to_utf8("����Ա��¼ҳ��תurl")));
	tags.insert(make_pair("[$id]", gbk_to_utf8("123")));


	string temp_html_403;

	ifstream ifs;
	ifs.open("./403.html", ios::in);
	if (ifs.is_open())
	{
		while (!ifs.eof())
		{
			char buf[1024] = { 0 };
			ifs.getline(buf, 1024);
			temp_html_403 += buf;
			temp_html_403 += "\n"; // ���뻻��
		}
		ifs.close();
	}
	
	// �����Զ����ǩ
	temp_html_403 = tag_parser(temp_html_403, tags);


	ofstream ofs;
	ofs.open("./temp_html_403.html", ios::out|ios::trunc);
	if (ofs.is_open())
	{
		ofs << temp_html_403;
		ofs.close();
	}
	cout << "����ļ��ɹ�!" << endl;

	getchar();
	return 0;
}