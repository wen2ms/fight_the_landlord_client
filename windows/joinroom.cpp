#include "joinroom.h"

#include "codec.h"
#include "datamanager.h"
#include "ui_joinroom.h"

JoinRoom::JoinRoom(DialogType dialog_type, QWidget *parent) : QDialog(parent), ui(new Ui::JoinRoom) {
    ui->setupUi(this);
    
    ui->join_room->setFixedSize(100, 45);
    ui->join_room->set_image(":images/addRoom-normal.png", ":images/addRoom-hover.png", ":images/addRoom-pressed.png",
                             ":images/addRoom-disable.png");
    ui->join_room->set_button_disable(true);

    const QString kTitles[] = {":/images/info1.png", ":/images/info2.png"};
    int index = static_cast<int>(dialog_type);
    
    ui->description->setPixmap(QPixmap(kTitles[index]));
    ui->stackedWidget->setCurrentIndex(index);
    
    this->setFixedSize(720, 360);
    
    Communication* communication = DataManager::get_instance()->communication();
    connect(communication, &Communication::room_exists, this, [=](bool success) {
        ui->join_room->set_button_disable(!success);
    });
    
    connect(ui->create_room, &QPushButton::clicked, this, &JoinRoom::join_room);
    connect(ui->search_room, &QPushButton::clicked, this, &JoinRoom::search_room);
    connect(ui->join_room, &QPushButton::clicked, this, &JoinRoom::join_room);
}

JoinRoom::~JoinRoom() {
    delete ui;
}

void JoinRoom::search_room() {
    encode_message(RequestCode::SEARCH_ROOM);
}

void JoinRoom::join_room() {
    encode_message(RequestCode::MANUAL_CREATE_ROOM);
    DataManager::get_instance()->set_room_mode(DataManager::kManual);
    accept();
}

void JoinRoom::encode_message(RequestCode reqcode) {
    Message msg;
    
    msg.reqcode = reqcode;
    msg.user_name = DataManager::get_instance()->user_name();
    msg.room_name = ui->room_name->text().toUtf8();
    
    DataManager::get_instance()->communication()->send_message(&msg);
}
