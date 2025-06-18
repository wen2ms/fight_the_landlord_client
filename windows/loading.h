#ifndef LOADING_H
#define LOADING_H

#include <QWidget>

class Loading : public QWidget {
    Q_OBJECT
  public:
    explicit Loading(QWidget *parent = nullptr);

  signals:
    
  protected:
    void paintEvent(QPaintEvent* event) override;
    
  private:
    QPixmap background_;
    QPixmap progress_;
    int bar_width_ = 15;
};

#endif  // LOADING_H
