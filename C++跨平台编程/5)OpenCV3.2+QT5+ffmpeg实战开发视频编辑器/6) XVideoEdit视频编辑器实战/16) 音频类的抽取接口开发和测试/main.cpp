#include "XVideoUI.h"
#include <QtWidgets/QApplication>
#include "XAudio.h"
int main(int argc, char *argv[])
{
	XAudio::Get()->ExportA("C:\\Users\\Dejan\\Videos\\test.mp4","./out.mp3");

	QApplication a(argc, argv);
	XVideoUI w;
	w.show();
	return a.exec();
}
