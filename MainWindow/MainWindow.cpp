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
    connect(treeView, &QTreeView::activated, this, &MainWindow::onTreeViewItemActive);
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

//    poemLayout = new QVBoxLayout;
//    poemLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
//
//    tableLayout = new QGridLayout;
//    tableLayout->setAlignment(Qt::AlignmentFlag::AlignTop);

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

MainWindow::~MainWindow() {
    removeLayoutWidgets();
}


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

    removeLayoutWidgets();
    if (otherContainer->layout())
        delete otherContainer->layout();
    poemLayout = new QVBoxLayout;
    poemLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    vector<PPoem> *data = dbIO->getPoems(id);

    {
        QPushButton *button;
        for (vector<PPoem>::const_iterator it = data->begin(); it < data->end(); it++) {
            button = new QPushButton(QString::fromStdString((*it)->title));
            button->setUserData(Qt::UserRole + 1, (QObjectUserData *) (*it));
            connect(button, &QPushButton::clicked, this, &MainWindow::onPoemClicked);
            poemLayout->addWidget(button);
        }
    }

    otherContainer->setLayout(poemLayout);
}

void MainWindow::onTreeViewItemActive(const QModelIndex &index) {
    QExtendedStandardItem *item = (QExtendedStandardItem *) treeViewModel->itemFromIndex(index);
    buildPoemList(((PCategory) (item->data()))->Id);
    qDebug() << QString::fromStdString(((PCategory) (item->data()))->name);
}

void MainWindow::buildVerseList(int id) {

    removeLayoutWidgets();

    if (otherContainer->layout())
        delete otherContainer->layout();
    tableLayout = new QGridLayout;
    tableLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    vector<PVerse> *data = dbIO->getVerses(id);

    QPushButton *next = new QPushButton("بعدی"), *prev = new QPushButton("قبلی");
    tableLayout->addWidget(prev, 0, 0);
    tableLayout->addWidget(next, 0, 1);

    for (vector<PVerse>::const_iterator it = data->begin(); it < data->end(); it++) {
        QLabel *label = new QLabel(QString::fromStdString((*it)->text));
        label->setAlignment(Qt::AlignmentFlag::AlignHCenter);
        tableLayout->addWidget(label, (*it)->vorder + 1, (*it)->position);
    }

    otherContainer->setLayout(tableLayout);
}

void MainWindow::onPoemClicked() {
    QPushButton *button = (QPushButton *) sender();
    PPoem poem = (PPoem) button->userData(Qt::UserRole + 1);
    buildVerseList((int) poem->Id);
}

void MainWindow::removeLayoutWidgets() {

    if (otherContainer->layout()) {
        QLayoutItem *item;
        QLayout *layout = otherContainer->layout();
        while ((item = layout->takeAt(0))) {
            item->widget()->setUserData(Qt::UserRole + 1, nullptr);
            delete item->widget();
            delete item;
        };
    }
}








