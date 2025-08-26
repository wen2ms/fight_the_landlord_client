#include "datamanager.h"

DataManager* DataManager::data_ = new DataManager;

DataManager *DataManager::get_instance() {
    return data_;
}

void DataManager::set_user_name(QByteArray name) {
    user_name_ = name;
}

void DataManager::set_room_name(QByteArray room_name) {
    room_name_ = room_name;
}

void DataManager::set_ip(QByteArray ip) {
    ip_ = ip;
}

void DataManager::set_port(QByteArray port) {
    port_ = port;
}

void DataManager::set_communication(Communication *comm) {
    comm_ = comm;
}

void DataManager::set_cards(Cards cards, Cards last_three_cards) {
    cards_ = cards;
    last_three_cards_ = last_three_cards;
}

void DataManager::set_game_mode_type(GameModeType mode_type) {
    game_mode_type_ = mode_type;
}

void DataManager::set_room_mode(RoomMode room_mode) {
    room_mode_ = room_mode;
}

QByteArray DataManager::user_name() const {
    return user_name_;
}

QByteArray DataManager::room_name() const {
    return room_name_;
}

QByteArray DataManager::ip() const {
    return ip_;
}

QByteArray DataManager::port() const {
    return port_;
}

Communication *DataManager::communication() const {
    return comm_;
}

Cards DataManager::cards() const {
    return cards_;    
}

Cards DataManager::last_three_cards() const {
    return last_three_cards_;
}

DataManager::GameModeType DataManager::game_mode_type() const {
    return game_mode_type_;
}

DataManager::RoomMode DataManager::room_mode() const {
    return room_mode_;
}
