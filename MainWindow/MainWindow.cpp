//
// Created by mohsen on 6/11/16.
//

#include <Database/DatabaseModels.h>
#include <QDebug>
#include <QtHelpers/QExtendedStandardItem.h>
#include "MainWindow.h"

//Todo:destroy allocated memories

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags) {
    this->setLayoutDirection(Qt::LayoutDirection::RightToLeft);

    dbIO = DatabaseIO::getInstance();

    //Init Menus
    fileMenu = new QMenu("پرونده");
    menuBar()->addMenu(fileMenu);

    //Init Actions
    closeAction = new QAction("خروج", fileMenu);
    connect(closeAction, &QAction::triggered, this, &MainWindow::close);
    fileMenu->addAction(closeAction);

    //Build TreeView
    treeView = new QTreeView();
    treeViewModel = new QStandardItemModel();
    treeView->setModel(treeViewModel);
    treeView->setHeaderHidden(true);
    connect(treeView,&QTreeView::activated,this,&MainWindow::onTreeViewItemActive);
    //Todo: set rtl indicator

    buildCategoryTree();


    //Build Layouts
    mainLayoutContainer = new QWidget;
    mainLayout = new QVBoxLayout;
    mainLayoutContainer->setLayout(mainLayout);

    otherContainer = new QWidget;
    scrollArea = new QScrollArea;
    scrollArea->setWidget(otherContainer);
    scrollArea->setWidgetResizable(true);
    poemLayout = new QVBoxLayout(otherContainer);
    poemLayout->setAlignment(Qt::AlignmentFlag::AlignTop);

    //Build Splitter
    splitter = new QSplitter;
    splitter->addWidget(treeView);
    splitter->addWidget(scrollArea);
    QList<int> sizes;
    sizes << 100 << 300;
    splitter->setSizes(sizes);

    mainLayout->addWidget(splitter);
    setCentralWidget(mainLayoutContainer);
}

MainWindow::~MainWindow() { }


void MainWindow::buildCategoryTree() {
    vector<PCategory> *data = dbIO->getCategories(0);
    for (vector<PCategory>::const_iterator it = data->begin(); it < data->end(); it++) {
        QExtendedStandardItem *item = new QExtendedStandardItem(QString::fromStdString((*it)->name));
        item->setEditable(false);
        item->setData((*it));
        buildCategoryTreeChildren(item, (int) (*it)->Id);
        treeViewModel->appendRow(item);
    }
}

void MainWindow::buildCategoryTreeChildren(QStandardItem *standardItem, int parentId) {
    vector<PCategory> *data = dbIO->getCategories(parentId);
    for (vector<PCategory>::const_iterator it = data->begin(); it < data->end(); it++) {
        QExtendedStandardItem *item = new QExtendedStandardItem(QString::fromStdString((*it)->name));
        item->setEditable(false);
        item->setData((*it));
        buildCategoryTreeChildren(item, (int) (*it)->Id);
        standardItem->appendRow(item);
    }
}

void MainWindow::buildPoemList(int id) {
    vector<PPoem> *data = dbIO->getPoems(id);

    {
        QLayoutItem *item;
        while ((item = poemLayout->takeAt(0))) {
            delete item->widget();
            delete item;
        };
    }

    {
        QPushButton *button;
        for (vector<PPoem>::const_iterator it = data->begin(); it < data->end(); it++) {
            button = new QPushButton(QString::fromStdString((*it)->title));
            poemLayout->addWidget(button);
        }
    }

    otherContainer->setLayout(poemLayout);
}

void MainWindow::onTreeViewItemActive(const QModelIndex &index) {
    QExtendedStandardItem *item = (QExtendedStandardItem *) treeViewModel->itemFromIndex(index);
    buildPoemList(((PCategory)(item->data()))->Id);
    qDebug() << QString::fromStdString(((PCategory)(item->data()))->name);
}


