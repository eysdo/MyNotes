#pragma once

#include <QtWidgets/QWidget>
#include "ui_XVideoUI.h"

class XVideoUI : public QWidget
{
	Q_OBJECT

public:
	XVideoUI(QWidget *parent = Q_NULLPTR);
	void timerEvent(QTimerEvent *e);

public slots:
	void Open();
	void SliderPress();
	void SliderRelease();
	// 滑动条拖动
	void SetPos(int);

	// 设置过滤器
	void Set();

private:
	Ui::XVideoUIClass ui;
};
