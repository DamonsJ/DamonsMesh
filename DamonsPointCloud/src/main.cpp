#include "DamonsPointCloud.h"
#include <QtWidgets/QApplication>
#include "DPCSettings.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QCoreApplication::setOrganizationName(DPCSettings::OrganizationName());
	QCoreApplication::setApplicationName(DPCSettings::ApplicationName());

	DamonsPointCloud w;
	w.show();
	return a.exec();
}
