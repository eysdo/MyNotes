#include "RuleReplace.h"
#include <vector>
using namespace std;

class CRuleReplace : public RuleReplace
{
public:
	/* 字符串分割 */
	vector<string> split(const string& str, const string& pattern)
	{
		vector<string> ret;
		if (pattern.empty()) return ret;
		size_t start = 0, index = str.find_first_of(pattern, 0);
		while (index != str.npos)
		{
			if (start != index)
				ret.push_back(str.substr(start, index - start));
			start = index + 1;
			index = str.find_first_of(pattern, start);
		}
		if (!str.substr(start).empty())
			ret.push_back(str.substr(start));
		return ret;
	}

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

	/* 规则格式化替换 */
	string Format(string fmt_str, string& vals)
	{
		vector<string> replace_arr = split(vals, ",");
		int i = 1;
		for (vector<string>::iterator it = replace_arr.begin(); it != replace_arr.end(); it++)
		{
			replace_all(fmt_str, string("#") + to_string(i), *it);
			++i;
		}
		return fmt_str;
	}
};

/* 工厂方法 */
RuleReplace* RuleReplace::Get()
{
	static RuleReplace* rr = 0;
	if (rr == 0)
	{
		static CRuleReplace c_rr;
		rr = &c_rr;
	}
	return rr;
}

RuleReplace::RuleReplace()
{
}


RuleReplace::~RuleReplace()
{
}
