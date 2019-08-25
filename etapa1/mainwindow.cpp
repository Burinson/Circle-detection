#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <stack>
#include <QOpenGLWidget>

#define TOP(p) p.first, p.second-1
#define RIGHT(p) p.first+1, p.second
#define BOTTOM(p) p.first, p.second+1
#define LEFT(p) p.first-1, p.second

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();

}

MainWindow::~MainWindow()
{
    delete ui;
}

pair<int, int> xMid(QImage image, QRgb color) {
    pair<int, int> xy = make_pair(-1, -1);
    for(int y = 0; y < image.height(); ++y)
        for(int x = 0; x < image.width(); ++x)
            if (image.pixelColor(x, y).rgb() == (QRgb)color) {
                int x_left = x;
                int x_right = x;
                while(image.pixelColor(x_right, y).rgb() == (QRgb)color)
                    x_right++;
                x_right--;
                xy.first = (x_left + x_right) / 2;
                xy.second = y;
                return xy;
            }
    return xy;
}

pair<int, int> antixMid(QImage image, QRgb color) {
    pair<int, int> xy = make_pair(-1, -1);
    for(int y = image.height()-1; y > 0; --y)
        for(int x = image.width()-1; x > 0; --x)
            if (image.pixelColor(x, y).rgb() == (QRgb)color) {
                int x_left = x;
                int x_right = x;
                while(image.pixelColor(x_left, y).rgb() == (QRgb)color)
                    x_left--;
                x_left++;
                xy.first = (x_left + x_right) / 2;
                xy.second = y;
                return xy;
            }
    return xy;
}

pair<int, int> yMid(QImage image, QRgb color) {
    pair<int, int> xy = make_pair(-1, -1);
    for(int x = 0; x < image.width(); ++x)
        for(int y = 0; y < image.height(); ++y)
            if (image.pixelColor(x, y).rgb() == (QRgb)color) {
                int y_up = y;
                int y_down = y;
                while(image.pixelColor(x, y_down).rgb() == (QRgb)color)
                    y_down++;
                y_down--;
                xy.first = x;
                xy.second = (y_down + y_up) / 2;
                return xy;
            }
    return xy;
}

pair<int, int> antiyMid(QImage image, QRgb color) {
    pair<int, int> xy = make_pair(-1, -1);
    for(int x = image.width()-1; x > 0; --x)
        for(int y = image.height()-1; y > 0; --y)
            if (image.pixelColor(x, y).rgb() == (QRgb)color) {
                int y_up = y;
                int y_down = y;
                while(image.pixelColor(x, y_up).rgb() == (QRgb)color)
                    y_up--;
                y_up++;
                xy.first = x;
                xy.second = (y_down + y_up) / 2;
                return xy;
            }
    return xy;
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

bool isCircle(int radio1, int radio2) {
    return abs(radio1 - radio2) <= 10;
}

pair<int, int> searchBlack(QImage image) {
    for(int y = 0; y < image.height(); ++y)
        for(int x = 0; x < image.width(); ++x)
            if (image.pixelColor(x, y).black() == 255)
                return make_pair(x, y);
    return make_pair(-1, -1);
}
int BFS(QImage &image, QRgb color, pair<int, int> s, int colorChange, int islands) {
    if (s.first == -1) {
        return islands;
    }
    stack<pair<int, int>> queue;
    queue.push(s);

    while(!queue.empty()){
        s = queue.top();
        image.setPixel(s.first, s.second, color);
        queue.pop();
        if (image.pixelColor(TOP(s)).black() == 255) {
            pair<int, int> sCopy = s;
            sCopy.second--;
            queue.push(sCopy);
        }
        if (image.pixelColor(RIGHT(s)).black() == 255) {
            pair<int, int> sCopy = s;
            sCopy.first++;
            queue.push(sCopy);
        }
        if (image.pixelColor(BOTTOM(s)).black() == 255) {
            pair<int, int> sCopy = s;
            sCopy.second++;
            queue.push(sCopy);
        }
        if (image.pixelColor(LEFT(s)).black() == 255) {
            pair<int, int> sCopy = s;
            sCopy.first--;
            queue.push(sCopy);
        }
    }

    return BFS(image, qRgb(colorChange+2, colorChange+2, colorChange+2), searchBlack(image), colorChange+2, islands+1);
}

void paintItWhite(QImage &image) {
    for(int y = 0; y < image.height(); ++y)
        for(int x = 0; x < image.width(); ++x)
            if (image.pixelColor(x, y).black() != 255 && image.pixelColor(x, y).black() != 0)
                image.setPixel(x, y, qRgb(255, 255, 255));
}



void MainWindow::on_openFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/uriel/Desktop/Seminario de algoritmia/etapa1/etapa1", tr("Image Files (*.png)"));

    QImage image = QImage(fileName);
    QImage copy = image;


    QGraphicsScene *graphic = new QGraphicsScene(this);
    graphic->addPixmap(QPixmap::fromImage(image));
    ui->graphicsViewOriginal->setScene(graphic);
    //ui->graphicsViewOriginal->fitInView(graphic->sceneRect(),Qt::KeepAspectRatio);

    paintItWhite(copy);
    int islands = BFS(copy, qRgb(2, 2, 2), searchBlack(copy), 2, 0);
    qDebug() << islands;

    QRgb color;

    for(int i = 1; i <= islands; ++i) {
        color = qRgb(2*i, 2*i, 2*i);
        // Puntos fundamentales
        pair<int, int> top = xMid(copy, color);
        pair<int, int> bot = antixMid(copy, color);
        pair<int, int> left = yMid(copy, color);
        pair<int, int> right = antiyMid(copy, color);
        pair<int, int> center = make_pair(top.first, left.second);

        copy.setPixel(top.first, top.second, qRgb(255, 0, 0));
        copy.setPixel(left.first, left.second, qRgb(255, 0, 0));
        copy.setPixel(bot.first, bot.second, qRgb(255, 0, 0));
        copy.setPixel(right.first, right.second, qRgb(255, 0, 0));

        //Centroide
        copy.setPixel(center.first, center.second, qRgb(0, 255, 0));
        copy.setPixel(TOP(center), qRgb(0, 255, 0));
        copy.setPixel(RIGHT(center), qRgb(0, 255, 0));
        copy.setPixel(BOTTOM(center), qRgb(0, 255, 0));
        copy.setPixel(LEFT(center), qRgb(0, 255, 0));

        // Radio
        int topRadio = radio(top, center, copy);
        int leftRadio = radio(left, center, copy);

        // Circulo?
        qDebug() << "top" << top.first << top.second;
        qDebug() << "bot" << bot.first << bot.second;
        qDebug() << "left" << left.first << left.second;
        qDebug() << "right" << right.first << right.second;
        qDebug() << "center" << center.first << center.second;
        qDebug() << isCircle(topRadio, leftRadio);

    }


    graphic = new QGraphicsScene(this);
    graphic->addPixmap(QPixmap::fromImage(copy));
    ui->graphicsViewResult->setScene(graphic);
    //ui->graphicsViewResult->fitInView(graphic->sceneRect(),Qt::KeepAspectRatio);

}


