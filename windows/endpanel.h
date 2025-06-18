#ifndef ENDPANEL_H
#define ENDPANEL_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "scorepanel.h"

class EndPanel : public QWidget {
    Q_OBJECT
  public:
    explicit EndPanel(bool is_lord, bool is_win, QWidget *parent = nullptr);
    
    void set_scores(int left_score, int right_score, int my_score);
    
  signals:
    void continue_game();
    
  protected:
    void paintEvent(QPaintEvent* event) override;
    
  private:
    QPixmap background_image_;
    QLabel* title_;
    ScorePanel* score_;
    QPushButton* continue_;
};

#endif  // ENDPANEL_H
