#include "cpp_qt_dump_test.h"

cpp_qt_dump_test::cpp_qt_dump_test(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// Ä£Äâ³ÌÐò±ÀÀ£
	char *p = NULL;
	memcpy(p, "HELLO", strlen("HELLO"));
}
