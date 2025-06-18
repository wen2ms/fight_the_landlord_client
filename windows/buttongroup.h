#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>

namespace Ui {
class ButtonGroup;
}

class ButtonGroup : public QWidget {
    Q_OBJECT

  public:
    enum Panel {kSatrt, KPlayACard, kPassOrPlay, kBidLord, kEmpty};
    
    explicit ButtonGroup(QWidget *parent = nullptr);
    ~ButtonGroup();
    
    void init_buttons();
    
    void select_panel(Panel panel_type, int bidding_points = 0);
    
  signals:
    void start_game();
    
    void play_a_hand();
    
    void pass();
    
    void bid_points(int points);

  private:
    Ui::ButtonGroup *ui;
};

#endif  // BUTTONGROUP_H
