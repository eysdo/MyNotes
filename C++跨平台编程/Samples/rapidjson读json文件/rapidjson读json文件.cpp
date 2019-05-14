#include <iostream>
// json����
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <vector>
#include <string>
using namespace std;
using namespace rapidjson;

struct URLRule // �Ƿ������������
{
	int id = 0;
	string method;
	string pattern;
};

/* URL�Ƿ����� */
static vector<URLRule> illegal_rule;

int main(int argc, char **argv)
{
	FILE *fp = fopen("./rule.data", "r");
	if (fp)
	{
		char* readBuffer = new char[20971520]; // ��� 20M���ļ�
		memset(readBuffer, 0, 20971520);
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		Document d;
		if (!d.ParseStream(is).HasParseError())
		{
			const Value& arr = d;
			for (Value::ConstValueIterator it = arr.Begin(); it != arr.End(); it++)
			{
				URLRule rule;
				rule.id = it->GetObject()["id"].GetInt(); // id
				rule.method = it->GetObject()["method"].GetString(); // ���󷽷�
				rule.pattern = it->GetObject()["pattern"].GetString(); // ƥ�����
				illegal_rule.push_back(rule);
			}
		}
		fclose(fp);
		delete[] readBuffer;
	}

	// ��������ƥ��
	for (vector<URLRule>::iterator it = illegal_rule.begin(); it != illegal_rule.end(); it++)
	{
		cout << "id:" << (*it).id << endl;
		cout << "method:" << (*it).method << endl;
		cout << "pattern:" << (*it).pattern << endl;
	}

	getchar();
	return 0;
}