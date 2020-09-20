#include "DamonsMesh.h"
#include "../../DamonsIO/include/FileIOFilter.h"
/// qt
#include <QMessageBox>

DamonsMesh::DamonsMesh(QWidget *parent): QMainWindow(parent)
{
	ui.setupUi(this);
	connectActions();
	initialize();
}

void DamonsMesh::connectActions(void)
{
	connect(ui.actionExit, &QAction::triggered, this, &DamonsMesh::doActionExit);
	connect(ui.actionImport, &QAction::triggered, this, &DamonsMesh::doActionImport);

}

void DamonsMesh::initialize()
{
	DamonsIO::FileIOFilter::InitInternalFilters();
}
///////////////////////////////////////////////////////////////////////////////////////////////////

void DamonsMesh::doActionExit()
{
	QApplication::quit();
}

void DamonsMesh::doActionImport()
{
	QMessageBox::information(NULL, "Warning", "Not Implement", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}