#ifndef USERPLAYER_H
#define USERPLAYER_H

#include <QObject>

#include "player.h"

class UserPlayer : public Player {
    Q_OBJECT
  public:
    using Player::Player;
    
    explicit UserPlayer(QObject *parent = nullptr);
    
    void prepare_bid_lord() override;
    void prepare_play_a_hand() override;
    
  signals:
    void start_count_down();
};

#endif  // USERPLAYER_H
