#pragma once
#include <string>
#include <map>

class TagParser
{
public:
	/* 工厂方法 */
	static TagParser* Get();

	/* 标签解析器 */
	virtual std::string Parser(std::string& content, std::map<std::string, std::string>& tags) = 0;

	/* GBK转UTF8编码 */
	virtual std::string GBKToUTF8(const std::string& strGBK) = 0;


public:
	virtual ~TagParser();

protected:
	TagParser();
};

/* GBK转UTF8编码 */
#define gbk_to_utf8 TagParser::Get()->GBKToUTF8

/* 标签解析器 */
#define tag_parser  TagParser::Get()->Parser

