#include "testQtOpengl.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	testQtOpengl w;
	w.show();
	return a.exec();
}
