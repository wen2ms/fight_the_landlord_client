#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include <QWidget>
#include <QPixmap>

class AnimationWindow : public QWidget {
    Q_OBJECT
  public:
    enum SeqType {kSeqSingle, kSeqPair};
    
    explicit AnimationWindow(QWidget *parent = nullptr);
    
    void show_bid_points(int points);
    
    void show_sequence(SeqType type);
    
    void show_joker_bomb();
    
    void show_bomb();
    
    void show_plane();
    
  protected:
    void paintEvent(QPaintEvent* event);
    
  signals:
    
  private:
    QPixmap image_;
    int frame_index_;
    int x_;
};

#endif  // ANIMATIONWINDOW_H
