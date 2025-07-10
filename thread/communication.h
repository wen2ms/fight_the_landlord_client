#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QRunnable>

#include "tcpsocket.h"
#include "codec.h"

class Communication : public QObject, public QRunnable {
    Q_OBJECT

  public:
    enum KeyLen {
        kLen16 = 16,
        kLen24 = 24,
        kLen32 = 32
    };
    
    explicit Communication(Message* msg, QObject *parent = nullptr);
    
    void stop_loop();
    
    void send_message(Message* msg);
    
    void parse_recv_message();
    
    void handle_rsa_distribution(Message* msg);
    
    QByteArray generate_aes_key(KeyLen len);
    
  protected:
    void run() override;
    
  signals:
    void connect_failed();
    
  private:
    TcpSocket* socket_;
    Message* msg_info_;
    
    bool is_stop_;
};

#endif  // COMMUNICATION_H
