#pragma once

#include <vtkAutoInit.h> 
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <QtWidgets/QMainWindow>
#include "ui_DamonsPointCloud.h"
#include <QFileDialog>

class DamonsPointCloud : public QMainWindow
{
	Q_OBJECT

public:
	DamonsPointCloud(QWidget *parent = Q_NULLPTR);

	//////////////////////////////////////////////////////////////////////////

private slots:
	//! Displays file open dialog
	void doActionLoadFile();

private:
	//! Connects all QT actions to slots
	void connectActions();

private:
	Ui::DamonsPointCloudClass ui;

};
