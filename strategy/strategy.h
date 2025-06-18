#ifndef STRATEGY_H
#define STRATEGY_H

#include "player.h"
#include "playahand.h"

class Strategy {
  public:
    Strategy(Player* player, const Cards& cards);
    
    Cards make_strategy();
    
    Cards first_play();
    
    Cards get_greater_cards(PlayAHand hand);
    
    bool whether_to_beat(Cards& beat_cards);
    
    Cards find_same_rank_cards(Card::CardRank rank, int count);
    
    QVector<Cards> find_cards_by_count(int count);
    
    Cards get_range_cards(Card::CardRank begin, Card::CardRank end);
    
    QVector<Cards> find_cards_by_type(PlayAHand hand, bool beat);
    
    void pick_seq_singles(QVector<QVector<Cards>>& all_seq_list_record, const QVector<Cards>& current_seq_single_list,
                          const Cards& cards);
    
    QVector<Cards> pick_optimal_seq_singles();
    
  private:
    using seq_cards_function = Cards (Strategy::*)(Card::CardRank rank);
    struct SeqCardsInfo {
        Card::CardRank rank_begin;
        Card::CardRank rank_end;
        int extra_info;
        bool beat;
        int search_number;
        int base_count;
        seq_cards_function get_base_seq;
    };
    
    QVector<Cards> get_satisfied_cards(Card::CardRank rank_begin, int count);
    
    QVector<Cards> get_triple_single_or_pair(Card::CardRank rank_begin, PlayAHand::HandType hand_type);
    
    QVector<Cards> get_plane(Card::CardRank rank_begin);
    
    QVector<Cards> get_plane_two_single_or_two_pair(Card::CardRank rank_begin, PlayAHand::HandType hand_type);
    
    QVector<Cards> get_seq_pair_or_seq_single(const SeqCardsInfo& info);
    
    Cards get_base_seq_pair(Card::CardRank rank);
    Cards get_base_seq_single(Card::CardRank rank);
    
    QVector<Cards> get_bomb(Card::CardRank rank_begin);
        
    Player* player_;
    Cards cards_;
};

#endif  // STRATEGY_H
