#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

pair<int, int> xMid(QImage image) {
    pair<int, int> xy;
    for(int y = 0; y < image.height(); ++y) {
        for(int x = 0; x < image.width(); ++x) {
            if (image.pixelColor(x, y).black() != 0) {

                int x_left = x;
                int x_right = x;

                while(image.pixelColor(x_right, y).black() != 0) {
                    x_right++;
                }
                x_right--;

                int x_mid = (x_left + x_right) / 2;
                xy.first = x_mid;
                xy.second = y;
                return xy;
            }
        }
    }
}

pair<int, int> yMid(QImage image) {
    pair<int, int> xy;
    for(int x = 0; x < image.width(); ++x) {
        for(int y = 0; y < image.height(); ++y) {
            if (image.pixelColor(x, y).black() != 0) {

                int y_up = y;
                int y_down = y;

                while(image.pixelColor(x, y_down).black() != 0) {
                    y_down++;
                }
                y_down--;

                int y_mid = (y_down + y_up) / 2;
                xy.first = x;
                xy.second = y_mid;
                return xy;
            }
        }
    }
}

void MainWindow::on_openFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/uriel/Desktop/Seminario de algoritmia/etapa1/etapa1", tr("Image Files (*.png *.jpg *.bmp)"));

    QImage image = QImage(fileName);
    QImage copy = image;
    copy.setPixel(xMid(image).first, xMid(image).second, qRgb(255, 0, 0));
    copy.setPixel(yMid(image).first, yMid(image).second, qRgb(255, 0, 0));

    QGraphicsScene *graphic = new QGraphicsScene(this);
    graphic->addPixmap(QPixmap::fromImage(copy));
    ui->graphicsView->setScene(graphic);
    ui->graphicsView->fitInView(graphic->sceneRect(),Qt::KeepAspectRatio);

}

