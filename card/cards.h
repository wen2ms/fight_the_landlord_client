#ifndef CARDS_H
#define CARDS_H

#include <QSet>

#include "card.h"

class Cards {
  public:
    enum SortType {
        kAscending,
        kDescending,
        kNoSorting
    };
    
    Cards();
    
    Cards(const Card& card);
    
    void add(const Card& card);
    void add(const Cards& cards);
    
    Cards& operator<<(const Card& card);
    Cards& operator<<(const Cards& cards);
    
    void remove(const Card& card);
    void remove(const Cards& cards);
    void remove(const QVector<Cards>& cards_list);
    
    int cards_count();
    
    bool is_empty();
    
    void clear();
    
    Card::CardRank max_rank();
    Card::CardRank min_rank();
    
    int rank_count(Card::CardRank rank);
    
    bool contains(const Card& card);
    bool contains(const Cards& cards);
    
    Card take_random_card();
    
    Card::CardList to_card_list(SortType type = kDescending);
    
  private:
    QSet<Card> cards_;
};

#endif  // CARDS_H
