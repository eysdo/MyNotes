#pragma once
#include <string>
#include <map>

class TagParser
{
public:
	/* �������� */
	static TagParser* Get();

	/* ��ǩ������ */
	virtual std::string Parser(std::string& content, std::map<std::string, std::string>& tags) = 0;

	/* GBKתUTF8���� */
	virtual std::string GBKToUTF8(const std::string& strGBK) = 0;


public:
	virtual ~TagParser();

protected:
	TagParser();
};

/* GBKתUTF8���� */
#define gbk_to_utf8 TagParser::Get()->GBKToUTF8

/* ��ǩ������ */
#define tag_parser  TagParser::Get()->Parser

