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

	// ȡ��һ֡����, ����ջ, ���û�з���NULL
	virtual AVPacket *Pop();

	// �������
	virtual void Clear();

	// ������Դ, ֹͣ�߳�
	virtual void Close();

	XDecodeThread();
	virtual ~XDecodeThread();

public:
	int _maxList = 100; // ������
	bool _isExit = false;
	XDecode *_decode = 0;

protected:
	std::list<AVPacket *> _packs;
	std::mutex _mux;
	
};

