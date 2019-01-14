#pragma once
#include <string>
class XAudio
{
public:
	static XAudio* Get();

	///////////////////////////////////////////
	///导出音频文件
	///@para src string 源文件(多媒体文件)
	///@para out string 输出的音频文件(MP3)
	virtual bool ExportA(std::string src, std::string out) = 0;
	
	XAudio();
	virtual ~XAudio();
};

