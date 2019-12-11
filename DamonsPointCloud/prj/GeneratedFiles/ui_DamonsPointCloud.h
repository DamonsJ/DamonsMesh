/********************************************************************************
** Form generated from reading UI file 'DamonsPointCloud.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DAMONSPOINTCLOUD_H
#define UI_DAMONSPOINTCLOUD_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_DamonsPointCloudClass
{
public:
    QAction *actionOpen;
    QAction *actionOpen_Recent;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QVTKWidget *mainView;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuTools;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *ModelTreeDockWidget;
    QWidget *dockWidgetModelTree;
    QVBoxLayout *verticalLayout;
    QTreeView *ModelTreeView;
    QDockWidget *PropertiesDockWidget;
    QWidget *dockWidgetProperties;
    QVBoxLayout *verticalLayout_3;
    QTreeView *propertiesTreeView;
    QDockWidget *ConsoleDockWidget;
    QWidget *dockWidgetConsole;
    QVBoxLayout *verticalLayout_4;
    QListWidget *consoleListWidget;

    void setupUi(QMainWindow *DamonsPointCloudClass)
    {
        if (DamonsPointCloudClass->objectName().isEmpty())
            DamonsPointCloudClass->setObjectName(QString::fromUtf8("DamonsPointCloudClass"));
        DamonsPointCloudClass->resize(1218, 786);
        QFont font;
        font.setFamily(QString::fromUtf8("Consolas"));
        DamonsPointCloudClass->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8("Resources/luke_96.ico"), QSize(), QIcon::Normal, QIcon::Off);
        DamonsPointCloudClass->setWindowIcon(icon);
        actionOpen = new QAction(DamonsPointCloudClass);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("Resources/folder.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon1);
        actionOpen_Recent = new QAction(DamonsPointCloudClass);
        actionOpen_Recent->setObjectName(QString::fromUtf8("actionOpen_Recent"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("Resources/Places_folder_recent.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_Recent->setIcon(icon2);
        centralWidget = new QWidget(DamonsPointCloudClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        mainView = new QVTKWidget(centralWidget);
        mainView->setObjectName(QString::fromUtf8("mainView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mainView->sizePolicy().hasHeightForWidth());
        mainView->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(mainView);

        DamonsPointCloudClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DamonsPointCloudClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1218, 21));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Consolas"));
        font1.setPointSize(10);
        menuBar->setFont(font1);
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        DamonsPointCloudClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(DamonsPointCloudClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        DamonsPointCloudClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(DamonsPointCloudClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        DamonsPointCloudClass->setStatusBar(statusBar);
        ModelTreeDockWidget = new QDockWidget(DamonsPointCloudClass);
        ModelTreeDockWidget->setObjectName(QString::fromUtf8("ModelTreeDockWidget"));
        ModelTreeDockWidget->setMinimumSize(QSize(146, 280));
        ModelTreeDockWidget->setFont(font);
        ModelTreeDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetModelTree = new QWidget();
        dockWidgetModelTree->setObjectName(QString::fromUtf8("dockWidgetModelTree"));
        verticalLayout = new QVBoxLayout(dockWidgetModelTree);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        ModelTreeView = new QTreeView(dockWidgetModelTree);
        ModelTreeView->setObjectName(QString::fromUtf8("ModelTreeView"));

        verticalLayout->addWidget(ModelTreeView);

        ModelTreeDockWidget->setWidget(dockWidgetModelTree);
        DamonsPointCloudClass->addDockWidget(Qt::LeftDockWidgetArea, ModelTreeDockWidget);
        PropertiesDockWidget = new QDockWidget(DamonsPointCloudClass);
        PropertiesDockWidget->setObjectName(QString::fromUtf8("PropertiesDockWidget"));
        PropertiesDockWidget->setMinimumSize(QSize(146, 175));
        PropertiesDockWidget->setFont(font);
        PropertiesDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetProperties = new QWidget();
        dockWidgetProperties->setObjectName(QString::fromUtf8("dockWidgetProperties"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetProperties);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        propertiesTreeView = new QTreeView(dockWidgetProperties);
        propertiesTreeView->setObjectName(QString::fromUtf8("propertiesTreeView"));

        verticalLayout_3->addWidget(propertiesTreeView);

        PropertiesDockWidget->setWidget(dockWidgetProperties);
        DamonsPointCloudClass->addDockWidget(Qt::LeftDockWidgetArea, PropertiesDockWidget);
        ConsoleDockWidget = new QDockWidget(DamonsPointCloudClass);
        ConsoleDockWidget->setObjectName(QString::fromUtf8("ConsoleDockWidget"));
        ConsoleDockWidget->setMinimumSize(QSize(100, 140));
        ConsoleDockWidget->setFont(font);
        dockWidgetConsole = new QWidget();
        dockWidgetConsole->setObjectName(QString::fromUtf8("dockWidgetConsole"));
        verticalLayout_4 = new QVBoxLayout(dockWidgetConsole);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        consoleListWidget = new QListWidget(dockWidgetConsole);
        consoleListWidget->setObjectName(QString::fromUtf8("consoleListWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(consoleListWidget->sizePolicy().hasHeightForWidth());
        consoleListWidget->setSizePolicy(sizePolicy1);

        verticalLayout_4->addWidget(consoleListWidget);

        ConsoleDockWidget->setWidget(dockWidgetConsole);
        DamonsPointCloudClass->addDockWidget(Qt::BottomDockWidgetArea, ConsoleDockWidget);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionOpen_Recent);
        mainToolBar->addAction(actionOpen);

        retranslateUi(DamonsPointCloudClass);

        QMetaObject::connectSlotsByName(DamonsPointCloudClass);
    } // setupUi

    void retranslateUi(QMainWindow *DamonsPointCloudClass)
    {
        DamonsPointCloudClass->setWindowTitle(QCoreApplication::translate("DamonsPointCloudClass", "DamonsPointCloud", nullptr));
        actionOpen->setText(QCoreApplication::translate("DamonsPointCloudClass", "&Open", nullptr));
        actionOpen->setIconText(QCoreApplication::translate("DamonsPointCloudClass", "Open", nullptr));
#if QT_CONFIG(tooltip)
        actionOpen->setToolTip(QCoreApplication::translate("DamonsPointCloudClass", "Open", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        actionOpen->setStatusTip(QCoreApplication::translate("DamonsPointCloudClass", "Open", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(shortcut)
        actionOpen->setShortcut(QCoreApplication::translate("DamonsPointCloudClass", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpen_Recent->setText(QCoreApplication::translate("DamonsPointCloudClass", "&Open Recent", nullptr));
        actionOpen_Recent->setIconText(QCoreApplication::translate("DamonsPointCloudClass", "Open Recent", nullptr));
#if QT_CONFIG(statustip)
        actionOpen_Recent->setStatusTip(QCoreApplication::translate("DamonsPointCloudClass", "Open Recent", nullptr));
#endif // QT_CONFIG(statustip)
        menuFile->setTitle(QCoreApplication::translate("DamonsPointCloudClass", "&File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("DamonsPointCloudClass", "Edit", nullptr));
        menuTools->setTitle(QCoreApplication::translate("DamonsPointCloudClass", "Tools", nullptr));
        ModelTreeDockWidget->setWindowTitle(QCoreApplication::translate("DamonsPointCloudClass", "ModelTree", nullptr));
        PropertiesDockWidget->setWindowTitle(QCoreApplication::translate("DamonsPointCloudClass", "Properties", nullptr));
        ConsoleDockWidget->setWindowTitle(QCoreApplication::translate("DamonsPointCloudClass", "Console", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DamonsPointCloudClass: public Ui_DamonsPointCloudClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DAMONSPOINTCLOUD_H
