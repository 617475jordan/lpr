#include "activate.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	activate w;
	w.show();
	return a.exec();
}
