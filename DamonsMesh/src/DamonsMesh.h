#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DamonsMesh.h"

class DamonsMesh : public QMainWindow
{
	Q_OBJECT

public:
	DamonsMesh(QWidget *parent = Q_NULLPTR);

private slots:
	 /// exit app
	void doActionExit();

	/// import file dialog
	void doActionImport();


private:
	/// create all the actions.
	void connectActions(void);
	/// initialize 
	void initialize();
private:
	Ui::DamonsMeshClass ui;
};
