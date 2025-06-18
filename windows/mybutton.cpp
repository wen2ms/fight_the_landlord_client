#include "mybutton.h"

#include <QPainter>

MyButton::MyButton(QWidget *parent) : QPushButton{parent} {}

void MyButton::set_image(QString normal, QString hovering, QString pressed) {
    normal_ = normal;
    hovering_ = hovering;
    pressed_ = pressed;
    
    image_.load(normal_);
}

void MyButton::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        image_.load(pressed_);
        update();
    }
    
    QPushButton::mousePressEvent(event);
}

void MyButton::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        image_.load(normal_);
        update();
    }
    
    QPushButton::mouseReleaseEvent(event);
}

void MyButton::enterEvent(QEnterEvent *event) {
    image_.load(hovering_);
    update();
}

void MyButton::leaveEvent(QEvent *event) {
    image_.load(normal_);
    update();
}

void MyButton::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    painter.drawPixmap(rect(), image_);
}
