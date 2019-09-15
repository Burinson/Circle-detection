#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <qstandarditemmodel.h>
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


private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    QStandardItemModel *model2;
};

#endif // MAINWINDOW_H
