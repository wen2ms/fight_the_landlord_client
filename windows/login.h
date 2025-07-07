#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLineEdit>

#include "codec.h"

namespace Ui {
class Login;
}

class Login : public QDialog {
    Q_OBJECT

  public:
    explicit Login(QWidget* parent = nullptr);
    ~Login();

    bool verfify_data(QLineEdit* edit);
    
    void start_connect(Message* msg);
    
    void on_login();
    
    void on_register();
    
    void on_ip_confirm();
        
  private:
    Ui::Login* ui;
    
    bool is_connected_;
};

#endif  // LOGIN_H
