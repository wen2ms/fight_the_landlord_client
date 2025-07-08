#include "communication.h"

#include <QDebug>
#include <QThread>

#include "datamanager.h"

Communication::Communication(Message* msg, QObject* parent)
    : QObject{parent}, socket_(nullptr), is_stop_(false), msg_info_(msg) {
    setAutoDelete(true);
}

void Communication::stop_loop() {
    is_stop_ = true;
}

void Communication::send_message(Message* msg) {
    Codec codec(msg);
    QByteArray data = codec.encode_msg();
    
    socket_->send_msg(data);
}

void Communication::parse_recv_message() {
    QByteArray data = socket_->recv_msg(2);
    Codec codec(data);
    QSharedPointer<Message> ptr = codec.decode_msg();
    
    if (ptr->reqcode) {
        
    }
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
        parse_recv_message();
    }
    
    socket_->disconnect();
    delete socket_;
}
