#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class Login;
}

class Login : public QDialog {
    Q_OBJECT

  public:
    explicit Login(QWidget* parent = nullptr);
    ~Login();

    bool verfify_data(QLineEdit* edit);
    
    void on_login();
    
    void on_register();
    
    void on_ip_confirm();
        
  private:
    Ui::Login* ui;
};

#endif  // LOGIN_H
