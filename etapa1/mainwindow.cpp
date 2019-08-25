#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <stack>
#include <QOpenGLWidget>
#include <math.h>

#define TOP(p) p.first, p.second-1
#define RIGHT(p) p.first+1, p.second
#define BOTTOM(p) p.first, p.second+1
#define LEFT(p) p.first-1, p.second
#define RED qRgb(255, 0, 0)
#define GREEN qRgb(0, 255, 0)
#define BLUE qRgb(0, 0, 255)
#define BLACK qRgb(0, 0, 0)
#define WHITE qRgb(255, 255, 255)

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
                xy.second = y+1;
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
                xy.first = x+1;
                xy.second = (y_down + y_up) / 2;
                return xy;
            }
    return xy;
}

QRgb radiusColor = GREEN;
QRgb centroidColor = RED;

int radiusLen(pair<int, int> point, pair<int, int> center) {
    int maximum;
    int minimum;
    if (point.first == center.first) {
        maximum = max(point.second, center.second);
        minimum = min(point.second, center.second);
    } else if (point.second == center.second) {
        maximum = max(point.first, center.first);
        minimum = min(point.first, center.first);
    }
    return maximum - minimum;
}

void drawRadius(pair<int, int> point, pair<int, int> center, QImage &image) {
    int maximum;
    int minimum;
    if (point.first == center.first) {
        maximum = max(point.second, center.second);
        minimum = min(point.second, center.second);
        for(int y = minimum; y < maximum; ++y) {
            image.setPixel(point.first, y, radiusColor);
        }
    } else if (point.second == center.second) {
        maximum = max(point.first, center.first);
        minimum = min(point.first, center.first);
        for(int x = minimum; x < maximum; ++x) {
            image.setPixel(x, point.second, radiusColor);
        }
    }
}

bool isCircle(int radio1, int radio2) {
    return abs(radio1 - radio2) <= 1;
}

pair<int, int> searchColor(QImage image, QRgb color) {
    for(int y = 0; y < image.height(); ++y)
        for(int x = 0; x < image.width(); ++x)
            if (QRgb(image.pixelColor(x, y).rgb()) == QRgb(color))
                return make_pair(x, y);
    return make_pair(-1, -1);
}

bool edge(QImage image, pair<int, int> p) {
    if (!image.valid(TOP(p)) ||
        !image.valid(RIGHT(p)) ||
        !image.valid(BOTTOM(p)) ||
        !image.valid(LEFT(p))) {
        qDebug() << "Incomplete";
            return true;
    }
    return false;
}

int DFS(QImage &image, QRgb color, pair<int, int> s, int colorChange, int circles, bool complete) {
    if (s.first == -1 && s.second == -1) {
        return circles;
    }
    pair<int, int> start = s;

    stack<pair<int, int>> queue;
    queue.push(s);

    while(!queue.empty()){
        s = queue.top();
        if (edge(image, s) && complete) {
            return DFS(image, BLUE, start, colorChange-2, circles-1, false);

        }
        image.setPixel(s.first, s.second, color);
        queue.pop();
        if (image.pixelColor(TOP(s)).black() == 255) {
            pair<int, int> sCopy = s;
            sCopy.second--;
            queue.push(sCopy);
        }
        if (image.pixelColor(TOP(make_pair(s.first+1, s.second) )).black() == 255) {
            pair<int, int> sCopy = s;
            sCopy.second--;
            sCopy.first++;
            queue.push(sCopy);
        }
        if (image.pixelColor(TOP(make_pair(s.first-1, s.second) )).black() == 255) {
            pair<int, int> sCopy = s;
            sCopy.second--;
            sCopy.first--;
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
        if (image.pixelColor(BOTTOM(make_pair(s.first-1, s.second))).black() == 255) {
            pair<int, int> sCopy = s;
            sCopy.second++;
            sCopy.first--;
            queue.push(sCopy);
        }
        if (image.pixelColor(BOTTOM(make_pair(s.first+1, s.second))).black() == 255) {
            pair<int, int> sCopy = s;
            sCopy.second++;
            sCopy.first++;
            queue.push(sCopy);
        }
        if (image.pixelColor(LEFT(s)).black() == 255) {
            pair<int, int> sCopy = s;
            sCopy.first--;
            queue.push(sCopy);
        }
    }

    return DFS(image, qRgb(colorChange+2, colorChange+2, colorChange+2), searchColor(image, BLACK), colorChange+2, circles+1, true);
}

void deleteOval(QImage &image, QRgb color, pair<int, int> s) {
    stack<pair<int, int>> queue;
    queue.push(s);

    while(!queue.empty()){
        s = queue.top();
        image.setPixel(s.first, s.second, WHITE);
        queue.pop();
        if ((QRgb)image.pixelColor(TOP(s)).rgb() == (QRgb)color) {
            pair<int, int> sCopy = s;
            sCopy.second--;
            queue.push(sCopy);
        }
        if ((QRgb)image.pixelColor(RIGHT(s)).rgb() == (QRgb)color) {
            pair<int, int> sCopy = s;
            sCopy.first++;
            queue.push(sCopy);
        }
        if ((QRgb)image.pixelColor(BOTTOM(s)).rgb() == (QRgb)color) {
            pair<int, int> sCopy = s;
            sCopy.second++;
            queue.push(sCopy);
        }
        if ((QRgb)image.pixelColor(LEFT(s)).rgb() == (QRgb)color) {
            pair<int, int> sCopy = s;
            sCopy.first--;
            queue.push(sCopy);
        }
    }

}
void clean(QImage &image) {
    for(int y = 0; y < image.height(); ++y)
        for(int x = 0; x < image.width(); ++x)
            if (image.pixelColor(x, y).black() != 255 && QRgb(image.pixelColor(x, y).rgb()) != QRgb(WHITE))
                image.setPixel(x, y, BLACK);
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

    clean(copy);
    int circles = DFS(copy, qRgb(2, 2, 2), searchColor(copy, BLACK), 2, 0, true);

    QRgb color;

    for(int i = 1; i <= circles; ++i) {
        color = qRgb(2*i, 2*i, 2*i);
        // Puntos fundamentales
        pair<int, int> top = xMid(copy, color);
        pair<int, int> bot = antixMid(copy, color);
        pair<int, int> right = antiyMid(copy, color);
        pair<int, int> left = yMid(copy, color);

        // Ajustar diferencias
        top.first = max(top.first, bot.first);
        bot.first = top.first;
        left.second = max(left.second, right.second);
        right.second = left.second;

        pair<int, int> center = make_pair(top.first, left.second);

        // Radio
        int topRadio = radiusLen(top, center);
        int rightRadio = radiusLen(right, center);
        int botRadio = radiusLen(bot, center);
        int leftRadio = radiusLen(left, center);

        // Posiciones
        qDebug() << "Top:" << top;
        qDebug() << "Right:" << right;
        qDebug() << "Down:" << bot;
        qDebug() << "Left:" << left;
        qDebug() << "Center" << center;

        // Diferencia entre radios
        int topDiff = abs(topRadio - rightRadio);
        int rightDiff = abs(rightRadio - botRadio);
        int botDiff = abs(botRadio - leftRadio);
        int leftDiff = abs(leftRadio - topRadio);
        int totalDiff = topDiff + rightDiff + botDiff + leftDiff;
        qDebug() << "Diferencia de radios" << i << "->" << totalDiff;

        // Eliminar óvalos
        if (totalDiff > 10) {
            deleteOval(copy, color, searchColor(copy, color));
        } else {
            // Dibujar radio
            drawRadius(top, center, copy);
            drawRadius(right, center, copy);
            drawRadius(bot, center, copy);
            drawRadius(left, center, copy);

            // Dibujar Centroide
            copy.setPixel(center.first, center.second, centroidColor);
            copy.setPixel(TOP(center), centroidColor);
            copy.setPixel(RIGHT(center), centroidColor);
            copy.setPixel(BOTTOM(center), centroidColor);
            copy.setPixel(LEFT(center), centroidColor);
            copy.setPixel(TOP(make_pair(center.first, center.second-1)), centroidColor);
            copy.setPixel(RIGHT(make_pair(center.first+1, center.second)), centroidColor);
            copy.setPixel(BOTTOM(make_pair(center.first, center.second+1)), centroidColor);
            copy.setPixel(LEFT(make_pair(center.first-1, center.second)), centroidColor);
        }
    }

    graphic = new QGraphicsScene(this);
    graphic->addPixmap(QPixmap::fromImage(copy));
    ui->graphicsViewResult->setScene(graphic);

}


