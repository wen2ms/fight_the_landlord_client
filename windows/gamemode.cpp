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
    
    connect(ui->standalone_btn, &QPushButton::clicked, this, [=]() {
        MainWindow* main_window = new MainWindow;
        
        connect(main_window, &MainWindow::window_close, this, &GameMode::show);
        
        main_window->show();
        
        hide();
    });
    
    connect(ui->online_btn, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
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
