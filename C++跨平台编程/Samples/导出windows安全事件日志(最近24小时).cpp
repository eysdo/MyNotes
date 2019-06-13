#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
	// 导出最近
	const char* ev_log_out_cmd = "wevtutil epl Security security_log.evtx \"/q:*[System[TimeCreated[timediff(@SystemTime) <= 86400000]]]\" /ow:true";
	system(ev_log_out_cmd);

	cout << "导出完成" << endl;
	getchar();
	return 0;
}