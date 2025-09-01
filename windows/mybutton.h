#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include <QPixmap>
#include <QMouseEvent>

class MyButton : public QPushButton {
    Q_OBJECT
  public:
    explicit MyButton(QWidget *parent = nullptr);
    
    void set_image(const QString& normal, const QString& hovering, const QString& pressed, const QString& disable = {});
    
    void set_button_disable(bool disable);
    
  protected:
    void mousePressEvent(QMouseEvent* event) override;
    
    void mouseReleaseEvent(QMouseEvent* event) override;
    
    void enterEvent(QEnterEvent* event) override;
    
    void leaveEvent(QEvent* event) override;
    
    void paintEvent(QPaintEvent* event) override;

  signals:
    
  private:
    QString normal_;
    QString hovering_;
    QString pressed_;
    QString disable_;
    QPixmap image_;
    bool is_disable_;
};

#endif  // MYBUTTON_H
