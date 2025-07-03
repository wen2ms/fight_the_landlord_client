#ifndef BASE64_H
#define BASE64_H

#include <QObject>

class Base64 : public QObject {
    Q_OBJECT
  public:
    explicit Base64(QObject *parent = nullptr);
    
    QByteArray encode(QByteArray data);
    
    QByteArray encode(const char* data, int length);
    
    QByteArray decode(QByteArray data);
    
    QByteArray decode(const char* data, int length);
    
  signals:
};

#endif  // BASE64_H
