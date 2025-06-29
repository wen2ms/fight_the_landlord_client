#include "login.h"

#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "ui_login.h"
#include "datamanager.h"

Login::Login(QWidget* parent) : QDialog(parent), ui(new Ui::Login) {
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->home_btn, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(0); });

    connect(ui->register_user, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(1); });

    connect(ui->net_config_btn, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(2); });

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
    
    connect(ui->login_btn, &QPushButton::clicked, this, &Login::on_login_btn_clicked);
    connect(ui->reg_btn, &QPushButton::clicked, this, &Login::on_reg_btn_clicked);
    connect(ui->confirm_btn, &QPushButton::clicked, this, &Login::on_confirm_btn_clicked);
    
}

Login::~Login() {
    delete ui;
}

bool Login::verfify_data(QLineEdit* edit) {
    if (edit->hasAcceptableInput()) {
        edit->setStyleSheet("none");
        
        return true;
    } else {
        edit->setStyleSheet("border: 2px solid red;");
        
        return false;
    }
}

void Login::on_login_btn_clicked() {
    bool valid_name = verfify_data(ui->username);
    bool valid_pwd = verfify_data(ui->password);
    
    if (valid_name && valid_pwd) {
        
    }
}

void Login::on_reg_btn_clicked() {
    bool valid_reg_name = verfify_data(ui->reg_username);
    bool valid_reg_pwd = verfify_data(ui->reg_password);
    bool valid_reg_phone = verfify_data(ui->phone);
    
    
    if (valid_reg_name && valid_reg_pwd && valid_reg_phone) {
        
    }
}

void Login::on_confirm_btn_clicked() {
    bool valid_host = verfify_data(ui->host_addr);
    bool valid_port = verfify_data(ui->port);
    
    if (valid_host && valid_port) {
        DataManager* manager = DataManager::get_instance();
        
        manager->set_ip(ui->host_addr->text().toUtf8());
        manager->set_port(ui->port->text().toUtf8());        
    }
}
