#pragma once

#include <QtWidgets/QWidget>
#include "ui_test_qt_mat.h"

class test_qt_mat : public QWidget
{
	Q_OBJECT

public:
	test_qt_mat(QWidget *parent = Q_NULLPTR);

private:
	Ui::test_qt_matClass ui;
};
