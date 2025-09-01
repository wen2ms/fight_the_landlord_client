#include <QApplication>
#include <QResource>
#include <QFile>

#include "cards.h"
#include "login.h"
#include "aescrypto.h"
#include "rsacrypto.h"

void test() {
    AesCrypto aes(AesCrypto::kAesCbc128, "1234567887654321");
    QByteArray text = aes.encrypt(
        "Even though assert will terminate the program in debug mode if the "
        "condition fails, it does nothing in release mode (the assert macro is "
        "removed if you compile with -DNDEBUG).");
    
    text = aes.decrypt(text);
    
    qDebug() << text;
    
    RsaCrypto rsa;
    
    rsa.generate_rsa_key(RsaCrypto::kBits2k);
    
    RsaCrypto rsa_pub("public.pem", RsaCrypto::kPublicKey);
    QByteArray temp = "Even though assert will terminate the program in debug mode.....";
    
    temp = rsa_pub.pub_key_encrypt(temp);
    
    qDebug() << "Encrypt =" << temp.data();
    
    RsaCrypto rsa_pri("private.pem", RsaCrypto::kPrivateKey);
    
    temp = rsa_pri.pri_key_decrypt(temp);
    
    qDebug() << "Decrypt =" << temp.data();
    
    temp = rsa_pri.sign(text);
    
    qDebug() << "Signed =" << temp.data();
    
    bool flag = rsa_pub.verify(temp, text);
    
    qDebug() << "flag =" << flag;    
}


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    qRegisterMetaType<Cards>("Cards&");
    QString rcc_path = QCoreApplication::applicationDirPath() + "/../Resources/resource.rcc";
    QResource::registerResource(rcc_path);
    
    QFile file(":/conf/style.qss");
    file.open(QFile::ReadOnly);
    QByteArray style = file.readAll();
    app.setStyleSheet(style);
    file.close();
    
    Login login_window;
    int ret = login_window.exec();
    
    if (ret == QDialog::Accepted) {
        // test();
        
        return app.exec();     
    }
    
    return 0;    
}
