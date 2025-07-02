#include "rsacrypto.h"

#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

RsaCrypto::RsaCrypto(QObject *parent) : QObject{parent} {}

void RsaCrypto::generate_rsa_key(KeyLength bits, QByteArray pub, QByteArray pri) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    
    int ret = EVP_PKEY_keygen_init(ctx);
    
    assert(ret == 1);
    
    ret = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits);
    
    assert(ret == 1);
    
    EVP_PKEY* key;
    
    ret = EVP_PKEY_generate(ctx, &key);
    
    assert(ret == 1);
    
    EVP_PKEY_CTX_free(ctx);
    
    BIO* bio = BIO_new_file(pub, "wb");
    
    ret = PEM_write_bio_PUBKEY(bio, key);
    
    assert(ret == 1);
    
    BIO_flush(bio);
    BIO_free(bio);
    
    bio = BIO_new_file(pri, "wb");
    ret = PEM_write_bio_PrivateKey(bio, key, NULL, NULL, 0, NULL, NULL);
    
    assert(ret == 1);
    
    BIO_flush(bio);
    BIO_free(bio);
}
