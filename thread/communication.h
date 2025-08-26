#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QRunnable>

#include "tcpsocket.h"
#include "codec.h"
#include "aescrypto.h"

class Communication : public QObject, public QRunnable {
    Q_OBJECT

  public:
    enum KeyLen {
        kLen16 = 16,
        kLen24 = 24,
        kLen32 = 32
    };
    
    explicit Communication(Message& msg, QObject *parent = nullptr);
    
    ~Communication();
    
    void stop_loop();
    
    void send_message(Message* msg, bool is_encrypt = true);
    
    void parse_recv_message();
    
    void handle_rsa_distribution(Message* msg);
    
    QByteArray generate_aes_key(KeyLen len);
    
    void parse_cards(QByteArray data1, QByteArray data2);
    
  protected:
    void run() override;
    
  signals:
    void connect_failed();
    
    void login_ok();
    void register_ok();
    
    void failed_msg(QByteArray msg);
    void player_count(int number);
    void start_game(QByteArray msg);
    void room_exists(bool success);
    
  private:
    TcpSocket* socket_;
    Message msg_info_;
    
    bool is_stop_;
    
    QByteArray aes_key_;
    AesCrypto* aes_crypto_;
};

#endif  // COMMUNICATION_H
