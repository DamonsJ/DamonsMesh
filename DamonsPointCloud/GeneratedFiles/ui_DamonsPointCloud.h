/********************************************************************************
** Form generated from reading UI file 'DamonsPointCloud.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DAMONSPOINTCLOUD_H
#define UI_DAMONSPOINTCLOUD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
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
            DamonsPointCloudClass->setObjectName(QStringLiteral("DamonsPointCloudClass"));
        DamonsPointCloudClass->resize(1218, 786);
        QFont font;
        font.setFamily(QStringLiteral("Consolas"));
        DamonsPointCloudClass->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral("Resources/luke_96.ico"), QSize(), QIcon::Normal, QIcon::Off);
        DamonsPointCloudClass->setWindowIcon(icon);
        actionOpen = new QAction(DamonsPointCloudClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon1;
        icon1.addFile(QStringLiteral("Resources/folder.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon1);
        actionOpen_Recent = new QAction(DamonsPointCloudClass);
        actionOpen_Recent->setObjectName(QStringLiteral("actionOpen_Recent"));
        QIcon icon2;
        icon2.addFile(QStringLiteral("Resources/Places_folder_recent.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_Recent->setIcon(icon2);
        centralWidget = new QWidget(DamonsPointCloudClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        mainView = new QVTKWidget(centralWidget);
        mainView->setObjectName(QStringLiteral("mainView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mainView->sizePolicy().hasHeightForWidth());
        mainView->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(mainView);

        DamonsPointCloudClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DamonsPointCloudClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1218, 21));
        QFont font1;
        font1.setFamily(QStringLiteral("Consolas"));
        font1.setPointSize(10);
        menuBar->setFont(font1);
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QStringLiteral("menuTools"));
        DamonsPointCloudClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(DamonsPointCloudClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        DamonsPointCloudClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(DamonsPointCloudClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DamonsPointCloudClass->setStatusBar(statusBar);
        ModelTreeDockWidget = new QDockWidget(DamonsPointCloudClass);
        ModelTreeDockWidget->setObjectName(QStringLiteral("ModelTreeDockWidget"));
        ModelTreeDockWidget->setMinimumSize(QSize(146, 280));
        ModelTreeDockWidget->setFont(font);
        ModelTreeDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetModelTree = new QWidget();
        dockWidgetModelTree->setObjectName(QStringLiteral("dockWidgetModelTree"));
        verticalLayout = new QVBoxLayout(dockWidgetModelTree);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        ModelTreeView = new QTreeView(dockWidgetModelTree);
        ModelTreeView->setObjectName(QStringLiteral("ModelTreeView"));

        verticalLayout->addWidget(ModelTreeView);

        ModelTreeDockWidget->setWidget(dockWidgetModelTree);
        DamonsPointCloudClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), ModelTreeDockWidget);
        PropertiesDockWidget = new QDockWidget(DamonsPointCloudClass);
        PropertiesDockWidget->setObjectName(QStringLiteral("PropertiesDockWidget"));
        PropertiesDockWidget->setMinimumSize(QSize(146, 175));
        PropertiesDockWidget->setFont(font);
        PropertiesDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetProperties = new QWidget();
        dockWidgetProperties->setObjectName(QStringLiteral("dockWidgetProperties"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetProperties);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        propertiesTreeView = new QTreeView(dockWidgetProperties);
        propertiesTreeView->setObjectName(QStringLiteral("propertiesTreeView"));

        verticalLayout_3->addWidget(propertiesTreeView);

        PropertiesDockWidget->setWidget(dockWidgetProperties);
        DamonsPointCloudClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), PropertiesDockWidget);
        ConsoleDockWidget = new QDockWidget(DamonsPointCloudClass);
        ConsoleDockWidget->setObjectName(QStringLiteral("ConsoleDockWidget"));
        ConsoleDockWidget->setMinimumSize(QSize(100, 140));
        ConsoleDockWidget->setFont(font);
        dockWidgetConsole = new QWidget();
        dockWidgetConsole->setObjectName(QStringLiteral("dockWidgetConsole"));
        verticalLayout_4 = new QVBoxLayout(dockWidgetConsole);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        consoleListWidget = new QListWidget(dockWidgetConsole);
        consoleListWidget->setObjectName(QStringLiteral("consoleListWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(consoleListWidget->sizePolicy().hasHeightForWidth());
        consoleListWidget->setSizePolicy(sizePolicy1);

        verticalLayout_4->addWidget(consoleListWidget);

        ConsoleDockWidget->setWidget(dockWidgetConsole);
        DamonsPointCloudClass->addDockWidget(static_cast<Qt::DockWidgetArea>(8), ConsoleDockWidget);

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
        DamonsPointCloudClass->setWindowTitle(QApplication::translate("DamonsPointCloudClass", "DamonsPointCloud", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("DamonsPointCloudClass", "&Open", Q_NULLPTR));
        actionOpen->setIconText(QApplication::translate("DamonsPointCloudClass", "Open", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("DamonsPointCloudClass", "Open", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        actionOpen->setStatusTip(QApplication::translate("DamonsPointCloudClass", "Open", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_SHORTCUT
        actionOpen->setShortcut(QApplication::translate("DamonsPointCloudClass", "Ctrl+O", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionOpen_Recent->setText(QApplication::translate("DamonsPointCloudClass", "&Open Recent", Q_NULLPTR));
        actionOpen_Recent->setIconText(QApplication::translate("DamonsPointCloudClass", "Open Recent", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        actionOpen_Recent->setStatusTip(QApplication::translate("DamonsPointCloudClass", "Open Recent", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        menuFile->setTitle(QApplication::translate("DamonsPointCloudClass", "&File", Q_NULLPTR));
        menuEdit->setTitle(QApplication::translate("DamonsPointCloudClass", "Edit", Q_NULLPTR));
        menuTools->setTitle(QApplication::translate("DamonsPointCloudClass", "Tools", Q_NULLPTR));
        ModelTreeDockWidget->setWindowTitle(QApplication::translate("DamonsPointCloudClass", "ModelTree", Q_NULLPTR));
        PropertiesDockWidget->setWindowTitle(QApplication::translate("DamonsPointCloudClass", "Properties", Q_NULLPTR));
        ConsoleDockWidget->setWindowTitle(QApplication::translate("DamonsPointCloudClass", "Console", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DamonsPointCloudClass: public Ui_DamonsPointCloudClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DAMONSPOINTCLOUD_H
