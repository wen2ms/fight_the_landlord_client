#ifndef AESCRYPTO_H
#define AESCRYPTO_H

#include <openssl/evp.h>

#include <QMap>
#include <QObject>

class AesCrypto : public QObject {
    Q_OBJECT

  public:
    enum Algorithm {
        kAesEcb128,
        kAesCbc128,
        kAesCfb128,
        kAesOfb128,
        kAesCtr128,

        kAesEcb192,
        kAesCbc192,
        kAesCfb192,
        kAesOfb192,
        kAesCtr192,

        kAesEcb256,
        kAesCbc256,
        kAesCfb256,
        kAesOfb256,
        kAesCtr256,
    };
    
    enum CryptoType {
        kDecrypto,
        kEncrypto
    };
    
    using algorithm_func = const EVP_CIPHER* (*)(void);

    AesCrypto(Algorithm algorithm, QByteArray key, QObject* parent = nullptr);
    ~AesCrypto();
    
    QByteArray encrypt(QByteArray text);
    QByteArray decrypt(QByteArray text);
    
  private:
    QByteArray aes_crypto(QByteArray text, CryptoType type);
    
    void generate_ivec(unsigned char* ivec);

    const QMap<Algorithm, algorithm_func> algorithms_ = {
        {kAesEcb128, EVP_aes_128_ecb},
        {kAesCbc128, EVP_aes_128_cbc},
        {kAesCfb128, EVP_aes_128_cfb128},
        {kAesOfb128, EVP_aes_128_ofb},
        {kAesCtr128, EVP_aes_128_ctr},
        
        {kAesEcb192, EVP_aes_192_ecb},
        {kAesCbc192, EVP_aes_192_cbc},
        {kAesCfb192, EVP_aes_192_cfb128},
        {kAesOfb192, EVP_aes_192_ofb},
        {kAesCtr192, EVP_aes_192_ctr},
        
        {kAesEcb256, EVP_aes_256_ecb},
        {kAesCbc256, EVP_aes_256_cbc},
        {kAesCfb256, EVP_aes_256_cfb128},
        {kAesOfb256, EVP_aes_256_ofb},
        {kAesCtr256, EVP_aes_256_ctr}
    };

    Algorithm type_;
    QByteArray key_;
};

#endif // AESCRYPTO_H
