#include "playahand.h"

PlayAHand::PlayAHand() : hand_type_(HandType::kHandUnknown), card_rank_(Card::CardRank::kRankBegin),
      extra_info_(0) {}

PlayAHand::PlayAHand(Cards &cards) {
    classify(cards);
    
    judge_card_type();
}

PlayAHand::PlayAHand(HandType type, Card::CardRank rank, int extra_info) {
    hand_type_ = type;
    card_rank_ = rank;
    extra_info_ = extra_info;   
}

void PlayAHand::classify(Cards &cards) {
    Card::CardList card_list = cards.to_card_list();
    int card_record[Card::CardRank::kRankEnd];
    
    memset(card_record, 0, sizeof(int) * Card::CardRank::kRankEnd);
    
    for (int i = 0; i < card_list.size(); ++i) {
        Card card = card_list.at(i);
        
        card_record[card.rank()]++;
    }
    
    one_card_.clear();
    two_card_.clear();
    three_card_.clear();
    four_card_.clear();
    
    for (int i = 0; i < Card::CardRank::kRankEnd; ++i) {
        if (card_record[i] == 1) {
            one_card_.push_back((Card::CardRank)i);
        } else if (card_record[i] == 2) {
            two_card_.push_back((Card::CardRank)i);
        } else if (card_record[i] == 3) {
            three_card_.push_back((Card::CardRank)i);
        } else if (card_record[i] == 4) {
            four_card_.push_back((Card::CardRank)i);
        }
    }
}

void PlayAHand::judge_card_type() {
    hand_type_ = HandType::kHandUnknown;
    card_rank_ = Card::CardRank::kRankBegin;
    extra_info_ = 0;
    
    if (is_pass()) {
        hand_type_ = HandType::kHandPass;
    } else if (is_single()) {
        hand_type_ = HandType::kHandSingle;
        card_rank_ = one_card_[0];
    } else if (is_pair()) {
        hand_type_ = HandType::kHandPair;
        card_rank_ = two_card_[0];
    } else if (is_triple()) {
        hand_type_ = HandType::kHandTriple;
        card_rank_ = three_card_[0];
    } else if (is_triple_single()) {
        hand_type_ = HandType::kHandTripleSingle;
        card_rank_ = three_card_[0];
    } else if (is_triple_pair()) {
        hand_type_ = HandType::kHandTriplePair;
        card_rank_ = three_card_[0];
    } else if (is_plane()) {
        hand_type_ = HandType::kHandPlane;
        card_rank_ = three_card_[0];
    } else if (is_plane_two_single()) {
        hand_type_ = HandType::kHandPlaneTwoSingle;
        card_rank_ = three_card_[0];
    } else if (is_plane_two_pair()) {
        hand_type_ = HandType::kHandPlaneTwoPair;
        card_rank_ = three_card_[0];
    } else if (is_seq_pair()) {
        hand_type_ = HandType::kHandSeqPair;
        card_rank_ = two_card_[0];
        extra_info_ = two_card_.size();
    } else if (is_seq_single()) {
        hand_type_ = HandType::kHandSeqSingle;
        card_rank_ = one_card_[0];
        extra_info_ = one_card_.size();
    } else if (is_bomb()) {
        hand_type_ = HandType::kHandBomb;
        card_rank_ = four_card_[0];
    } else if (is_bomb_single()) {
        hand_type_ = HandType::kHandBombSingle;
        card_rank_ = four_card_[0];
    } else if (is_bomb_pair()) {
        hand_type_ = HandType::kHandBombPair;
        card_rank_ = four_card_[0];
    } else if (is_bomb_two_single()) {
        hand_type_ = HandType::kHandBombTwoSingle;
        card_rank_ = four_card_[0];
    } else if (is_bomb_jokers()) {
        hand_type_ = HandType::kHandBombJokers;
    } else if (is_bomb_jokers_single()) {
        hand_type_ = HandType::kHandBombJokersSingle; 
    } else if (is_bomb_jokers_pair()) {
        hand_type_ = HandType::kHandBombJokersPair; 
    } else if (is_bomb_jokers_two_single()) {
        hand_type_ = HandType::kHandBombJokersTwoSingle; 
    }
}

PlayAHand::HandType PlayAHand::hand_type() const {
    return hand_type_;
}

Card::CardRank PlayAHand::card_rank() const {
    return card_rank_;
}

int PlayAHand::extra_info() const {
    return extra_info_;
}

bool PlayAHand::can_beat(const PlayAHand &other) const {
    if (hand_type_ == HandType::kHandUnknown) {
        return false;
    }
    
    if (other.hand_type_ == HandType::kHandPass) {
        return true;
    }
    
    if (hand_type_ == HandType::kHandBombJokers) {
        return true;
    }
    
    if (hand_type_ == HandType::kHandBomb && other.hand_type_ >= HandType::kHandSingle
        && other.hand_type_ <= HandType::kHandSeqSingle) {
        return true;
    }
    
    if (hand_type_ == other.hand_type_) {
        if (hand_type_ == HandType::kHandSeqPair || hand_type_ == HandType::kHandSeqSingle) {
            return card_rank_ > other.card_rank_ && extra_info_ == other.extra_info_;
        } else {
            return card_rank_ > other.card_rank_;
        }
    }
    
    return false;
}

bool PlayAHand::is_pass() {
    if (one_card_.empty() && two_card_.empty() && three_card_.empty() && four_card_.empty()) {
        return true;
    }
    
    return false;
}

bool PlayAHand::is_single() {
    if (one_card_.size() == 1 && two_card_.empty() && three_card_.empty() && four_card_.empty()) {
        return true;
    }
    
    return false;
}

bool PlayAHand::is_pair() {
    if (one_card_.empty() && two_card_.size() == 1 && three_card_.empty() && four_card_.empty()) {
        return true;
    }
    
    return false;
}

bool PlayAHand::is_triple() {
    if (one_card_.empty() && two_card_.empty() && three_card_.size() == 1 && four_card_.empty()) {
        return true;
    }
    
    return false;
}

bool PlayAHand::is_triple_single() {
    if (one_card_.size() == 1 && two_card_.empty() && three_card_.size() == 1 && four_card_.empty()) {
        return true;
    }
    
    return false;
}

bool PlayAHand::is_triple_pair() {
    if (one_card_.empty() && two_card_.size() == 1 && three_card_.size() == 1 && four_card_.empty()) {
        return true;
    }
    
    return false;
}

bool PlayAHand::is_plane() {
    if (one_card_.empty() && two_card_.empty() && three_card_.size() == 2 && four_card_.empty()) {
        std::sort(three_card_.begin(), three_card_.end());
        
        if (three_card_[1] - three_card_[0] == 1 && three_card_[1] < Card::CardRank::kCard2) {
            return true;   
        }
    }
    
    return false;
}

bool PlayAHand::is_plane_two_single() {
    if (one_card_.size() == 2 && two_card_.empty() && three_card_.size() == 2 && four_card_.empty()) {
        std::sort(three_card_.begin(), three_card_.end());
        std::sort(one_card_.begin(), one_card_.end());
        
        if (three_card_[1] - three_card_[0] == 1 && three_card_[1] < Card::CardRank::kCard2
            && one_card_[0] != Card::CardRank::kCardSJ && one_card_[1] != Card::CardRank::kCardBJ) {
            return true;   
        }
    }
    
    return false;
}

bool PlayAHand::is_plane_two_pair() {
    if (one_card_.empty() && two_card_.size() == 2 && three_card_.size() == 2 && four_card_.empty()) {
        std::sort(three_card_.begin(), three_card_.end());
        
        if (three_card_[1] - three_card_[0] == 1 && three_card_[1] < Card::CardRank::kCard2) {
            return true;   
        }
    }
    
    return false;
}

bool PlayAHand::is_seq_pair() {
    if (one_card_.empty() && two_card_.size() >= 3 && three_card_.empty() && four_card_.empty()) {
        std::sort(two_card_.begin(), two_card_.end());
        
        if (two_card_.last() - two_card_.first() == (two_card_.size() - 1)
            && two_card_.first() >= Card::CardRank::kCard3 && two_card_.last() < Card::CardRank::kCard2) {
            return true;   
        }
    }
    
    return false;
}

bool PlayAHand::is_seq_single() {
    if (one_card_.size() >= 5 && two_card_.empty() && three_card_.empty() && four_card_.empty()) {
        std::sort(one_card_.begin(), one_card_.end());
        
        if (one_card_.last() - one_card_.first() == (one_card_.size() - 1)
            && one_card_.first() >= Card::CardRank::kCard3 && one_card_.last() < Card::CardRank::kCard2) {
            return true;   
        }
    }
    
    return false;
}

bool PlayAHand::is_bomb() {
    if (one_card_.empty() && two_card_.empty() && three_card_.empty() && four_card_.size() == 1) {
        return true;   
    }
    
    return false;
}

bool PlayAHand::is_bomb_single() {
    if (one_card_.size() == 1 && two_card_.empty() && three_card_.empty() && four_card_.size() == 1) {
        return true;   
    }
    
    return false;
}

bool PlayAHand::is_bomb_pair() {
    if (one_card_.empty() && two_card_.size() == 1 && three_card_.empty() && four_card_.size() == 1) {
        return true;   
    }
    
    return false;
}

bool PlayAHand::is_bomb_two_single() {
    if (one_card_.size() == 2 && two_card_.empty() && three_card_.empty() && four_card_.size() == 1) {
        std::sort(one_card_.begin(), one_card_.end());
        
        if (one_card_[0] != Card::CardRank::kCardSJ && one_card_[1] != Card::CardRank::kCardBJ) {
            return true;   
        } 
    }
    
    return false;
}

bool PlayAHand::is_bomb_jokers() {
    if (one_card_.size() == 2 && two_card_.empty() && three_card_.empty() && four_card_.empty()) {
        std::sort(one_card_.begin(), one_card_.end());
        
        if (one_card_[0] == Card::CardRank::kCardSJ && one_card_[1] == Card::CardRank::kCardBJ) {
            return true;   
        } 
    }
    
    return false;
}

bool PlayAHand::is_bomb_jokers_single() {
    if (one_card_.size() == 3 && two_card_.empty() && three_card_.empty() && four_card_.empty()) {
        std::sort(one_card_.begin(), one_card_.end());
        
        if (one_card_[1] == Card::CardRank::kCardSJ && one_card_[2] == Card::CardRank::kCardBJ) {
            return true;   
        } 
    }
    
    return false;
}

bool PlayAHand::is_bomb_jokers_pair() {
    if (one_card_.size() == 2 && two_card_.size() == 1 && three_card_.empty() && four_card_.empty()) {
        std::sort(one_card_.begin(), one_card_.end());
        
        if (one_card_[0] == Card::CardRank::kCardSJ && one_card_[1] == Card::CardRank::kCardBJ) {
            return true;   
        } 
    }
    
    return false;
}

bool PlayAHand::is_bomb_jokers_two_single() {
    if (one_card_.size() == 4 && two_card_.empty() && three_card_.empty() && four_card_.empty()) {
        std::sort(one_card_.begin(), one_card_.end());
        
        if (one_card_[2] == Card::CardRank::kCardSJ && one_card_[3] == Card::CardRank::kCardBJ) {
            return true;   
        } 
    }
    
    return false;
}
