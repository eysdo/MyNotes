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
	// »¬¶¯ÌõÍÏ¶¯
	void SetPos(int);

private:
	Ui::XVideoUIClass ui;
};
