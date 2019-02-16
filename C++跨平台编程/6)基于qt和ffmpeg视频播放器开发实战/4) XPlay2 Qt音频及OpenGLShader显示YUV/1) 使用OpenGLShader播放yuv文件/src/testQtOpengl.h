#pragma once

#include <QtWidgets/QWidget>
#include "ui_testQtOpengl.h"

class testQtOpengl : public QWidget
{
	Q_OBJECT

public:
	testQtOpengl(QWidget *parent = Q_NULLPTR);

private:
	Ui::testQtOpenglClass ui;
};
