#pragma once

#include <QtWidgets/QWidget>
#include "ui_cpp_qt_dump_test.h"

class cpp_qt_dump_test : public QWidget
{
	Q_OBJECT

public:
	cpp_qt_dump_test(QWidget *parent = Q_NULLPTR);

private:
	Ui::cpp_qt_dump_testClass ui;
};
