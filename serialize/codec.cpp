#include "codec.h"

Codec::Codec(Message *msg) {
    reload(msg);
}

Codec::Codec(QByteArray msg) {
    reload(msg);
}

QByteArray Codec::encode_msg() {
    std::string out;
    
    obj_.SerializeToString(&out);
    
    return QByteArray::fromStdString(out);
}

QSharedPointer<Message> Codec::decode_msg() {
    std::string data = msg_.toStdString();
    
    obj_.ParseFromString(data);
    
    Message* msg = new Message;
    
    msg->user_name = QByteArray::fromStdString(obj_.user_name());
    msg->room_name = QByteArray::fromStdString(obj_.room_name());
    msg->data1 = QByteArray::fromStdString(obj_.data1());
    msg->data2 = QByteArray::fromStdString(obj_.data2());
    msg->data3 = QByteArray::fromStdString(obj_.data3());
    msg->reqcode = obj_.reqcode();
    msg->rescode = obj_.rescode();
    
    QSharedPointer<Message> ptr(msg);
    
    return ptr;
}

void Codec::reload(QByteArray msg) {
    msg_ = msg;
}

void Codec::reload(Message *msg) {
    obj_.set_user_name(msg->user_name.toStdString());
    obj_.set_room_name(msg->room_name.toStdString());
    obj_.set_data1(msg->data1.toStdString());
    obj_.set_data2(msg->data2.toStdString());
    obj_.set_data3(msg->data3.toStdString());
    obj_.set_reqcode(msg->reqcode);
    obj_.set_rescode(msg->rescode);
}
