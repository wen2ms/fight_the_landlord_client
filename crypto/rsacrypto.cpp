#include "rsacrypto.h"

#include "base64.h"

#include <openssl/rsa.h>
#include <openssl/pem.h>

RsaCrypto::RsaCrypto(QObject *parent) : QObject{parent} , pub_key_(NULL), pri_key_(NULL) {}

RsaCrypto::RsaCrypto(QByteArray filename, KeyType type, QObject* parent) : RsaCrypto() {
    BIO* bio = BIO_new_file(filename.data(), "rb");
    
    assert(bio != NULL);
    
    if (type == kPublicKey) {
        PEM_read_bio_PUBKEY(bio, &pub_key_, NULL, NULL);
    } else {
        PEM_read_bio_PrivateKey(bio, &pri_key_, NULL, NULL);
    }
    
    BIO_free(bio);
}

RsaCrypto::~RsaCrypto() {
    if (pub_key_) {
        EVP_PKEY_free(pub_key_);
    }
    
    if (pri_key_) {
        EVP_PKEY_free(pri_key_);
    }
}

void RsaCrypto::parse_string_to_key(QByteArray data, KeyType type) {
    BIO* bio = BIO_new_mem_buf(data.data(), data.size());
    
    assert(bio != NULL);
    
    if (type == kPublicKey) {
        PEM_read_bio_PUBKEY(bio, &pub_key_, NULL, NULL);
    } else {
        PEM_read_bio_PrivateKey(bio, &pri_key_, NULL, NULL);
    }
    
    BIO_free(bio);
}

void RsaCrypto::generate_rsa_key(KeyLength bits, QByteArray pub, QByteArray pri) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    
    int ret = EVP_PKEY_keygen_init(ctx);
    
    assert(ret == 1);
    
    ret = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits);
    
    assert(ret == 1);
    
    ret = EVP_PKEY_generate(ctx, &pri_key_);
    
    assert(ret == 1);
    
    EVP_PKEY_CTX_free(ctx);
    
    BIO* bio = BIO_new_file(pub.data(), "wb");
    
    ret = PEM_write_bio_PUBKEY(bio, pri_key_);
    
    assert(ret == 1);
    
    BIO_flush(bio);
    BIO_free(bio);
    
    bio = BIO_new_file(pri.data(), "wb");
    ret = PEM_write_bio_PrivateKey(bio, pri_key_, NULL, NULL, 0, NULL, NULL);
    
    assert(ret == 1);
    
    BIO_flush(bio);
    BIO_free(bio);
}

QByteArray RsaCrypto::pub_key_encrypt(QByteArray data) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pub_key_, NULL);
    
    assert(ctx != NULL);
    
    int ret = EVP_PKEY_encrypt_init(ctx);
    
    assert(ret == 1);
    
    ret = EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING);
    
    assert(ret == 1);
    
    size_t outlen = 0;
    
    ret = EVP_PKEY_encrypt(ctx, NULL, &outlen, reinterpret_cast<const unsigned char*>(data.data()), data.size());
    
    assert(ret == 1);
    
    auto out = new unsigned char[outlen];
    
    ret = EVP_PKEY_encrypt(ctx, out, &outlen, reinterpret_cast<const unsigned char*>(data.data()), data.size());
    
    assert(ret == 1);
    
    Base64 base64;
    QByteArray str = base64.encode(reinterpret_cast<char*>(out), outlen);
    
    delete[] out;
    EVP_PKEY_CTX_free(ctx);
    
    return str;
}

QByteArray RsaCrypto::pri_key_decrypt(QByteArray data) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pri_key_, NULL);
    
    assert(ctx != NULL);
    
    int ret = EVP_PKEY_decrypt_init(ctx);
    
    assert(ret == 1);
    
    ret = EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING);
    
    assert(ret == 1);
    
    Base64 base64;
    
    data = base64.decode(data);
    
    size_t outlen = 0;
    
    ret = EVP_PKEY_decrypt(ctx, NULL, &outlen, reinterpret_cast<const unsigned char*>(data.data()), data.size());
    
    assert(ret == 1);
    
    auto out = new unsigned char[outlen];
    
    ret = EVP_PKEY_decrypt(ctx, out, &outlen, reinterpret_cast<const unsigned char*>(data.data()), data.size());
    
    assert(ret == 1);
    
    QByteArray str(reinterpret_cast<char*>(out), outlen);
    
    delete[] out;
    EVP_PKEY_CTX_free(ctx);
    
    return str;
}

QByteArray RsaCrypto::sign(QByteArray data, QCryptographicHash::Algorithm hash) {
    QCryptographicHash hash_val(hash);
    
    hash_val.addData(data);
    
    QByteArray md = hash_val.result();
    
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pri_key_, NULL);
    
    assert(ctx != NULL);
    
    int ret = EVP_PKEY_sign_init(ctx);
    
    assert(ret == 1);
    
    ret = EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
    
    assert(ret == 1);
    
    ret = EVP_PKEY_CTX_set_signature_md(ctx, hash_methods_.value(hash)());
    
    assert(ret == 1);

    size_t out_len = 0;
    
    ret = EVP_PKEY_sign(ctx, NULL, &out_len, reinterpret_cast<const unsigned char*>(md.data()), md.size());
    
    assert(ret == 1);
    
    auto out = new unsigned char[out_len];
    
    ret = EVP_PKEY_sign(ctx, out, &out_len, reinterpret_cast<const unsigned char*>(md.data()), md.size());
    
    assert(ret == 1);
    
    Base64 base64;
    QByteArray str = base64.encode(reinterpret_cast<char*>(out), out_len);
    
    delete[] out;
    EVP_PKEY_CTX_free(ctx);
    
    return str;
}

bool RsaCrypto::verify(QByteArray sign, QByteArray data, QCryptographicHash::Algorithm hash) {
    Base64 base64;
    
    sign = base64.decode(sign);
    
    QCryptographicHash hash_val(hash);

    hash_val.addData(data);

    QByteArray md = hash_val.result();

    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pub_key_, NULL);

    assert(ctx != NULL);

    int ret = EVP_PKEY_verify_init(ctx);

    assert(ret == 1);

    ret = EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);

    assert(ret == 1);

    ret = EVP_PKEY_CTX_set_signature_md(ctx, hash_methods_.value(hash)());

    assert(ret == 1);

    ret = EVP_PKEY_verify(ctx, reinterpret_cast<const unsigned char*>(sign.data()), sign.size(),
                          reinterpret_cast<const unsigned char*>(md.data()), md.size());

    EVP_PKEY_CTX_free(ctx);

    return ret == 1;
}
