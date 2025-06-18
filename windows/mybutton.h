#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include <QPixmap>
#include <QMouseEvent>

class MyButton : public QPushButton {
    Q_OBJECT
  public:
    explicit MyButton(QWidget *parent = nullptr);
    
    void set_image(QString normal, QString hovering, QString pressed);
    
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
    
    QPixmap image_; 
};

#endif  // MYBUTTON_H
