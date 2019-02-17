#pragma once

#include <QObject>
#include <QMouseEvent>
#include <QSlider>

class XSlider : public QSlider
{
	Q_OBJECT

public:
	void mousePressEvent(QMouseEvent *e);

	XSlider(QWidget *parent = NULL);
	~XSlider();
};
