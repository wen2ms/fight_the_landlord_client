#include "aescrypto.h"

#include <openssl/aes.h>
#include <QCryptographicHash>

AesCrypto::AesCrypto(Algorithm algorithm, QByteArray key, QObject *parent) {
    switch (algorithm) {
        case kAesEcb128:
        case kAesCbc128:
        case kAesCfb128:
        case kAesOfb128:
        case kAesCtr128:
            assert(key.size() == 16);
            break;

        case kAesEcb192:
        case kAesCbc192:
        case kAesCfb192:
        case kAesOfb192:
        case kAesCtr192:
            assert(key.size() == 24);
            break;

        case kAesEcb256:
        case kAesCbc256:
        case kAesCfb256:
        case kAesOfb256:
        case kAesCtr256:
            assert(key.size() == 32);
            break;
    }

    type_ = algorithm;
    key_ = key;
}

QByteArray AesCrypto::encrypt(QByteArray text) {
    return aes_crypto(text, kEncrypto);
}

QByteArray AesCrypto::decrypt(QByteArray text) {
    return aes_crypto(text, kDecrypto);
}

QByteArray AesCrypto::aes_crypto(QByteArray text, CryptoType type) {
    unsigned char ivec[AES_BLOCK_SIZE];
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    
    assert(ctx);
    
    generate_ivec(ivec);
    
    int ret = EVP_CipherInit_ex(ctx, algorithms_.value(type_)(), NULL, reinterpret_cast<unsigned char*>(key_.data()), ivec, type);
    
    assert(ret);
    
    int length = text.size() + 1;
    
    if (length % AES_BLOCK_SIZE > 0) {
        length = (length / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
    }
    
    auto out = new unsigned char[length];
    int out_len = 0;
    int total_len = 0;
    
    ret = EVP_CipherUpdate(ctx, out, &out_len, reinterpret_cast<const unsigned char*>(text.data()), text.size());
    
    total_len += out_len;
    
    assert(ret);
    
    ret = EVP_CipherFinal_ex(ctx, out + out_len, &out_len);
    
    total_len += out_len;
    
    assert(ret);
    
    QByteArray out_text(reinterpret_cast<char*>(out), total_len);
    
    delete[] out;
    EVP_CIPHER_CTX_free(ctx);
    
    return out_text;
}

void AesCrypto::generate_ivec(unsigned char *ivec) {
    QCryptographicHash hash(QCryptographicHash::Md5);
    
    hash.addData(key_);
    
    std::string str = hash.result().toStdString();
    
    for (int i = 0; i < AES_BLOCK_SIZE; ++i) {
        ivec[i] = str.at(i);
    }
}
