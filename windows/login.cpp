#include "login.h"

#include <QRegularExpression>
#include <QRegularExpressionValidator>

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
    
    QRegularExpression expreg("^[a-zA-Z0-9_]{3,9}$");
    QRegularExpressionValidator* validator_name = new QRegularExpressionValidator(expreg, this);
    
    ui->username->setValidator(validator_name);
    ui->reg_username->setValidator(validator_name);
    
    
    expreg.setPattern("^(?=.*[A-Z])(?=.*\\d)(?=.*[a-z])(?=.*[@$!%*?&_])[a-zA-Z\\d@$!%*?&_]{4,20}$");
    QRegularExpressionValidator* validator_pwd = new QRegularExpressionValidator(expreg, this);
    
    ui->password->setValidator(validator_pwd);
    ui->reg_password->setValidator(validator_pwd);
    
    expreg.setPattern("^1[3456789]\\d{9}$");
    QRegularExpressionValidator* validator_phone = new QRegularExpressionValidator(expreg, this);
    
    ui->phone->setValidator(validator_phone);
    
    expreg.setPattern("^((25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\.){3}(25[0-5]|2[0-4]\\d|[01]?\\d\\d?)$");
    QRegularExpressionValidator* validator_ip = new QRegularExpressionValidator(expreg, this);
    
    ui->host_addr->setValidator(validator_ip);
    
    expreg.setPattern("^(6553[0-5]|655[0-2]\\d|65[0-4]\\d{2}|6[0-4]\\d{3}|[1-9]\\d{0,3}|0)$");
    QRegularExpressionValidator* validator_port = new QRegularExpressionValidator(expreg, this);
    
    ui->port->setValidator(validator_port);
}

Login::~Login() {
    delete ui;
}
