#include <QtCore/QCoreApplication>
#include <iostream>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QThread>	
using namespace std;
// 准备好pcm数据
// ffmpeg -i test.mp4 -f s16le out.pcm
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QAudioFormat fmt;
	fmt.setSampleRate(44100);
	fmt.setSampleSize(16);
	fmt.setChannelCount(2);
	fmt.setCodec("audio/pcm");
	fmt.setByteOrder(QAudioFormat::LittleEndian);
	fmt.setSampleType(QAudioFormat::UnSignedInt);
	QAudioOutput *out = new QAudioOutput(fmt);
	QIODevice *io = out->start(); // 开始播放

	FILE *fp = fopen("./out.pcm", "rb");
	if (!fp)
	{
		cout << "open file failed!" << endl;
	}
	cout << "open pcm file success!" << endl;

	int size = out->periodSize();
	char *buf = new char[size];

	while (!feof(fp))
	{
		if (out->bytesFree() < size) // bytesFree()返回音频缓冲区中可用的空闲空间。
		{
			QThread::msleep(1);
			continue;
		}
		int len = fread(buf, 1, size, fp);
		if (len <= 0) break;
		io->write(buf, len);
	}
	fclose(fp);
	delete[] buf;
	buf = 0;

	return a.exec();
}
