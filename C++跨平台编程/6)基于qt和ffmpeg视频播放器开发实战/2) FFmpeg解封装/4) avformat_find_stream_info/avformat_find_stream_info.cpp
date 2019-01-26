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

	// 初始化封装库
	av_register_all();

	// 初始化网络库 (可以打开rtsp rtmp http 协议的视频)
	avformat_network_init();

	// 参数设置
	AVDictionary *opts = NULL;
	// 设置rtsp流以tcp协议打开
	av_dict_set(&opts, "rtsp_transport", "tcp", 0);
	// 网络延时时间
	av_dict_set(&opts, "max_delay", "500", 0);

	AVFormatContext *ic = NULL;
	int re = avformat_open_input(
		&ic, 
		path, 
		0,    // 0表示自动选择解封装器
		&opts // 参数设置, 比如rtsp的延时时间
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

	// 获取流信息
	re = avformat_find_stream_info(ic, 0);

	// 总时长 毫秒
	int totalMs = ic->duration / (AV_TIME_BASE / 1000);
	cout << "totalMs = " << totalMs << endl;

	// 打印视频流详细信息
	av_dump_format(ic, 0, path, 0);


	if (ic)
	{
		// 释放封装上下文, 并且把ic置0
		avformat_close_input(&ic);
	}

	getchar();
	return 0;
}