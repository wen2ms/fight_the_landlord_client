#include "login.h"

#include <QCryptographicHash>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QThreadPool>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

#include "datamanager.h"
#include "communication.h"
#include "mainwindow.h"
#include "ui_login.h"

Login::Login(QWidget* parent)
    : QDialog(parent), ui(new Ui::Login), is_connected_(false), key_("I have a dream that one day on the red hills of Georgia") {
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

    expreg.setPattern(R"(^((25[0-5]|2[0-4]\d|[01]?\d\d?)\.){3}(25[0-5]|2[0-4]\d|[01]?\d\d?)$)");
    QRegularExpressionValidator* validator_ip = new QRegularExpressionValidator(expreg, this);

    ui->host_addr->setValidator(validator_ip);

    expreg.setPattern(R"(^(6553[0-5]|655[0-2]\d|65[0-4]\d{2}|6[0-4]\d{3}|[1-9]\d{0,3}|0)$)");
    QRegularExpressionValidator* validator_port = new QRegularExpressionValidator(expreg, this);

    ui->port->setValidator(validator_port);
    
    connect(ui->login_btn, &QPushButton::clicked, this, &Login::on_login);
    connect(ui->reg_btn, &QPushButton::clicked, this, &Login::on_register);
    connect(ui->confirm_btn, &QPushButton::clicked, this, &Login::on_ip_confirm);
    
    QThreadPool::globalInstance()->setMaxThreadCount(8);
    
    // ui->username->setText("FooBar");
    // ui->password->setText("Aa*1");
    
    ui->reg_username->setText("FooBar");
    ui->reg_password->setText("Aa*1");
    ui->phone->setText("13832123211");
    
    load_user_info();
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

void Login::start_connect(Message* msg) {
    if (!is_connected_) {
        Communication* task = new Communication(*msg);
        
        connect(task, &Communication::connect_failed, this, [=]() {
            QMessageBox::critical(this, "Server connection", "connect failed");
            
            is_connected_ = false;
        });
        
        connect(task, &Communication::login_ok, this, [=]() {
            DataManager::get_instance()->set_user_name(ui->username->text().toUtf8());
            
            save_user_info();
        });
        
        connect(task, &Communication::register_ok, this, [=]() {
            ui->stackedWidget->setCurrentIndex(0);
        });
        
        connect(task, &Communication::failed_msg, this, [=](QByteArray msg) {
            QMessageBox::warning(this, "Error", msg);
        });
        
        is_connected_ = true;
        
        DataManager::get_instance()->set_communication(task);
        
        QThreadPool::globalInstance()->start(task);
    } else {
        DataManager::get_instance()->communication()->send_message(msg);
    }
}

void Login::on_login() {    
    bool valid_name = verfify_data(ui->username);
    bool valid_pwd = verfify_data(ui->password);
    
    if (valid_name && valid_pwd) {
        Message msg;
        
        msg.user_name = ui->username->text().toUtf8();
        msg.reqcode = RequestCode::USER_LOGIN;
        
        QByteArray password = ui->password->text().toUtf8();
        
        password = QCryptographicHash::hash(password, QCryptographicHash::Sha224).toHex();
        
        msg.data1 = password;
        
        start_connect(&msg);
    }
}

void Login::on_register() {
    bool valid_reg_name = verfify_data(ui->reg_username);
    bool valid_reg_pwd = verfify_data(ui->reg_password);
    bool valid_reg_phone = verfify_data(ui->phone);
    
    
    if (valid_reg_name && valid_reg_pwd && valid_reg_phone) {
        Message msg;
        
        msg.user_name = ui->reg_username->text().toUtf8();
        msg.reqcode = RequestCode::REGISTER;

        QByteArray password = ui->reg_password->text().toUtf8();
        
        password = QCryptographicHash::hash(password, QCryptographicHash::Sha224).toHex();
        
        msg.data1 = password;
        msg.data2 = ui->phone->text().toUtf8();
        
        start_connect(&msg);
    }
}

void Login::on_ip_confirm() {
    bool valid_host = verfify_data(ui->host_addr);
    bool valid_port = verfify_data(ui->port);
    
    if (valid_host && valid_port) {
        DataManager* manager = DataManager::get_instance();
        
        manager->set_ip(ui->host_addr->text().toUtf8());
        manager->set_port(ui->port->text().toUtf8());        
    }
}

void Login::save_user_info() {
    if (ui->save_password->isChecked()) {
        QJsonObject object;
        
        object.insert("username", ui->username->text());
        object.insert("password", ui->password->text());
        
        QJsonDocument document(object);
        QByteArray json = document.toJson();
        AesCrypto aes(AesCrypto::kAesCbc128, key_.left(16));
        
        json = aes.encrypt(json);
        
        QFile file("password.bin");
        
        file.open(QFile::WriteOnly);
        
        file.write(json);
        
        file.close();
    } else {
        QFile file("password.bin");
        
        file.remove();
    }
}

void Login::load_user_info() {
    QFile file("password.bin");
    bool exists = file.open(QFile::ReadOnly);
    
    if (exists) {
        ui->save_password->setChecked(true);
        
        QByteArray json = file.readAll();
        AesCrypto aes(AesCrypto::kAesCbc128, key_.left(16));
        
        json = aes.decrypt(json);
        
        QJsonDocument document = QJsonDocument::fromJson(json);
        QJsonObject object = document.object();
        
        QString user_name = object.value("username").toString();
        QString password = object.value("password").toString();
        
        ui->username->setText(user_name);
        ui->password->setText(password);
    } else {
        ui->save_password->setChecked(false);
    }
    
    file.close();
}
