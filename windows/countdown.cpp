#include "countdown.h"

#include <QPainter>

CountDown::CountDown(QWidget *parent) : QWidget{parent} {
    setFixedSize(70, 70);
    
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, [=]() {
        count_--;
        if (count_ < 10 && count_ > 0) {
            clock_.load(":/images/clock.png");
            number_ = QPixmap(":/images/number.png").copy(count_ * (30 + 10), 0, 30, 42).scaled(20, 30);
            if (count_ == 5) {
                emit not_enough_time();
            }
        } else if (count_ <= 0) {
            clock_ = QPixmap();
            number_ = QPixmap();
            timer_->stop();
            
            emit timeout();
        }
        
        update();
    });
}

void CountDown::show_count_down() {
    count_ = 15;
    timer_->start(1000);
}

void CountDown::stop_count_down() {
    timer_->stop();
    clock_ = QPixmap();
    number_ = QPixmap();
    
    update();
}

void CountDown::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    painter.drawPixmap(rect(), clock_);
    painter.drawPixmap(24, 24, number_.width(), number_.height(), number_);
}
