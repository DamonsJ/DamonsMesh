#include "DamonsPointCloud.h"
#include <QString>
#include <QSettings>
#include "DPCSettings.h"

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
// static functions
static QFileDialog::Options DPCFileDialogOptions()
{
	//dialog options
	QFileDialog::Options dialogOptions = QFileDialog::Options();
	return dialogOptions;
}

//////////////////////////////////////////////////////////////////////////
DamonsPointCloud::DamonsPointCloud(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}


//////////////////////////////////////////////////////////////////////////
// all connections

void DamonsPointCloud::connectActions() {

	//////////////////////////////////////////////////////////////////////////
	/*** MAIN MENU ***/
	//"File" menu
	connect(ui.actionOpen, &QAction::triggered, this, &DamonsPointCloud::doActionLoadFile);
	//connect(ui.actionSave, &QAction::triggered, this, &MainWindow::doActionSaveFile);
	//connect(ui.actionGlobalShiftSettings, &QAction::triggered, this, &MainWindow::doActionGlobalShiftSeetings);
	//connect(ui.actionPrimitiveFactory, &QAction::triggered, this, &MainWindow::doShowPrimitiveFactory);
	//connect(ui.actionCloseAll, &QAction::triggered, this, &MainWindow::closeAll);
	//connect(ui.actionQuit, &QAction::triggered, this, &QWidget::close);
}


//////////////////////////////////////////////////////////////////////////
// slot functions
void DamonsPointCloud::doActionLoadFile()
{
	//persistent settings
	QSettings settings;
	settings.beginGroup(DPCSettings::LoadFile());
	QString currentPath = settings.value(DPCSettings::CurrentPath(), DPCSettings::DefaultDocPath()).toString();
	QString currentOpenDlgFilter = settings.value(DPCSettings::SelectedInputFilter(), DPCSettings::AllFileFilters()).toString();

	// Add all available file I/O filters (with import capabilities)
	QStringList fileFilters;
	fileFilters.append(DPCSettings::AllFileFilters());
	bool defaultFilterFound = false;
	{
		for (const FileIOFilter::Shared &filter : FileIOFilter::GetFilters())
		{
			if (filter->importSupported())
			{
				const QStringList	fileFilterList = filter->getFileFilters(true);

				for (const QString &fileFilter : fileFilterList)
				{
					fileFilters.append(fileFilter);
					//is it the (last) default filter?
					if (!defaultFilterFound && (currentOpenDlgFilter == fileFilter))
					{
						defaultFilterFound = true;
					}
				}
			}
		}
	}

	//default filter is still valid?
	if (!defaultFilterFound)
		currentOpenDlgFilter = DPCSettings::AllFileFilters();

	//file choosing dialog
	QStringList selectedFiles = QFileDialog::getOpenFileNames(this,
		"Open file(s)",
		currentPath,
		fileFilters.join(DPCSettings::FileFiltersSeparators()),
		&currentOpenDlgFilter,
		DPCFileDialogOptions());

	if (selectedFiles.isEmpty())
		return;

	//save last loading parameters
	currentPath = QFileInfo(selectedFiles[0]).absolutePath();
	settings.setValue(DPCSettings::CurrentPath(), currentPath);
	settings.setValue(DPCSettings::SelectedInputFilter(), currentOpenDlgFilter);
	settings.endGroup();

	if (currentOpenDlgFilter == DPCSettings::AllFileFilters())
		currentOpenDlgFilter.clear(); //this way FileIOFilter will try to guess the file type automatically!

	//load files
	//addToDB(selectedFiles, currentOpenDlgFilter);
}