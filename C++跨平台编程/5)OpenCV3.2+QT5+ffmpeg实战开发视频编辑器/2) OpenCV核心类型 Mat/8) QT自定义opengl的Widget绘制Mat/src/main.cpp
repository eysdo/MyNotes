#include "test_qt_mat.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	test_qt_mat w;
	w.show();
	return a.exec();
}
