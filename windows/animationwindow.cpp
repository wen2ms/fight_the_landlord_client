#include "animationwindow.h"

#include <QPainter>
#include <QTimer>

AnimationWindow::AnimationWindow(QWidget *parent) : QWidget{parent} {
    x_ = 0;
}

void AnimationWindow::show_bid_points(int points) {
    x_ = 0;
    
    if (points == 1) {
        image_.load(":/images/score1.png");
    } else if (points == 2) {
        image_.load(":/images/score2.png");
    } else if (points == 3) {
        image_.load(":/images/score3.png");
    }
    
    update();
    
    QTimer::singleShot(2000, this, &AnimationWindow::hide);
}

void AnimationWindow::show_sequence(SeqType type) {
    x_ = 0;
    
    QString name;
    if (type == SeqType::kSeqSingle) {
        name = ":/images/shunzi.png";
    } else if (type == SeqType::kSeqPair) {
        name = ":/images/liandui.png";
    }
    
    image_.load(name);
    update();
    QTimer::singleShot(2000, this, &AnimationWindow::hide);
}

void AnimationWindow::show_joker_bomb() {
    frame_index_ = 0;
    x_ = 0;
    
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        frame_index_++;
        if (frame_index_ > 8) {
            timer->stop();
            timer->deleteLater();
            frame_index_ = 8;
            hide();
        }
        QString name = QString(":/images/joker_bomb_%1.png").arg(frame_index_);
        image_.load(name);
        update();
    });
    timer->start(60);
}

void AnimationWindow::show_bomb() {
    frame_index_ = 0;
    x_ = 0;
    
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        frame_index_++;
        if (frame_index_ > 12) {
            timer->stop();
            timer->deleteLater();
            frame_index_ = 12;
            hide();
        }
        QString name = QString(":/images/bomb_%1.png").arg(frame_index_);
        image_.load(name);
        update();
    });
    timer->start(60);
}

void AnimationWindow::show_plane() {
    x_ = width();
    image_.load(":/images/plane_1.png");
    setFixedHeight(image_.height());
    update();
    
    int step = width() / 5;
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        static int distance = 0;
        static int times = 0;
        
        distance += 5;
        if (distance >= step) {
            distance = 0;
            times++;
            QString name = QString(":/images/plane_%1.png").arg(times % 5 + 1);
            image_.load(name);
        }
        
        if (x_ <= 110) {
            timer->stop();
            timer->deleteLater();
            distance = times = 0;
            hide();
        }
        
        x_ -= 5;
        update();
    });
    
    timer->start(15);
}

void AnimationWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    painter.drawPixmap(x_, 0, image_.width(), image_.height(), image_);
}
