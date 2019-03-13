/**
 * CPP-RTMP推流 - 低延迟方案
 * Author: Dejan  QQ: 673008865
 */

#include <iostream>
using namespace std;
extern "C" {
	/* 引入FFMPEG API头文件 */
#include <libavformat/avformat.h>
#include <libavutil/time.h>
}

/* 引入库 */
#pragma comment(lib, "avformat.lib") // 格式封装库
#pragma comment(lib, "avcodec.lib")  // 编解码库
#pragma comment(lib, "avutil.lib")   // 工具库, 包括获取错误信息、av_usleep()等


#define AV_CODEC_FLAG_GLOBAL_HEADER (1 << 22)
#define CODEC_FLAG_GLOBAL_HEADER AV_CODEC_FLAG_GLOBAL_HEADER

// 错误输出
int avError(int errNum);

int main()
{
	const char* inUrl = "F:/video/2_bd.mp4"; // mp4 or flv
	const char* outUrl = "rtmp://192.168.163.131/live/dejan";

	/////////////////////////////////////////////////////
	/// 1. 初始化
	// 初始化所有的封装和解封装 flv mp4 mov mp3 ..
	av_register_all();

	// 初始化网络库
	avformat_network_init();
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	/// 2. 输入流处理
	// 输入文件的封装上下文
	AVFormatContext *ictx = NULL;

	// 打开文件, 解封文件头
	int ret = avformat_open_input(&ictx, inUrl, 0, 0);
	if (0 != ret)
		return avError(ret);
	cout << "avformat_open_input success!" << endl;

	// 获取音频视频流信息, h264 flv
	ret = avformat_find_stream_info(ictx, 0);
	if (0 > ret)
		return avError(ret);
	// 打印输入流信息
	av_dump_format(ictx, 0, inUrl, 0);
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	/// 3. 输出流处理
	// 创建输出流上下文
	AVFormatContext *octx = NULL;
	ret = avformat_alloc_output_context2(&octx, 0, "flv", outUrl);
	if (0 > ret)
		return avError(ret);
	cout << "avformat_alloc_output_context2 success!" << endl;

	cout << "nb_streams : " << ictx->nb_streams << endl; // AVStream数组的大小
	int vstm_index = -1; // 视频流索引
	// 遍历流
	for (int i = 0; i < ictx->nb_streams; i++)
	{
		// 获得输入流: 音频和视频信息
		AVStream *in_stm = ictx->streams[i];
		// 创建新通道索引为输出流同时初始化空间
		AVStream *out_stm = avformat_new_stream(octx, in_stm->codec->codec);
		if (!out_stm)
		{
			cout << "avformat_new_stream() failed!";
			ret = AVERROR_UNKNOWN;
		}
		// 复制编码器参数配置: 输入编解码器上下文 copy to 输出编解码器上下文
		ret = avcodec_parameters_copy(out_stm->codecpar, in_stm->codecpar);
		if (0 > ret)
			cout << "avcodec_parameters_copy() failed!" << endl;
		out_stm->codecpar->codec_tag = 0;
		out_stm->codec->codec_tag = 0;
		if (octx->oformat->flags & AVFMT_GLOBALHEADER)
			out_stm->codec->flags = out_stm->codec->flags | CODEC_FLAG_GLOBAL_HEADER;
		// 获得视频流索引
		if (in_stm->codec->codec_type == AVMEDIA_TYPE_VIDEO && vstm_index == -1)
			vstm_index = i;
	}
	// 打印输出流信息
	av_dump_format(octx, 0, outUrl, 1);
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	/// 4. 准备推流

	// 打开IO
	ret = avio_open(&octx->pb, outUrl, AVIO_FLAG_WRITE);
	if (0 > ret)
		return avError(ret);

	// 写入头部信息
	ret = avformat_write_header(octx, 0);
	if (0 > ret)
		return avError(ret);
	cout << "avformat_write_header Success!" << endl;
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	/// 5. 开始推流
	// 推流每一帧数据
	AVPacket pkt;
	long long start_time = av_gettime(); // 获取当前的时间戳(单位:微妙)
	long long frame_index = 0; // 自定义视频帧索引
	for (;;)
	{
		// 输入输出音视频流
		AVStream *in_stm = NULL, *out_stm = NULL;
		// 读取输入帧数据
		ret = av_read_frame(ictx, &pkt);
		if (0 > ret) break;

		/*
		PTS（Presentation Time Stamp）显示播放时间
		DTS（Decoding Time Stamp）解码时间
		*/
		//没有显示时间 (比如未解码的 H.264)
		if (pkt.pts == AV_NOPTS_VALUE)
		{
			AVRational time_base = ictx->streams[vstm_index]->time_base;

			// 计算两帧之间的时间
			/*
			r_frame_rate 基流帧速率
			av_q2d 转化为double类型
			*/
			int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(ictx->streams[vstm_index]->r_frame_rate);

			// 配置参数
			pkt.pts = (double)(frame_index*calc_duration) / (double)(av_q2d(time_base)*AV_TIME_BASE);
			pkt.dts = pkt.pts;
			pkt.duration = (double)calc_duration / (double)(av_q2d(time_base)*AV_TIME_BASE);
		}

		// 延时同步处理, 低延时推流的关键
		if (pkt.stream_index == vstm_index)
		{
			AVRational time_base = ictx->streams[vstm_index]->time_base;
			AVRational time_base_q = { 1, AV_TIME_BASE };
			// 计算视频播放时间
			int64_t pts_time = av_rescale_q(pkt.dts, time_base, time_base_q);
			// 计算实际视频的播放时间
			int64_t now_time = av_gettime() - start_time;

			AVRational avr = ictx->streams[vstm_index]->time_base;
			cout << avr.num << " " << avr.den << "  " << pkt.dts << "  " << pkt.pts << "   " << pts_time << endl;
			if (pts_time > now_time)
			{
				// 睡眠一段时间（目的是让当前视频记录的播放时间与实际时间同步）
				av_usleep((unsigned int)(pts_time - now_time));
			}
		}

		in_stm = ictx->streams[pkt.stream_index];
		out_stm = octx->streams[pkt.stream_index];

		// 计算延时后，重新指定时间戳
		pkt.pts = av_rescale_q_rnd(pkt.pts, in_stm->time_base, out_stm->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
		pkt.dts = av_rescale_q_rnd(pkt.dts, in_stm->time_base, out_stm->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
		pkt.duration = (int)av_rescale_q(pkt.duration, in_stm->time_base, out_stm->time_base);
		// 字节流的位置，-1 表示不知道字节流位置
		pkt.pos = -1;

		if (pkt.stream_index == vstm_index)
		{
			printf("Send %8d video frames to output URL\n", frame_index);
			frame_index++;
		}

		// 推送帧数据
		ret = av_interleaved_write_frame(octx, &pkt);
		if (0 != ret)
			return avError(ret);

		//释放 packet
		av_free_packet(&pkt);
	}


	getchar();
	return 0;
}

int avError(int errNum) {
	char buf[1024];
	//获取错误信息
	av_strerror(errNum, buf, sizeof(buf));
	cout << " failed! " << buf << endl;
	getchar();
	return -1;
}