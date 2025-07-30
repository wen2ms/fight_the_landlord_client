#include "gamemode.h"

#include "ui_gamemode.h"

#include "mainwindow.h"
#include "codec.h"
#include "datamanager.h"
#include "joinroom.h"

GameMode::GameMode(QWidget *parent) : QDialog(parent), ui(new Ui::GameMode) {
    ui->setupUi(this);
    
    this->setFixedSize(1080, 720);
    ui->stackedWidget->setCurrentIndex(0);
    ui->information->setVisible(false);

    Communication* communication = DataManager::get_instance()->communication();
    
    connect(communication, &Communication::player_count, this, [=](int count) {
        QString tip = QString("There are <%1> players in the current <%2> room, waiting for other players to enter")
                          .arg(count)
                          .arg(DataManager::get_instance()->room_name());
        ui->information->setText(tip);
        ui->information->setVisible(true);
    });
    
    connect(communication, &Communication::start_game, this, [=](QByteArray msg) {
        this->hide();
        MainWindow* main_window = new MainWindow;
        connect(main_window, &MainWindow::window_close, this, &GameMode::show);
        main_window->show();
        main_window->init_main_window(msg);
        disconnect(communication, &Communication::start_game, this, nullptr);
    });
    
    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [=](int index) {
        if (index == 0) {
            ui->information->setVisible(false);
        }
    });

    connect(ui->standalone_btn, &QPushButton::clicked, this, [=]() {
        MainWindow* main_window = new MainWindow;
        connect(main_window, &MainWindow::window_close, this, &GameMode::show);
        main_window->show();
        hide();
        DataManager::get_instance()->set_game_mode_type(DataManager::kStandalone);
    });

    connect(ui->online_btn, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
        DataManager::get_instance()->set_game_mode_type(DataManager::kOnline);
    });
    
    connect(ui->auto_create_room_btn, &QPushButton::clicked, this, [=]() {
        Message msg;
        
        msg.user_name = DataManager::get_instance()->user_name();
        msg.reqcode = RequestCode::AUTO_CREATE_ROOM;
        
        DataManager::get_instance()->communication()->send_message(&msg);
    });
    
    connect(ui->manual_create_room_btn, &QPushButton::clicked, this, [=]() {
        JoinRoom room(DialogType::kMaunalCreateRoom);
        
        room.setWindowTitle("Create Room");
        room.exec();
    });
    
    connect(ui->search_room_btn, &QPushButton::clicked, this, [=]() {
        JoinRoom room(DialogType::kSearchRoom);
        
        room.setWindowTitle("Search Room");
        room.exec();
    });
}

GameMode::~GameMode() {
    delete ui;
}

void GameMode::closeEvent(QCloseEvent *event) {
    if (ui->stackedWidget->currentIndex() == 1) {
        ui->stackedWidget->setCurrentIndex(0);
        
        event->ignore();
    } else {
        event->accept();
    }
}
