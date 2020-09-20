#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DamonsMesh.h"

class DamonsMesh : public QMainWindow
{
	Q_OBJECT

public:
	DamonsMesh(QWidget *parent = Q_NULLPTR);

private:
	Ui::DamonsMeshClass ui;
};
