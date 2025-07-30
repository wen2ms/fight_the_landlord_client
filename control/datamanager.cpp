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

QByteArray DataManager::user_name() {
    return user_name_;
}

QByteArray DataManager::room_name() {
    return room_name_;
}

QByteArray DataManager::ip() {
    return ip_;
}

QByteArray DataManager::port() {
    return port_;
}

Communication *DataManager::communication() {
    return comm_;
}

Cards DataManager::cards() {
    return cards_;    
}

Cards DataManager::last_three_cards() {
    return last_three_cards_;
}

DataManager::GameModeType DataManager::game_mode_type() {
    return game_mode_type_;
}
