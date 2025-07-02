#ifndef RSACRYPTO_H
#define RSACRYPTO_H

#include <openssl/evp.h>

#include <QCryptographicHash>
#include <QObject>

class RsaCrypto : public QObject {
    Q_OBJECT
  public:
    enum KeyLength {
        kBits1k = 1024,
        kBits2k = 2048,
        kBits3k = 3072,
        kBits4k = 4096,
    };
    
    enum KeyType {
        kPublicKey,
        kPrivateKey
    };

    explicit RsaCrypto(QObject *parent = nullptr);
    
    explicit RsaCrypto(QByteArray filename, QObject *parent = nullptr);
    
    void prase_string_to_key(QByteArray data, KeyType type);
    
    void generate_rsa_key(KeyLength bits, QByteArray pub = "public.pem", QByteArray pri = "private.pem");
    
    QByteArray pub_key_encrypt(QByteArray data);
    
    QByteArray pri_key_decrypt(QByteArray data);
    
    QByteArray sign(QByteArray data, QCryptographicHash::Algorithm hash = QCryptographicHash::Sha256);
    
    bool verify(QByteArray sign, QByteArray data, QCryptographicHash::Algorithm hash = QCryptographicHash::Sha256);
    
  signals:
    
  private:
    EVP_PKEY* pub_key_;
    EVP_PKEY* pri_key_;
};

#endif  // RSACRYPTO_H
