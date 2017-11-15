#include "lprdemo.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	LPRdemo w;
	w.show();
	return a.exec();
}
