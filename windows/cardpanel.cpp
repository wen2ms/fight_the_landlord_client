#include "cardpanel.h"

#include <QPainter>

CardPanel::CardPanel(QWidget *parent) : QWidget{parent}, is_selected_(false), is_front_(true), player_(nullptr) {}

void CardPanel::set_image(const QPixmap& front, const QPixmap& back) {
    front_ = front;
    back_ = back;
    
    setFixedSize(front_.size());
    
    update();    
}

QPixmap CardPanel::get_image() const {
    return front_;   
}

void CardPanel::set_front_side(bool is_front) {
    is_front_ = is_front;
}

bool CardPanel::is_front_side() const {
    return is_front_;
}

void CardPanel::set_selected_side(bool is_selected) {
    is_selected_ = is_selected;
}

bool CardPanel::is_selected() const {
    return is_selected_;   
}

void CardPanel::set_card(const Card& card) {
    card_ = card;
}

Card CardPanel::get_card() const {
    return card_;
}

void CardPanel::set_owner(Player* player) {
    player_ = player;
}

Player* CardPanel::get_owner() {
    return player_;
}

void CardPanel::clicked() {
    emit card_selected(Qt::LeftButton);
}

void CardPanel::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    
    if (is_front_) {
        painter.drawPixmap(rect(), front_);
    } else {
        painter.drawPixmap(rect(), back_);
    }
}

void CardPanel::mousePressEvent(QMouseEvent* event) {
    emit card_selected(event->button());
}
