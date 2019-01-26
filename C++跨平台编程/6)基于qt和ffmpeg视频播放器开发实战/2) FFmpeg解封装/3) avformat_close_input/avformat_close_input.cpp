#include<iostream>
extern "C" {
#include <libavformat/avformat.h>
}
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
using namespace std;

int main(int argc, char** argv)
{
	const char* path = "C:/Users/Dejan/Videos/test.mp4";

	// ��ʼ����װ��
	av_register_all();

	// ��ʼ������� (���Դ�rtsp rtmp http Э�����Ƶ)
	avformat_network_init();

	// ��������
	AVDictionary *opts = NULL;
	// ����rtsp����tcpЭ���
	av_dict_set(&opts, "rtsp_transport", "tcp", 0);
	// ������ʱʱ��
	av_dict_set(&opts, "max_delay", "500", 0);

	AVFormatContext *ic = NULL;
	int re = avformat_open_input(
		&ic, 
		path, 
		0,    // 0��ʾ�Զ�ѡ����װ��
		&opts // ��������, ����rtsp����ʱʱ��
	);
	if (re != 0)
	{
		char buf[1024] = {0};
		av_strerror(re, buf, sizeof(buf)-1);
		cout << "open " << path << " failed! :" << buf << endl;
		getchar();
		return -1;
	}
	cout << "open " << path << " success!" << endl;

	if (ic)
	{
		// �ͷŷ�װ������, ���Ұ�ic��0
		avformat_close_input(&ic);
	}

	getchar();
	return 0;
}