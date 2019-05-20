#include "TagParser.h"
#include <windows.h>
using namespace std;

class CTagParser : public TagParser
{
public:
	/* 字符串替换 */
	string&  replace_all(string& str, const string& old_value, const string& new_value)
	{
		for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
			if ((pos = str.find(old_value, pos)) != string::npos)
				str.replace(pos, old_value.length(), new_value);
			else   break;
		}
		return   str;
	}

	/* GBK转UTF8编码 */
	string GBKToUTF8(const std::string& strGBK)
	{
		string strOutUTF8 = "";
		WCHAR * str1;
		int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
		str1 = new WCHAR[n];
		MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
		n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
		char * str2 = new char[n];
		WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
		strOutUTF8 = str2;
		delete[]str1;
		str1 = NULL;
		delete[]str2;
		str2 = NULL;
		return strOutUTF8;
	}

	/* 标签解析器 */
	string Parser(string& content, map<string, string>& tags)
	{
		string temp = content;
		for (map<string, string>::iterator it = tags.begin(); it != tags.end(); it++)
		{
			temp = replace_all(temp, it->first.c_str(), it->second.c_str());
		}
		return temp;
	}

};

/* 工厂方法 */
TagParser* TagParser::Get()
{
	static TagParser* tp = 0;
	if (0 == tp)
	{
		static CTagParser c_tp;
		tp = &c_tp;
	}
	return tp;
}

TagParser::TagParser()
{
}


TagParser::~TagParser()
{
}
