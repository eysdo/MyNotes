#pragma once
#include <string>

class RuleReplace
{
public:
	/* 工厂方法 */
	static RuleReplace* Get();

	/* 规则格式化替换 */
	virtual std::string Format(std::string fmt_str, std::string& vals) = 0;



	virtual ~RuleReplace();

protected:
	RuleReplace();
};

