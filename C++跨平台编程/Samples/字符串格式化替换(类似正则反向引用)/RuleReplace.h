#pragma once
#include <string>

class RuleReplace
{
public:
	/* �������� */
	static RuleReplace* Get();

	/* �����ʽ���滻 */
	virtual std::string Format(std::string fmt_str, std::string& vals) = 0;



	virtual ~RuleReplace();

protected:
	RuleReplace();
};

