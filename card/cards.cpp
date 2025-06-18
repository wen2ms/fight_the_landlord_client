#include "cards.h"

#include <QRandomGenerator>

Cards::Cards() {}

Cards::Cards(const Card& card) {
    add(card);
}

void Cards::add(const Card& card) {
    cards_.insert(card);    
}

void Cards::add(const Cards& cards) {
    cards_.unite(cards.cards_);   
}

Cards& Cards::operator<<(const Card& card) {
    add(card);
    
    return *this;
}

void Cards::remove(const Card& card) {
    cards_.remove(card);
}

void Cards::remove(const Cards& cards) {
    cards_.subtract(cards.cards_);
}

void Cards::remove(const QVector<Cards>& cards_list) {
    for (int i = 0; i < cards_list.size(); ++i) {
        remove(cards_list.at(i));
    }
}

Cards& Cards::operator<<(const Cards& cards) {
    add(cards);
    
    return *this;
}

int Cards::cards_count() {
    return cards_.size();
}

bool Cards::is_empty() {
    return cards_.isEmpty();
}

void Cards::clear() {
    cards_.clear();
}

Card::CardRank Cards::max_rank() {
    Card::CardRank maximum = Card::CardRank::kRankBegin;
    
    for (auto it = cards_.begin(); it != cards_.end(); ++it) {
        if (it->rank() > maximum) {
            maximum = it->rank();
        }
    }
    
    return maximum;
}

Card::CardRank Cards::min_rank() {
    Card::CardRank minimum = Card::CardRank::kRankEnd;
    
    for (auto it = cards_.begin(); it != cards_.end(); ++it) {
        if (it->rank() < minimum) {
            minimum = it->rank();
        }
    }
    
    return minimum;
}

int Cards::rank_count(Card::CardRank rank) {
    int count = 0;
    for (auto it = cards_.begin(); it != cards_.end(); ++it) {
        if (it->rank() == rank) {
            count++;
        }
    }
    
    return count;
}

bool Cards::contains(const Card& card) {
    return cards_.contains(card);
}

bool Cards::contains(const Cards& cards) {
    return cards_.contains(cards.cards_);
}

Card Cards::take_random_card() {
    int random = QRandomGenerator::global()->bounded(cards_.size());
    
    QSet<Card>::const_iterator it = cards_.constBegin();
    
    for (int i = 0; i < random; ++i) {
        ++it;
    }
    
    Card card = *it;
    
    cards_.erase(it);
    return card;
}

Card::CardList Cards::to_card_list(SortType type) {
    Card::CardList card_list;
    
    for (auto it = cards_.begin(); it != cards_.end(); ++it) {
        card_list.append(*it);
    }
    
    if (type == kAscending) {
        std::sort(card_list.begin(), card_list.end(), less_card);
    } else if (type == kDescending) {
        std::sort(card_list.begin(), card_list.end(), greater_card);
    }
    
    return card_list;
}
