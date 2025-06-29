#include "datamanager.h"

DataManager* DataManager::data_ = new DataManager;

DataManager *DataManager::get_instance() {
    return data_;
}

void DataManager::set_user_name(QByteArray name) {
    user_name_ = name;
}

void DataManager::set_ip(QByteArray ip) {
    ip_ = ip;
}

void DataManager::set_port(QByteArray port) {
    port_ = port;
}

QByteArray DataManager::user_name() {
    return user_name_;
}

QByteArray DataManager::ip() {
    return ip_;
}

QByteArray DataManager::port() {
    return port_;
}
