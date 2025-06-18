#include "gamecontrol.h"

#include <QRandomGenerator>
#include <QTimer>

#include "playahand.h"

GameControl::GameControl(QObject *parent) : QObject{parent}, left_robot_(nullptr), right_robot_(nullptr),
      user_player_(nullptr), current_player_(nullptr), pending_player_(nullptr), bid_points_(0) {}

void GameControl::player_init() {
    left_robot_ = new Robot("Robot Left", this);
    right_robot_ = new Robot("Robot Right", this);
    user_player_ = new UserPlayer("Mine", this);
    
    left_robot_->set_direction(Player::Direction::kLeft);
    right_robot_->set_direction(Player::Direction::kRight);
    user_player_->set_direction(Player::Direction::kRight);
    
    Player::Sex sex;
    
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    left_robot_->set_sex(sex);
    
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    right_robot_->set_sex(sex);
    
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    user_player_->set_sex(sex);
    
    user_player_->set_prev_player(left_robot_);
    user_player_->set_next_player(right_robot_);
    
    left_robot_->set_prev_player(right_robot_);
    left_robot_->set_next_player(user_player_);
    
    right_robot_->set_prev_player(user_player_);
    right_robot_->set_next_player(left_robot_);
    
    current_player_ = user_player_;
    
    connect(user_player_, &UserPlayer::notify_bid_lord, this, &GameControl::on_bid_lord);
    connect(left_robot_, &Robot::notify_bid_lord, this, &GameControl::on_bid_lord);
    connect(right_robot_, &Robot::notify_bid_lord, this, &GameControl::on_bid_lord);
    
    connect(this, &GameControl::pending_info, user_player_, &UserPlayer::set_pending_info);
    connect(this, &GameControl::pending_info, left_robot_, &Robot::set_pending_info);
    connect(this, &GameControl::pending_info, right_robot_, &Robot::set_pending_info);
    
    connect(user_player_, &UserPlayer::notify_play_a_hand, this, &GameControl::on_play_a_hand);
    connect(left_robot_, &Robot::notify_play_a_hand, this, &GameControl::on_play_a_hand);
    connect(right_robot_, &Robot::notify_play_a_hand, this, &GameControl::on_play_a_hand);
}

Robot *GameControl::left_robot() {
    return left_robot_;
}

Robot *GameControl::right_robot() {
    return right_robot_;
}

UserPlayer *GameControl::user_player() {
    return user_player_;
}

void GameControl::set_current_player(Player *player) {
    current_player_ = player;
}

Player *GameControl::current_player() {
    return current_player_;
}

Player *GameControl::pending_player() {
    return pending_player_;
}

Cards GameControl::pending_cards() const {
    return pending_cards_;
}

void GameControl::init_all_cards() {
    all_cards_.clear();
    
    for (int suit = Card::CardSuit::kSuitBegin + 1; suit < Card::CardSuit::kSuitEnd; ++suit) {
        for (int rank = Card::CardRank::kRankBegin + 1; rank < Card::CardRank::kCardSJ; ++rank) {
            Card card((Card::CardSuit)suit, (Card::CardRank)rank);
            
            all_cards_.add(card);
        }
    }
    
    all_cards_.add(Card(Card::CardSuit::kSuitBegin, Card::CardRank::kCardSJ));
    all_cards_.add(Card(Card::CardSuit::kSuitBegin, Card::CardRank::kCardBJ));
}

Card GameControl::take_one_card() {
    return all_cards_.take_random_card();   
}

Cards GameControl::take_remaining_cards() const {
    return all_cards_;
}

void GameControl::reset_all_cards() {
    init_all_cards();
    
    left_robot_->clear_cards();
    right_robot_->clear_cards();
    user_player_->clear_cards();
    
    pending_player_ = nullptr;
    pending_cards_.clear();
}

void GameControl::start_bid_lord() {
    current_player_->prepare_bid_lord();
    
    emit player_status_changed(current_player_, PlayerStatus::kPrepareBidLord);
}

void GameControl::set_lord(Player* player, int points) {
    bid_points_ = points;
    
    player->set_role(Player::Role::kLord);
    
    player->prev_player()->set_role(Player::Role::kFarmer);
    player->next_player()->set_role(Player::Role::kFarmer);
    
    current_player_ = player;
    
    player->store_dealt_card(all_cards_);
    
    QTimer::singleShot(1000, this, [=]() {
        emit game_status_changed(GameStatus::kPlayingAHand);
        emit player_status_changed(player, PlayerStatus::kPreparePlayAHand);
        
        current_player_->prepare_play_a_hand();
    });
}

void GameControl::clear_player_score() {
    left_robot_->set_score(0);
    right_robot_->set_score(0);
    user_player_->set_score(0);
}

int GameControl::max_bidding_points() const {
    return bidding_record_.points;
}

void GameControl::on_bid_lord(Player *player, int points) {
    if (points == 0 || bidding_record_.points >= points) {
        emit notify_bid_lord(player, 0, false);
    } else if (points > 0 && bidding_record_.points == 0) {
        emit notify_bid_lord(player, points, true);
    } else {
        emit notify_bid_lord(player, points, false);
    }
    
    if (points == 3) {
        set_lord(player, points);
        
        bidding_record_.reset();
        return;
    }
    
    if (points > bidding_record_.points) {
        bidding_record_.points = points;
        bidding_record_.player = player;
    }
    
    bidding_record_.bidding_times++;
    
    if (bidding_record_.bidding_times == 3) {
        if (bidding_record_.points == 0) {
            emit game_status_changed(GameStatus::kDealingCard);
        } else {
            set_lord(bidding_record_.player, bidding_record_.points);
        }
        
        bidding_record_.reset();
        return;
    }
    
    current_player_ = player->next_player();
    
    emit player_status_changed(current_player_, PlayerStatus::kPrepareBidLord);
    current_player_->prepare_bid_lord();
}

void GameControl::on_play_a_hand(Player *player, Cards& cards) {
    emit notify_play_a_hand(player, cards);
    
    if (!cards.is_empty()) {
        pending_cards_ = cards;
        pending_player_ = player;
        
        emit pending_info(player, cards);
    }
    
    PlayAHand::HandType hand_type = PlayAHand(cards).hand_type();
    
    if (hand_type == PlayAHand::HandType::kHandBomb || hand_type == PlayAHand::HandType::kHandBombJokers) {
        bid_points_ *= 2;
    }
    
    if (player->cards().is_empty()) {
        Player* prev_player = player->prev_player();
        Player* next_player = player->next_player();
        
        if (player->role() == Player::Role::kLord) {
            player->set_score(player->score() + 2 * bid_points_);
            prev_player->set_score(prev_player->score() - bid_points_);
            next_player->set_score(next_player->score() - bid_points_);
            
            player->set_win(true);
            prev_player->set_win(false);
            next_player->set_win(false);
        } else {
            player->set_win(true);
            player->set_score(player->score() + bid_points_);
            
            if (prev_player->role() == Player::Role::kLord) {
                prev_player->set_score(prev_player->score() - 2 * bid_points_);
                next_player->set_score(next_player->score() + bid_points_);
                
                prev_player->set_win(false);
                next_player->set_win(true);
            } else {
                prev_player->set_score(prev_player->score() + bid_points_);
                next_player->set_score(next_player->score() - 2 * bid_points_);
                
                prev_player->set_win(true);
                next_player->set_win(false);
            }
        }
        
        emit player_status_changed(player, GameControl::PlayerStatus::kWin);
        return;
    }
    
    current_player_ = player->next_player();
    current_player_->prepare_play_a_hand();
    
    emit player_status_changed(current_player_, GameControl::PlayerStatus::kPreparePlayAHand);
}
