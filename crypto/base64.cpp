#include "base64.h"

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

Base64::Base64(QObject *parent) : QObject{parent} {}

QByteArray Base64::encode(QByteArray data) {
    return encode(data.data(), data.size());
}

QByteArray Base64::encode(const char *data, int length) {
    BIO* base64 = BIO_new(BIO_f_base64());
    BIO* mem = BIO_new(BIO_s_mem());
    
    BIO_push(base64, mem);
    
    BIO_write(base64, data, length);
    BIO_flush(base64);
    
    BUF_MEM* ptr;
    
    BIO_get_mem_ptr(base64, &ptr);
    
    QByteArray str(ptr->data, ptr->length);
    
    BIO_free_all(base64);
    
    return str;
}

QByteArray Base64::decode(QByteArray data) {
    return decode(data.data(), data.size());
}

QByteArray Base64::decode(const char *data, int length) {
    BIO* base64 = BIO_new(BIO_f_base64());
    BIO* mem = BIO_new(BIO_s_mem());
    
    BIO_push(base64, mem);
    
    BIO_write(mem, data, length);
    
    char* buf = new char[length];
    int buf_len = BIO_read(base64, buf, length);
    QByteArray out(buf, buf_len);
    
    delete[] buf;
    BIO_free_all(base64);
    
    return out;
}
