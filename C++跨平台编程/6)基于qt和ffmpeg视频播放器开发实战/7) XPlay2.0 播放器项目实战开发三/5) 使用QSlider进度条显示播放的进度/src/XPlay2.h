#pragma once

#include <QtWidgets/QWidget>
#include "ui_XPlay2.h"

class XPlay2 : public QWidget
{
	Q_OBJECT

public:
	// 定时器 滑动条显示
	void timerEvent(QTimerEvent *e);

	XPlay2(QWidget *parent = Q_NULLPTR);
	~XPlay2();

public slots:
	void OpenFile();

private:
	Ui::XPlay2Class ui;
};
