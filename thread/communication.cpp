#include "communication.h"

#include <QDebug>
#include <QThread>

#include "datamanager.h"

Communication::Communication(QObject* parent) : QObject{parent}, socket_(nullptr), is_stop_(false) {
    setAutoDelete(true);
}

void Communication::run() {
    socket_ = new TcpSocket;

    DataManager* manager = DataManager::get_instance();

    bool is_connected = socket_->connect_to_host(manager->ip(), manager->port().toInt());
    
    qDebug() << "----------Communcation thread id:" << QThread::currentThread() << "connected ?" << is_connected;

    if (!is_connected) {
        emit connect_failed();
        
        delete socket_;

        return;
    }
    
    while (!is_stop_) {
    
    }
    
    socket_->disconnect();
    delete socket_;
}
