#ifndef CODEC_H
#define CODEC_H

#include <QByteArray>
#include <QSharedPointer>

#include "information.pb.h"

struct Message {
    QByteArray user_name;
    QByteArray room_name;
    QByteArray data1;
    QByteArray data2;
    QByteArray data3;
    RequestCode reqcode;
    ResponseCode rescode;
};

class Codec {
  public:
    explicit Codec(Message* msg);
    
    explicit Codec(QByteArray msg);
    
    QByteArray encode_msg();
    
    QSharedPointer<Message> decode_msg();
    
    void reload(QByteArray msg);
    void reload(Message* msg);
    
  private:
    QByteArray msg_;
    Information obj_;
};

#endif  // CODEC_H
