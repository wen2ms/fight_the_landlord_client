#ifndef CARD_H
#define CARD_H

#include <QList>

class Card {
  public:
    using CardList = QList<Card>;
    
    enum CardSuit {
        kSuitBegin,
        kDiamond,
        kClub,
        kHeart,
        kSpade,
        kSuitEnd
    };
    
    enum CardRank {
        kRankBegin,
        kCard3,
        kCard4,
        kCard5,
        kCard6,
        kCard7,
        kCard8,
        kCard9,
        kCard10,
        kCardJ,
        kCardQ,
        kCardK,
        kCardA,
        kCard2,
        kCardSJ,
        kCardBJ,
        kRankEnd
    };
    
    Card();
    Card(CardSuit suit, CardRank rank);
    
    void set_suit(CardSuit suit);
    void set_rank(CardRank rank);
    CardSuit suit() const;
    CardRank rank() const;
    
  private:
    CardSuit suit_;
    CardRank rank_;
};

bool less_card(const Card& card1, const Card& card2);
bool greater_card(const Card& card1, const Card& card2);

bool operator==(const Card& left, const Card& right);
bool operator<(const Card& left, const Card& right);
uint qHash(const Card& card);

#endif  // CARD_H
