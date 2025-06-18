#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>

#include "robot.h"
#include "userplayer.h"
#include "cards.h"

struct BiddingRecord {
    BiddingRecord() {
        reset();
    }
    
    void reset() {
        player = nullptr;
        points = 0;
        bidding_times = 0;
    }
    
    Player* player;
    int points;
    int bidding_times;
};

class GameControl : public QObject {
    Q_OBJECT
  public:
    enum GameStatus {
        kDealingCard,
        kBiddingLord,
        kPlayingAHand
    };
    
    enum PlayerStatus {
        kPrepareBidLord,
        kPreparePlayAHand,
        kWin
    };
    
    explicit GameControl(QObject *parent = nullptr);
    
    void player_init();
    
    Robot* left_robot();
    Robot* right_robot();
    UserPlayer* user_player();
    
    void set_current_player(Player* player);
    Player* current_player();
    
    Player* pending_player();
    Cards pending_cards() const;
    
    void init_all_cards();
    
    Card take_one_card();
    Cards take_remaining_cards() const;
    
    void reset_all_cards();
    
    void start_bid_lord();
    
    void set_lord(Player* player, int points);
    
    void clear_player_score();
    
    int max_bidding_points() const;
    
    void on_bid_lord(Player* player, int points);
    
    void on_play_a_hand(Player* player, Cards& cards);

  signals:
    void player_status_changed(Player* player, PlayerStatus status);
    
    void notify_bid_lord(Player* player, int points, bool is_first_bidding);
    
    void game_status_changed(GameStatus status);
    
    void notify_play_a_hand(Player* player, Cards cards);
    
    void pending_info(Player* player, Cards& cards);
    
  private:
    Robot* left_robot_;
    Robot* right_robot_;
    UserPlayer* user_player_;
    
    Player* current_player_;
    
    Player* pending_player_;
    Cards pending_cards_;
    
    Cards all_cards_;
    
    BiddingRecord bidding_record_;
    
    int bid_points_;
};

#endif  // GAMECONTROL_H
