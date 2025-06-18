#include "endpanel.h"

#include <QPainter>

EndPanel::EndPanel(bool is_lord, bool is_win, QWidget *parent) : QWidget{parent} {
    background_image_.load(":/images/gameover.png");
    setFixedSize(background_image_.size());
    
    title_ = new QLabel(this);
    if (is_lord) {
        if (is_win) {
            title_->setPixmap(QPixmap(":/images/lord_win.png"));
        } else {
            title_->setPixmap(QPixmap(":/images/lord_fail.png"));
        }
    } else {
        if (is_win) {
            title_->setPixmap(QPixmap(":/images/farmer_win.png"));
        } else {
            title_->setPixmap(QPixmap(":/images/farmer_fail.png"));
        }
    }
    title_->move(125, 125);
    
    score_ = new ScorePanel(this);
    score_->move(75, 230);
    score_->setFixedSize(260, 160);
    score_->set_score_font_size(18);
    score_->set_score_font_color(ScorePanel::FontColor::kRed);
    
    continue_ = new QPushButton(this);
    continue_->move(84, 429);
    QString style_sheet = R"(
        QPushButton{border-image: url(:/images/button_normal.png)}
        QPushButton:hover{border-image: url(:/images/button_hover.png)}
        QPushButton:pressed{border-image: url(:/images/button_pressed.png)}  
    )";
    continue_->setStyleSheet(style_sheet);
    continue_->setFixedSize(231, 48);
    
    connect(continue_, &QPushButton::clicked, this, &EndPanel::continue_game);
}

void EndPanel::set_scores(int left_score, int right_score, int my_score) {
    score_->set_scores(left_score, right_score, my_score);
}

void EndPanel::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    painter.drawPixmap(rect(), background_image_);
}
