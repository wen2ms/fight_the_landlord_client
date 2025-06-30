#include <QApplication>
#include <QResource>

#include "cards.h"
#include "login.h"
#include "aescrypto.h"

void test() {
    AesCrypto aes(AesCrypto::kAesCbc128, "1234567887654321");
    QByteArray text = aes.encrypt(
        "Even though assert will terminate the program in debug mode if the "
        "condition fails, it does nothing in release mode (the assert macro is "
        "removed if you compile with -DNDEBUG).");
    
    text = aes.decrypt(text);
    
    qDebug() << text;
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    
    qRegisterMetaType<Cards>("Cards&");
    QString rcc_path = QCoreApplication::applicationDirPath() + "/../Resources/resource.rcc";
    QResource::registerResource(rcc_path);
    
    Login w;
    w.show();
    
    test();
    
    return a.exec();
}
