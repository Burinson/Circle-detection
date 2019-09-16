#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <qstandarditemmodel.h>
#include <set>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openFile_clicked();
    void mousePressEvent(QMouseEvent *event);


private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    QStandardItemModel *model2;
    QString fileName;
    bool openFile = true;
    set<int> trash;
    int deletex;
    int deletey;
};

#endif // MAINWINDOW_H
