#ifndef PLAYAHAND_H
#define PLAYAHAND_H

#include "cards.h"

class PlayAHand {
  public:
    enum HandType
    {
        kHandUnknown,
        kHandPass,
        
        kHandSingle,
        kHandPair,
        
        kHandTriple,
        kHandTripleSingle,
        kHandTriplePair,
        
        kHandPlane,
        kHandPlaneTwoSingle,
        kHandPlaneTwoPair,
        
        kHandSeqPair,
        kHandSeqSingle,
        
        kHandBomb,
        kHandBombSingle,
        kHandBombPair,
        kHandBombTwoSingle,
        
        kHandBombJokers,
        kHandBombJokersSingle,
        kHandBombJokersPair,
        kHandBombJokersTwoSingle
    };

    
    PlayAHand();
    PlayAHand(Cards& cards);
    PlayAHand(HandType type, Card::CardRank rank, int extra_info);
    
    HandType hand_type() const;
    
    Card::CardRank card_rank() const;
    
    int extra_info() const;
    
    bool can_beat(const PlayAHand& other) const;
    
  private:
    void classify(Cards& cards);
    
    void judge_card_type();
    
    bool is_pass();
    
    bool is_single();
    bool is_pair();
    
    bool is_triple();
    bool is_triple_single();
    bool is_triple_pair();
    
    bool is_plane();
    bool is_plane_two_single();
    bool is_plane_two_pair();
    
    bool is_seq_pair();
    bool is_seq_single();
    
    bool is_bomb();
    bool is_bomb_single();
    bool is_bomb_pair();
    bool is_bomb_two_single();
    bool is_bomb_jokers();
    bool is_bomb_jokers_single();
    bool is_bomb_jokers_pair();
    bool is_bomb_jokers_two_single();
    
    HandType hand_type_;
    Card::CardRank card_rank_;
    int extra_info_;
    
    QVector<Card::CardRank> one_card_;
    QVector<Card::CardRank> two_card_;
    QVector<Card::CardRank> three_card_;
    QVector<Card::CardRank> four_card_;
};

#endif  // PLAYAHAND_H
