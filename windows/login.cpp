#include "login.h"

#include "ui_login.h"

Login::Login(QWidget *parent) : QDialog(parent), ui(new Ui::Login) {
    ui->setupUi(this);
    
    ui->stackedWidget->setCurrentIndex(0);
    
    connect(ui->home_btn, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });
    
    connect(ui->register_user, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
    });
    
    connect(ui->net_config_btn, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(2);
    });
}

Login::~Login() {
    delete ui;
}
