#include "loading.h"

#include <QPainter>
#include <QTimer>

#include "mainwindow.h"

Loading::Loading(QWidget *parent) : QWidget{parent} {
    background_.load(":/images/loading.png");
    setFixedSize(background_.size());
    
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground);
    
    QPixmap pixmap(":/images/progress.png");
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        progress_ = pixmap.copy(0, 0, bar_width_, pixmap.height());
        
        update();
        if (bar_width_ >= pixmap.width()) {
            timer->stop();
            timer->deleteLater();
            MainWindow* mainwindow = new MainWindow;
            mainwindow->show();
            close();
        }
        
        bar_width_ += 5;
    });
    
    timer->start(15);
}

void Loading::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    painter.drawPixmap(rect(), background_);
    painter.drawPixmap(62, 417, progress_.width(), progress_.height(), progress_);
}
