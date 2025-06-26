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
    
  private slots:
    void on_login_btn_clicked();
    
    void on_reg_btn_clicked();
    
    void on_confirm_btn_clicked();
    
  private:
    Ui::Login* ui;
};

#endif  // LOGIN_H
