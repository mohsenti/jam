//
// Created by mohsen on 6/11/16.
//

#ifndef JAM_MAINWINDOW_H
#define JAM_MAINWINDOW_H


#include <QtWidgets>
#include <QtWidgets/QMainWindow>
#include <QAction>
#include <Database/DatabaseIO.h>


class MainWindow : public QMainWindow {
public:

    MainWindow(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());

    virtual ~MainWindow();

private:
    void buildCategoryTree();
    void buildCategoryTreeChildren(QStandardItem *standardItem, int parentId);
    void buildPoemList(int id);
    void buildVerseList(int id);
    void removeLayoutWidgets();
    //menus
    QMenu *fileMenu;
    QAction *closeAction;

    //components
    QTreeView *treeView;
    QStandardItemModel *treeViewModel;
    QSplitter *splitter;
    QScrollArea *scrollArea;

    //layouts
    QVBoxLayout *mainLayout;
    QVBoxLayout *poemLayout;
    QGridLayout *tableLayout;

    //layout containers
    QWidget *mainLayoutContainer,*otherContainer;

    //garbage


    //database
    DatabaseIO *dbIO;

private slots:
    void onTreeViewItemActive(const QModelIndex &index);
    void onPoemClicked();
};


#endif //JAM_MAINWINDOW_H
