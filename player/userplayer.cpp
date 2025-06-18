#include "userplayer.h"

UserPlayer::UserPlayer(QObject *parent) : Player{parent} {
    type_ = kUser;
}

void UserPlayer::prepare_bid_lord() {}

void UserPlayer::prepare_play_a_hand() {
    emit start_count_down();
}
