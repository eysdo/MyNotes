#pragma once
#include <list>
#include <mutex>
#include <QThread>
struct AVPacket;
class XDecode;

class XDecodeThread : public QThread
{
public:
	virtual void Push(AVPacket *pkt);

	// 取出一帧数据, 并出栈, 如果没有返回NULL
	virtual AVPacket *Pop();

	// 清理队列
	virtual void Clear();

	// 清理资源, 停止线程
	virtual void Close();

	XDecodeThread();
	virtual ~XDecodeThread();

public:
	int _maxList = 100; // 最大队列
	bool _isExit = false;
	XDecode *_decode = 0;

protected:
	std::list<AVPacket *> _packs;
	std::mutex _mux;
	
};

