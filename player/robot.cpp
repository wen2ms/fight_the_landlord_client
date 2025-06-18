#include "robot.h"

#include <QDebug>

#include "strategy.h"
#include "robotbidlord.h"
#include "robotplayahand.h"

Robot::Robot(QObject *parent) : Player{parent} {
    type_ = kRobot;
}

void Robot::prepare_bid_lord() {
    RobotBidLord* subthread = new RobotBidLord(this);
    
    connect(subthread, &RobotBidLord::finished, this, [=]() {
        subthread->deleteLater();
    });
    subthread->start();
}

void Robot::prepare_play_a_hand() {
    RobotPlayAHand* subthread = new RobotPlayAHand(this);
    
    connect(subthread, &RobotPlayAHand::finished, this, [=]() {
        subthread->deleteLater();
    });
    subthread->start();
}

void Robot::thinking_bid_lord() {
    int weight = 0;
    
    Strategy strategy(this, cards_);
    
    weight += strategy.get_range_cards(Card::CardRank::kCardSJ, Card::CardRank::kRankEnd).cards_count() * 6;
    
    QVector<Cards> optimal_seq_singles = strategy.pick_optimal_seq_singles();
    weight += optimal_seq_singles.size() * 5;
    
    QVector<Cards> bomb_list = strategy.find_cards_by_count(4);
    weight += bomb_list.size() * 5;
    
    weight += cards_.rank_count(Card::CardRank::kCard2) * 3;
    
    Cards remaining_cards = cards_;
    
    remaining_cards.remove(optimal_seq_singles);
    remaining_cards.remove(bomb_list);
    
    Cards card2 = Strategy(this, remaining_cards).get_range_cards(Card::CardRank::kCard2, Card::CardRank::kCard3);
    
    remaining_cards.remove(card2);
    
    QVector<Cards> triple_list = Strategy(this, remaining_cards).find_cards_by_count(3);
    
    weight += triple_list.size() * 4;
    remaining_cards.remove(triple_list);
    
    QVector<Cards> pair_list = Strategy(this, remaining_cards).find_cards_by_count(2);
    
    weight += pair_list.size() * 1;
    
    if (weight >= 22) {
        bid_lord(3);
    } else if (weight >= 18 && weight < 22) {
        bid_lord(2);
    } else if (weight >= 10 && weight < 18) {
        bid_lord(1);
    } else {
        bid_lord(0);
    }
}

void Robot::thinking_play_a_hand() {
    Strategy strategy(this, cards_);
    
    Cards cards = strategy.make_strategy();
    
    play_a_hand(cards);
}
