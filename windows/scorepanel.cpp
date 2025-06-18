#include "scorepanel.h"

#include "ui_scorepanel.h"

const QString kFontColor[] = {"black", "white", "red", "blue", "green"};

ScorePanel::ScorePanel(QWidget *parent) : QWidget(parent), ui(new Ui::ScorePanel) {
    ui->setupUi(this);
    
    labels_list_ << ui->my_score << ui->left_score << ui->right_score << ui->my_title << ui->left_title << ui->right_title
                 << ui->points1 << ui->points2 << ui->points3;
}

ScorePanel::~ScorePanel() {
    delete ui;
}

void ScorePanel::set_scores(int left_score, int right_score, int my_score) {
    ui->left_score->setText(QString::number(left_score));
    ui->right_score->setText(QString::number(right_score));
    ui->my_score->setText(QString::number(my_score));
}

void ScorePanel::set_score_font_size(int point) {
    QFont font("AppleGothic", point, QFont::Bold);
    
    for (int i = 0; i < labels_list_.size(); ++i) {
        labels_list_[i]->setFont(font);
    }
}

void ScorePanel::set_score_font_color(FontColor color) {
    QString style_sheet = QString("QLabel{color:%1}").arg(kFontColor[color]);    
    for (int i = 0; i < labels_list_.size(); ++i) {
            
        labels_list_[i]->setStyleSheet(style_sheet);
    }
}
