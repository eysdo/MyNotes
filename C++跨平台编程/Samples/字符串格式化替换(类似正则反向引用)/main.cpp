#include <iostream>
#include "RuleReplace.h"
#include <string>
using namespace std;

int main(int argc, char **argv)
{
	string rule("(\\?#1$)|(\\?#2&)|(&#3$)|(&#1&)");
	string vals("action=catchimage,action=catchimage2,action=catchimage3");

	// ¸ñÊ½»¯Ìæ»»
	cout<<RuleReplace::Get()->Format(rule, vals)<<endl;
	


	getchar();
	return 0;
}