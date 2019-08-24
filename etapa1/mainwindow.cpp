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
    for(int y = 0; y < image.height(); ++y)
        for(int x = 0; x < image.width(); ++x)
            if (image.pixelColor(x, y).black() != 0) {
                int x_left = x;
                int x_right = x;
                while(image.pixelColor(x_right, y).black() != 0)
                    x_right++;
                x_right--;
                xy.first = (x_left + x_right) / 2;
                xy.second = y;
                return xy;
            }
}

pair<int, int> antixMid(QImage image) {
    pair<int, int> xy;
    for(int y = image.height()-1; y > 0; --y)
        for(int x = image.width()-1; x > 0; --x)
            if (image.pixelColor(x, y).black() != 0) {
                int x_left = x;
                int x_right = x;
                while(image.pixelColor(x_left, y).black() != 0)
                    x_left--;
                x_left++;
                xy.first = (x_left + x_right) / 2;
                xy.second = y;
                return xy;
            }
}

pair<int, int> yMid(QImage image) {
    pair<int, int> xy;
    for(int x = 0; x < image.width(); ++x)
        for(int y = 0; y < image.height(); ++y)
            if (image.pixelColor(x, y).black() != 0) {
                int y_up = y;
                int y_down = y;
                while(image.pixelColor(x, y_down).black() != 0)
                    y_down++;
                y_down--;
                xy.first = x;
                xy.second = (y_down + y_up) / 2;
                return xy;
            }
}

pair<int, int> antiyMid(QImage image) {
    pair<int, int> xy;
    for(int x = image.width()-1; x > 0; --x)
        for(int y = image.height()-1; y > 0; --y)
            if (image.pixelColor(x, y).black() != 0) {
                int y_up = y;
                int y_down = y;
                while(image.pixelColor(x, y_up).black() != 0)
                    y_up--;
                y_up++;
                xy.first = x;
                xy.second = (y_down + y_up) / 2;
                return xy;
            }
}

int radio(pair<int, int> point, pair<int, int> center, QImage &image) {
    int maximum;
    int minimum;
    if (point.first == center.first) {
        maximum = max(point.second, center.second);
        minimum = min(point.second, center.second);
        for(int y = minimum; y < maximum; ++y) {
            image.setPixel(point.first, y, qRgb(255, 0, 0));
        }
    } else if (point.second == center.second) {
        maximum = max(point.first, center.first);
        minimum = min(point.first, center.first);
        for(int x = minimum; x < maximum; ++x) {
            image.setPixel(x, point.second, qRgb(255, 0, 0));
        }
    }
    return maximum - minimum;
}

void MainWindow::on_openFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/uriel/Desktop/Seminario de algoritmia/etapa1/etapa1", tr("Image Files (*.png *.jpg *.bmp)"));

    QImage image = QImage(fileName);
    QImage copy = image;

    // Puntos fundamentales
    pair<int, int> top = xMid(image);
    pair<int, int> bot = antixMid(image);
    pair<int, int> left = yMid(image);
    pair<int, int> right = antiyMid(image);
    pair<int, int> center = make_pair(top.first, left.second);

    copy.setPixel(top.first, top.second, qRgb(255, 0, 0));
    copy.setPixel(left.first, left.second, qRgb(255, 0, 0));
    copy.setPixel(bot.first, bot.second, qRgb(255, 0, 0));
    copy.setPixel(right.first, right.second, qRgb(255, 0, 0));
    copy.setPixel(center.first, center.second, qRgb(255, 0, 0));

    // Radio
    radio(top, center, copy);
    radio(right, center, copy);
    radio(bot, center, copy);
    radio(left, center, copy);


    QGraphicsScene *graphic = new QGraphicsScene(this);
    graphic->addPixmap(QPixmap::fromImage(copy));
    ui->graphicsView->setScene(graphic);
    ui->graphicsView->fitInView(graphic->sceneRect(),Qt::KeepAspectRatio);

}

