#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QWidget>
#include <QTimer>
#include <QPaintEvent>

class CountDown : public QWidget {
    Q_OBJECT
  public:
    explicit CountDown(QWidget *parent = nullptr);
    
    void show_count_down();
    
    void stop_count_down();

  signals:
    void not_enough_time();
    void timeout();
    
  protected:
    void paintEvent(QPaintEvent* event) override;
    
  private:
    QPixmap clock_;
    QPixmap number_;
    QTimer* timer_;
    
    int count_;
};

#endif  // COUNTDOWN_H
