#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
extern "C"
{
	#include <libavformat/avformat.h>
}
using namespace std;
#ifdef _MSC_VER // Windows
	#pragma comment(lib, "avformat.lib")
	#pragma comment(lib, "avutil.lib")
#elif __GNUC__ // Linux

#endif

void print_err(int& re)
{
	if (0 != re)
	{
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf));
		cout << buf << endl;
		getchar();
	}
}

int main()
{
	// ��ʼ�����еķ�װ�ͽ��װ flv mp4 mov mp3 ..
	av_register_all();

	// ��ʼ�������
	avformat_network_init();

	// 1.���ļ�, ���װ
	// �����װ������
	AVFormatContext *ictx = NULL;
	const char* inUrl = "E:/WorkSpace/test/test.mp4";

	// ���ļ�, ����ļ�ͷ
	int re = avformat_open_input(&ictx, inUrl, 0, 0);
	if (0 != re)
	{
		print_err(re);
		return -1;
	}
	cout << "open file " << inUrl << "Success" << endl;

	// ��ȡ��Ƶ��Ƶ����Ϣ, h264 flv
	re = avformat_find_stream_info(ictx, 0);
	if (0 != re)
	{
		print_err(re);
		return -1;
	}

	av_dump_format(ictx, 0, inUrl, 0);


	cout << "file to rtmp test" << endl;
	getchar();
	return 0;
}