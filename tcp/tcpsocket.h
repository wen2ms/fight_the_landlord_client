#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>

#ifdef Q_OS_WIN
#include <winsock2.h>
#endif

#ifdef Q_OS_UNIX
#include <arpa/inet.h>
#endif

class TcpSocket : public QObject {
    Q_OBJECT

  public:
    TcpSocket(QObject *parent = nullptr);
    TcpSocket(QByteArray ip, unsigned short port, QObject *parent = nullptr);

    ~TcpSocket();

    bool connect_to_host(QByteArray ip, unsigned short port);

    QByteArray recv_msg(int timeout = -1);

    void send_msg(QByteArray msg, int timeout = -1);

    void disconnect();

  private:
#ifdef Q_OS_WIN
    SOCKET socket_;
#endif
    
#ifdef Q_OS_UNIX
    int socket_;
#endif
};

#endif // TCPSOCKET_H
