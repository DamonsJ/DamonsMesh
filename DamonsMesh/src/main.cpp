#include "DamonsMesh.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DamonsMesh w;
	w.show();
	return a.exec();
}
