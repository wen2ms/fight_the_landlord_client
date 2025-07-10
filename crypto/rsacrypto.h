#ifndef RSACRYPTO_H
#define RSACRYPTO_H

#include <openssl/evp.h>

#include <QCryptographicHash>
#include <QObject>
#include <QMap>

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
    
    using hash_func = const EVP_MD* (*)();

    explicit RsaCrypto(QObject *parent = nullptr);
    
    explicit RsaCrypto(QByteArray filename, KeyType type, QObject *parent = nullptr);
    
    ~RsaCrypto();
    
    void parse_string_to_key(QByteArray data, KeyType type);
    
    void generate_rsa_key(KeyLength bits, QByteArray pub = "public.pem", QByteArray pri = "private.pem");
    
    QByteArray pub_key_encrypt(QByteArray data);
    
    QByteArray pri_key_decrypt(QByteArray data);
    
    QByteArray sign(QByteArray data, QCryptographicHash::Algorithm hash = QCryptographicHash::Sha256);
    
    bool verify(QByteArray sign, QByteArray data, QCryptographicHash::Algorithm hash = QCryptographicHash::Sha256);
    
  signals:
    
  private:
    const QMap<QCryptographicHash::Algorithm, hash_func> hash_methods_ = {
        {QCryptographicHash::Md5, EVP_md5}, 
        {QCryptographicHash::Sha1, EVP_sha1},
        {QCryptographicHash::Sha224, EVP_sha224},
        {QCryptographicHash::Sha256, EVP_sha256},
        {QCryptographicHash::Sha384, EVP_sha384},
        {QCryptographicHash::Sha512, EVP_sha512},
        {QCryptographicHash::Sha3_224, EVP_sha3_224},
        {QCryptographicHash::Sha3_256, EVP_sha3_256},
        {QCryptographicHash::Sha3_384, EVP_sha3_384},
        {QCryptographicHash::Sha3_512, EVP_sha3_512},
    };

    EVP_PKEY* pub_key_;
    EVP_PKEY* pri_key_;
};

#endif  // RSACRYPTO_H
