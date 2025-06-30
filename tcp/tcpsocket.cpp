#include "tcpsocket.h"

#include <unistd.h>

TcpSocket::TcpSocket(QObject *parent) {
#ifdef Q_OS_WIN
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);
#endif
}

TcpSocket::TcpSocket(QByteArray ip, unsigned short port, QObject *parent) : TcpSocket(parent) {
    connect_to_host(ip, port);
}

TcpSocket::~TcpSocket() {
#ifdef Q_OS_WIN
    WSACleanup();
#endif
}

bool TcpSocket::connect_to_host(QByteArray ip, unsigned short port) {
    assert(port > 0 && port <= 65535);
    
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    
    assert(socket_ > 0);
    
    struct sockaddr_in addr;
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    
    inet_pton(AF_INET, ip.data(), &addr.sin_addr.s_addr);
    
    int ret = ::connect(socket_, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
    
    return ret == 0;
}

QByteArray TcpSocket::recv_msg(int timeout) {}

void TcpSocket::send_msg(QByteArray msg, int timeout) {}

void TcpSocket::disconnect() {
    if (socket_ > 0) {
#ifdef Q_OS_WIN
        closesocket(socket_);
#endif
        
#ifdef Q_OS_UNIX
        close(socket_);
#endif
    }
}
