#include "tcpsocket.h"

#include <unistd.h>

#include <QDebug>

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

QByteArray TcpSocket::recv_msg(int timeout) {
    bool flag = read_timeout(timeout);
    QByteArray arr;
    
    if (flag) {
        int head_len = 0;
        int ret = readn(reinterpret_cast<char*>(&head_len), sizeof(int));
        
        if (ret == 0) {
            return QByteArray();
        }
        
        head_len = ntohl(head_len);
        
        qDebug() << "Recv head len:" << head_len;
        
        char* data = new char[head_len];
        
        assert(data);
        
        ret = readn(data, head_len);
        
        if (ret == head_len) {
            arr = QByteArray(data, head_len);
        }
        
        delete[] data;
    }
    
    return arr;
}

bool TcpSocket::send_msg(QByteArray msg, int timeout) {
    bool flag = write_timeout(timeout);
    
    if (flag) {
        int head_len = htonl(msg.size());
        int length = sizeof(int) + msg.size();
        char* data = new char[length];
        
        assert(data);
        
        memcpy(data, &head_len, sizeof(int));
        memcpy(data + sizeof(int), msg.data(), msg.size());
        
        int ret = writen(data, length);
        
        flag = (ret == length ? true : false);
        
        delete[] data;
    }
    
    return flag;
}

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

bool TcpSocket::read_timeout(int timeout) {
    if (timeout == -1) {
        return true;
    }

#ifdef Q_OS_WIN
    int nfds = 0;
#endif

#ifdef Q_OS_UNIX
    int nfds = socket_ + 1;
#endif

    fd_set rdset;

    FD_ZERO(&rdset);
    FD_SET(socket_, &rdset);

    struct timeval elapse;

    elapse.tv_sec = timeout;
    elapse.tv_usec = 0;

    int ret = select(nfds, &rdset, NULL, NULL, &elapse);

    return ret > 0;
}

bool TcpSocket::write_timeout(int timeout) {
    if (timeout == -1) {
        return true;
    }

#ifdef Q_OS_WIN
    int nfds = 0;
#endif

#ifdef Q_OS_UNIX
    int nfds = socket_ + 1;
#endif

    fd_set wrset;

    FD_ZERO(&wrset);
    FD_SET(socket_, &wrset);

    struct timeval elapse;

    elapse.tv_sec = timeout;
    elapse.tv_usec = 0;

    int ret = select(nfds, NULL, &wrset, NULL, &elapse);

    return ret > 0;
}

int TcpSocket::readn(char *buf, int count) {
    char* pt = buf;
    int last = count;
    int len = 0;
    
    while (last > 0) {
        len = recv(socket_, pt, last, 0);
        
        if (len == -1) {
            perror("recv");
            
            return -1;
        } else if (len == 0) {
            break;
        }
        
        pt += len;
        last -= len;
    }
    
    return count - last;
}

int TcpSocket::writen(const char *buf, int count) {
    const char* pt = buf;
    int last = count;
    int len = 0;
    
    while (last > 0) {
        len = send(socket_, pt, last, 0);
        
        if (len == -1) {
            perror("send");
            
            return -1;
        } else if (len == 0) {
            continue;
        }
        
        pt += len;
        last -= len;
    }
    
    return count - last;
}
