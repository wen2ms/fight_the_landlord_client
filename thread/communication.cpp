#include "communication.h"

#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>
#include <QThread>

#include "cards.h"
#include "datamanager.h"
#include "rsacrypto.h"
#include "taskqueue.h"

Communication::Communication(Message& msg, QObject* parent)
    : QObject{parent}, socket_(nullptr), is_stop_(false), msg_info_(msg), aes_crypto_(nullptr) {
    setAutoDelete(true);
}

Communication::~Communication() {
    if (aes_crypto_ != nullptr) {
        delete aes_crypto_;
        
        aes_crypto_ = nullptr;
    }
}

void Communication::stop_loop() {
    is_stop_ = true;
}

void Communication::send_message(Message* msg, bool is_encrypt) {
    Codec codec(msg);
    QByteArray data = codec.encode_msg();
    
    if (is_encrypt) {
        data = aes_crypto_->encrypt(data);
    }
    
    socket_->send_msg(data);
}

void Communication::parse_recv_message() {
    QByteArray data = socket_->recv_msg(2);
    
    qDebug() << "Recv message:" << data;
    
    if (data.isEmpty()) {
        return;
    }
    
    Codec codec(data);
    QSharedPointer<Message> ptr = codec.decode_msg();
    
    qDebug() << ptr->rescode << "," << ptr->data1 << "," << ptr->data2;
    
    switch (ptr->rescode) {
        case LOGIN_OK:
            emit login_ok();
            qDebug() << "Login successfully!";            
            break;
        case REGISTER_OK:
            emit register_ok();
            qDebug() << "Register successfully!";
            break;
        case RSA_DISTRIBUTION:
            handle_rsa_distribution(ptr.get());
            break;
        case AES_VERIFY_OK:
            aes_crypto_ = new AesCrypto(AesCrypto::kAesCbc256, aes_key_);
            
            send_message(&msg_info_);
            qDebug() << "Aes key distribution successfully!";
            break;
        case JOIN_ROOM_OK:
            DataManager::get_instance()->set_room_name(ptr->room_name);
            emit player_count(ptr->data1.toInt());
            break;
        case DEAL_CARDS:
            parse_cards(ptr->data1, ptr->data2);
            break;
        case START_GAME:
            emit start_game(ptr->data1);
            break;
        case OTHER_BID_LORD: {
            Task task;
            task.points = ptr->data1.toInt();
            TaskQueue::get_instance()->add(task);
            break;    
        }
        case OTHER_PLAY_A_HAND: {
            Task task;
            QDataStream stream(ptr->data2);
            for (int i = 0; i < ptr->data1.toInt(); ++i) {
                Card card;
                stream >> card;
                task.cards.add(card);
            }
            TaskQueue::get_instance()->add(task);
            break;
        }
        case SEARCH_ROOM_OK: {
            bool success = (ptr->data1 == "true");
            emit room_exists(success);
            break;
        }
        case FAILED:
            emit failed_msg(ptr->data1);
            break;
        default:
            break;
    }
}

void Communication::handle_rsa_distribution(Message* msg) {
    RsaCrypto rsa;
    
    rsa.parse_string_to_key(msg->data1, RsaCrypto::kPublicKey);
    
    bool is_valid = rsa.verify(msg->data2, msg->data1);
    
    assert(is_valid);
    
    aes_key_ = generate_aes_key(KeyLen::kLen32);
    
    Message res_msg;
    
    res_msg.reqcode = AES_DISTRIBUTION;
    res_msg.data1 = rsa.pub_key_encrypt(aes_key_);
    res_msg.data2 = QCryptographicHash::hash(aes_key_, QCryptographicHash::Sha224).toHex();
    
    send_message(&res_msg, false);
}

QByteArray Communication::generate_aes_key(KeyLen len) {
    QByteArray time = QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm:ss.zzz").toUtf8();
    QCryptographicHash hash(QCryptographicHash::Sha3_384);
    
    hash.addData(time);
    
    time = hash.result().left(len);
    
    return time;
}

void Communication::parse_cards(QByteArray data1, QByteArray data2) {
    auto func = [](QByteArray message) {
        auto data_list = message.left(message.length() - 1).split('#');
        Cards cards;
        for (const auto& data : data_list) {
            auto sub_data = data.split('-');
            Card card(static_cast<Card::CardSuit>(sub_data.first().toInt()),
                      static_cast<Card::CardRank>(sub_data.last().toInt()));
            cards.add(card);
        }
        return cards;
    };
    
    Cards cards = func(data1);
    Cards last_three_cards = func(data2);
    
    DataManager::get_instance()->set_cards(cards, last_three_cards);
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
