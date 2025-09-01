#include "mybutton.h"

#include <QPainter>

MyButton::MyButton(QWidget *parent) : QPushButton{parent} ,is_disable_(false) {}

void MyButton::set_image(const QString& normal, const QString& hovering, const QString& pressed, const QString& disable) {
    normal_ = normal;
    hovering_ = hovering;
    pressed_ = pressed;
    disable_ = disable;
    image_.load(normal_);
}

void MyButton::set_button_disable(bool disable) {
    is_disable_ = disable;
    setDisabled(disable);
    QString picture = disable ? disable_ : normal_;
    image_.load(picture);
    update();    
}

void MyButton::mousePressEvent(QMouseEvent *event) {
    if (is_disable_) {
        return;
    }
    
    if (event->button() == Qt::LeftButton) {
        image_.load(pressed_);
        update();
    }
    
    QPushButton::mousePressEvent(event);
}

void MyButton::mouseReleaseEvent(QMouseEvent *event) {
    if (is_disable_) {
        return;
    }
    
    if (event->button() == Qt::LeftButton) {
        image_.load(normal_);
        update();
    }
    
    QPushButton::mouseReleaseEvent(event);
}

void MyButton::enterEvent(QEnterEvent *event) {
    if (is_disable_) {
        return;
    }
    
    image_.load(hovering_);
    update();
}

void MyButton::leaveEvent(QEvent *event) {
    if (is_disable_) {
        return;
    }
    
    image_.load(normal_);
    update();
}

void MyButton::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    painter.drawPixmap(rect(), image_);
}
