#include "buttongroup.h"

#include "ui_buttongroup.h"

ButtonGroup::ButtonGroup(QWidget *parent) : QWidget(parent), ui(new Ui::ButtonGroup) {
    ui->setupUi(this);
}

ButtonGroup::~ButtonGroup() {
    delete ui;
}

void ButtonGroup::init_buttons() {
    ui->start->set_image(":/images/start-1.png", ":/images/start-3.png", ":/images/start-2.png");
    
    ui->play_a_card->set_image(":/images/chupai_btn-1.png", ":/images/chupai_btn-3.png", ":/images/chupai_btn-2.png");
    ui->play_a_card1->set_image(":/images/chupai_btn-1.png", ":/images/chupai_btn-3.png", ":/images/chupai_btn-2.png");
    
    ui->pass->set_image(":/images/pass_btn-1.png", ":/images/pass_btn-3.png", ":/images/pass_btn-2.png");
    
    ui->give_up->set_image(":/images/buqiang-1.png", ":/images/buqiang-3.png", ":/images/buqiang-2.png");
    
    ui->one_point->set_image(":/images/1fen-1.png", ":/images/1fen-3.png", ":/images/1fen-2.png");
    ui->two_points->set_image(":/images/2fen-1.png", ":/images/2fen-3.png", ":/images/2fen-2.png");
    ui->three_points->set_image(":/images/3fen-1.png", ":/images/3fen-3.png", ":/images/3fen-2.png");
    
    QList<MyButton*> buttons;
    
    buttons << ui->start << ui->play_a_card << ui->play_a_card1 << ui->pass << ui->give_up << ui->one_point
            << ui->two_points << ui->three_points;
    
    for (int i = 0; i < buttons.size(); ++i) {
        buttons[i]->setFixedSize(90, 45);
    }
    
    connect(ui->start, &MyButton::clicked, this, &ButtonGroup::start_game);
    connect(ui->play_a_card, &MyButton::clicked, this, &ButtonGroup::play_a_hand);
    connect(ui->play_a_card1, &MyButton::clicked, this, &ButtonGroup::play_a_hand);
    connect(ui->pass, &MyButton::clicked, this, &ButtonGroup::pass);
    
    connect(ui->give_up, &MyButton::clicked, this, [=]() {
        emit bid_points(0);
    });
    connect(ui->one_point, &MyButton::clicked, this, [=]() {
        emit bid_points(1);
    });
    connect(ui->two_points, &MyButton::clicked, this, [=]() {
        emit bid_points(2);
    });
    connect(ui->three_points, &MyButton::clicked, this, [=]() {
        emit bid_points(3);
    });
}

void ButtonGroup::select_panel(Panel panel_type, int bidding_points) {
    ui->stackedWidget->setCurrentIndex(panel_type);
    
    if (panel_type != Panel::kBidLord) {
        return;
    }
    
    if (bidding_points == 0) {
        ui->one_point->setVisible(true);
        ui->two_points->setVisible(true);
        ui->three_points->setVisible(true);
    } else if (bidding_points == 1) {
        ui->one_point->setVisible(false);
        ui->two_points->setVisible(true);
        ui->three_points->setVisible(true);
    } else if (bidding_points == 2) {
        ui->one_point->setVisible(false);
        ui->two_points->setVisible(false);
        ui->three_points->setVisible(true);
    }
}
