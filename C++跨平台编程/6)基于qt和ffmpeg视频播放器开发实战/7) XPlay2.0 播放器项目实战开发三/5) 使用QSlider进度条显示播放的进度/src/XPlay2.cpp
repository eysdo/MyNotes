#include "XPlay2.h"
#include <QFileDialog>
#include <QDebug>
#include "XDemuxThread.h"
#include <QMessageBox>
#include <QTimerEvent>

static XDemuxThread dt;

// 定时器 滑动条显示
void XPlay2::timerEvent(QTimerEvent *e)
{
	long long total = dt._totalMs;
	if (total > 0)
	{
		double pos = (double)dt._pts / (double)total;
		int v = ui.playPos->maximum() * pos;
		ui.playPos->setValue(v);
	}
}

XPlay2::XPlay2(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	dt.Start();
	startTimer(40);
}

XPlay2::~XPlay2()
{
	dt.Close();
}

void XPlay2::OpenFile()
{
	// 选择文件
	QString name = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择视频文件"));
	if (name.isEmpty()) return;
	this->setWindowTitle(name);
	if (!dt.Open(name.toLocal8Bit(), ui.video))
	{
		QMessageBox::information(0, "error", "open file failed!");
		return;
	}

}