#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QRunnable>

#include "tcpsocket.h"

class Communication : public QObject, public QRunnable {
    Q_OBJECT

  public:
    explicit Communication(QObject *parent = nullptr);
    
    inline void stop_loop() {
        is_stop_ = true;
    }
    
  protected:
    void run() override;
    
  signals:
    void connect_failed();
    
  private:
    TcpSocket* socket_;
    
    bool is_stop_;
};

#endif  // COMMUNICATION_H
