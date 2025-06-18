#include "card.h"

Card::Card() : suit_(CardSuit::kSuitBegin), rank_(CardRank::kRankBegin) {}

Card::Card(CardSuit suit, CardRank rank) {
    set_suit(suit);
    set_rank(rank);
}

void Card::set_suit(CardSuit suit) {
    suit_ = suit;
}

void Card::set_rank(CardRank rank) {
    rank_ = rank;
}

Card::CardSuit Card::suit() const {
    return suit_;
}

Card::CardRank Card::rank() const {
    return rank_;
}

bool less_card(const Card& card1, const Card& card2) {
    if (card1.rank() == card2.rank()) {
        return card1.suit() < card2.suit();
    } else {
        return card1.rank() < card2.rank();
    }
}

bool greater_card(const Card& card1, const Card& card2) {
    if (card1.rank() == card2.rank()) {
        return card1.suit() > card2.suit();
    } else {
        return card1.rank() > card2.rank();
    }
}

bool operator==(const Card& left, const Card& right) {
    return left.rank() == right.rank() && left.suit() == right.suit();
}

uint qHash(const Card& card) {
    return card.rank() * 100 + card.suit();
}

bool operator<(const Card& left, const Card& right) {
    if (left.rank() == right.rank()) {
        return left.suit() > right.suit();
    } else {
        return left.rank() < right.rank();   
    }
}
