#pragma once
#include <string>
class XAudio
{
public:
	static XAudio* Get();

	///////////////////////////////////////////
	///������Ƶ�ļ�
	///@para src string Դ�ļ�(��ý���ļ�)
	///@para out string �������Ƶ�ļ�(MP3)
	virtual bool ExportA(std::string src, std::string out) = 0;
	
	XAudio();
	virtual ~XAudio();
};

