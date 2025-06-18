#include "strategy.h"

#include <QMap>

Strategy::Strategy(Player *player, const Cards &cards) : player_(player), cards_(cards) {}

Cards Strategy::make_strategy() {
    Player* pending_player = player_->pending_player();
    Cards pending_cards = player_->pending_cards();
    
    if (pending_player == player_ || pending_player == nullptr) {
        return first_play();
    } else {
        PlayAHand hand(pending_cards);
        Cards beat_cards = get_greater_cards(hand);
        bool is_to_beat = whether_to_beat(beat_cards);
        
        if (is_to_beat) {
            return beat_cards;
        } else {
            return Cards();
        }   
    }
}

Cards Strategy::first_play() {
    PlayAHand hand(cards_);
    
    if (hand.hand_type() != PlayAHand::HandType::kHandUnknown) {
        return cards_;
    }
    
    QVector<Cards> optimal_seq_single = pick_optimal_seq_singles();
    
    if (!optimal_seq_single.empty()) {
        int original_singles_count = find_cards_by_count(1).size();
        
        Cards remaining_cards = cards_;
        remaining_cards.remove(optimal_seq_single);
        
        int removed_singles_count = Strategy(player_, remaining_cards).find_cards_by_count(1).size();
        
        if (removed_singles_count < original_singles_count) {
            return optimal_seq_single[0];
        }
    }
    
    bool has_plane = false;
    bool has_triple = false;
    bool has_seq_pair = false;
    
    Cards filtered_cards = cards_;
    
    QVector<Cards> bomb_list = find_cards_by_type(PlayAHand(PlayAHand::HandType::kHandBomb, Card::CardRank::kRankBegin, 0),
                                                  false);
    filtered_cards.remove(bomb_list);
    
    QVector<Cards> plane_list = Strategy(player_, filtered_cards).find_cards_by_type(PlayAHand(PlayAHand::HandType::kHandPlane,
                                                                                               Card::CardRank::kRankBegin, 0),
                                                                                     false);
    if (!plane_list.empty()) {
        has_plane = true;
        filtered_cards.remove(plane_list);
    }
    
    QVector<Cards> triple_list = Strategy(player_, filtered_cards).find_cards_by_type(PlayAHand(PlayAHand::HandType::kHandTriple,
                                                                                               Card::CardRank::kRankBegin, 0),
                                                                                     false);
    if (!triple_list.empty()) {
        has_triple = true;
        filtered_cards.remove(triple_list);
    }
    
    QVector<Cards> seq_pair_list = Strategy(player_, filtered_cards).find_cards_by_type(PlayAHand(PlayAHand::HandType::kHandSeqPair,
                                                                                               Card::CardRank::kRankBegin, 0),
                                                                                     false);
    if (!seq_pair_list.empty()) {
        has_seq_pair = true;
        filtered_cards.remove(seq_pair_list);
    }
    
    if (has_seq_pair) {
        Cards max_seq_pair;
        for (int i = 0; i < seq_pair_list.size(); ++i) {
            if (seq_pair_list[i].cards_count() > max_seq_pair.cards_count()) {
                max_seq_pair = seq_pair_list[i];
            }
        }
        
        return max_seq_pair;
    }
    
    if (has_plane) {
        bool has_two_pair = false;
        QVector<Cards> pair_list;
        
        for (int rank = Card::CardRank::kCard3; rank <= Card::CardRank::kCard10; ++rank) {
            Cards pair = Strategy(player_, filtered_cards).find_same_rank_cards((Card::CardRank)rank, 2);
            if (!pair.is_empty()) {
                pair_list << pair;
                
                if (pair_list.size() == 2) {
                    has_two_pair = true;
                    break;
                }
            }
        }
        
        if (has_two_pair) {
            Cards plane_two_pair = plane_list[0];
            plane_two_pair << pair_list[0] << pair_list[1];
            
            return plane_two_pair;
        } else {
            bool has_two_single = false;
            QVector<Cards> two_single_list;
            
            for (int rank = Card::CardRank::kCard3; rank <= Card::CardRank::kCard10; ++rank) {
                if (filtered_cards.rank_count((Card::CardRank)rank) == 1) {
                    Cards single = Strategy(player_, filtered_cards).find_same_rank_cards((Card::CardRank)rank, 1);
                    if (!single.is_empty()) {
                        two_single_list << single;
                        
                        if (two_single_list.size() == 2) {
                            has_two_single = true;
                            break;
                        }
                    }   
                }
            }
            
            if (has_two_single) {
                Cards plane_two_single = plane_list[0];
                plane_two_single << pair_list[0] << pair_list[1];
                
                return plane_two_single;
            } else {
                return plane_list[0];
            }
        }
    }
    
    if (has_triple) {
        if (PlayAHand(triple_list[0]).card_rank() < Card::kCardA) {
            for (int rank = Card::CardRank::kCard3; rank <= Card::CardRank::kCardA; ++rank) {
                int rank_count = filtered_cards.rank_count((Card::CardRank)rank);
                if (rank_count == 1) {
                    Cards single = Strategy(player_, filtered_cards).find_same_rank_cards((Card::CardRank)rank, 1);
                    Cards triple_single = triple_list[0];
                    
                    triple_single.add(single);
                    
                    return triple_single;
                } else if (rank_count == 2) {
                    Cards pair = Strategy(player_, filtered_cards).find_same_rank_cards((Card::CardRank)rank, 2);
                    Cards triple_pair= triple_list[0];
                    
                    triple_pair.add(pair);
                    
                    return triple_pair;
                }
            }
        }
        
        return triple_list[0];
    }
    
    Player* next_player = player_->next_player();
    if (next_player->role() != player_->role() && next_player->cards().cards_count() == 1) {
        for (int rank = Card::CardRank::kCardBJ; rank >= Card::CardRank::kCard3; --rank) {
            int rank_count = filtered_cards.rank_count((Card::CardRank)rank);
            if (rank_count == 1) {
                Cards single = Strategy(player_, filtered_cards).find_same_rank_cards((Card::CardRank)rank, 1);
                
                return single;
            } else if (rank_count == 2) {
                Cards pair = Strategy(player_, filtered_cards).find_same_rank_cards((Card::CardRank)rank, 2);
                
                return pair;
            }
        }
    } else {
        for (int rank = Card::CardRank::kCard3; rank < Card::CardRank::kRankEnd; ++rank) {
            int rank_count = filtered_cards.rank_count((Card::CardRank)rank);
            if (rank_count == 1) {
                Cards single = Strategy(player_, filtered_cards).find_same_rank_cards((Card::CardRank)rank, 1);
                
                return single;
            } else if (rank_count == 2) {
                Cards pair = Strategy(player_, filtered_cards).find_same_rank_cards((Card::CardRank)rank, 2);
                
                return pair;
            }
        }
    }
    
    return Cards();
}

Cards Strategy::get_greater_cards(PlayAHand hand) {
    Player* pending_player = player_->pending_player();
    
    if (pending_player != nullptr && pending_player->role() != player_->role() && pending_player->cards().cards_count() <= 3) {
        QVector<Cards> bombs = find_cards_by_count(4);
        
        for (int i = 0; i < bombs.size(); ++i) {
            if (PlayAHand(bombs[i]).can_beat(hand)) {
                return bombs[i];
            }
        }
        
        Cards sj_card = find_same_rank_cards(Card::CardRank::kCardSJ, 1);
        Cards bj_card = find_same_rank_cards(Card::CardRank::kCardBJ, 1);
        
        if (!sj_card.is_empty() && !bj_card.is_empty()) {
            Cards joker_bomb;
            
            joker_bomb << sj_card << bj_card;
            return joker_bomb;
        }
    }
    
    Player* next_player = player_->next_player();
    Cards remaining_cards = cards_;
    
    remaining_cards.remove(Strategy(player_, remaining_cards).pick_optimal_seq_singles());
    
    auto find_beat_card = std::bind([=](const Cards& cards) {
        QVector<Cards> beat_cards_list = Strategy(player_, cards).find_cards_by_type(hand, true);
        
        if (!beat_cards_list.empty()) {
            if (pending_player->role() != player_->role() && next_player->cards().cards_count() <= 2) {
                return beat_cards_list.back();
            } else {
                return beat_cards_list.front();
            }
        }
        
        return Cards();
    }, std::placeholders::_1);
    
    Cards cards = find_beat_card(remaining_cards);
    
    if (!cards.is_empty()) {
        return cards;
    } else {
        cards = find_beat_card(cards_);
        
        if (!cards.is_empty()) {
            return cards;
        }
    }
    
    return Cards();
}

bool Strategy::whether_to_beat(Cards& beat_cards) {
    if (beat_cards.is_empty()) {
        return false;
    }
    
    Player* pending_player = player_->pending_player();
    if (pending_player->role() == player_->role()) {
        Cards remaining_cards = cards_;
        
        remaining_cards.remove(beat_cards);
        if (PlayAHand(remaining_cards).hand_type() != PlayAHand::HandType::kHandUnknown) {
            return true;
        }
        
        Card::CardRank beat_card_rank = PlayAHand(beat_cards).card_rank();
        if (beat_card_rank == Card::CardRank::kCard2 || beat_card_rank == Card::CardRank::kCardSJ
            || beat_card_rank == Card::CardRank::kCardBJ) {
            return false;
        }
    } else {
        PlayAHand hand(beat_cards);
        
        if ((hand.hand_type() == PlayAHand::HandType::kHandTripleSingle || hand.hand_type() == PlayAHand::HandType::kHandTriplePair)
            && hand.card_rank() == Card::CardRank::kCard2) {
            return false;
        }
        
        if (hand.hand_type() == PlayAHand::HandType::kHandPair && hand.card_rank() == Card::CardRank::kCard2
            && pending_player->cards().cards_count() >= 10 && player_->cards().cards_count() >= 5) {
            return false;
        }
    }
    
    return true;
}

Cards Strategy::find_same_rank_cards(Card::CardRank rank, int count) {
    if (count < 1 || count > 4) {
        return Cards();
    }
    
    if (rank == Card::CardRank::kCardSJ || rank == Card::CardRank::kCardBJ) {
        if (count > 1) {
            return Cards();
        }
        
        Card card(Card::CardSuit::kSuitBegin, rank);
        
        if (cards_.contains(card)) {
            Cards cards(card);
            
            return cards;
        } else {
            return Cards();
        }
    }
    
    int find_count = 0;
    Cards find_cards;
    for (int suit = Card::CardSuit::kSuitBegin + 1; suit < Card::CardSuit::kSuitEnd; ++suit) {
        Card card((Card::CardSuit)suit, rank);
        
        if (cards_.contains(card)) {
            find_count++;
            find_cards.add(card);
            
            if (find_count == count) {
                return find_cards;
            }
        }
    }
    
    return Cards();
}

QVector<Cards> Strategy::find_cards_by_count(int count) {
    if (count < 1 || count > 4) {
        return QVector<Cards>();
    }
    
    
    QVector<Cards> cards_list;
    for (int rank = Card::CardRank::kCard3; rank < Card::CardRank::kRankEnd; ++rank) {
        if (cards_.rank_count((Card::CardRank)rank) == count) {
            Cards cards;
            
            cards << find_same_rank_cards((Card::CardRank)rank, count);
            
            cards_list << cards;
        }
    }
    
    return cards_list;
}

Cards Strategy::get_range_cards(Card::CardRank begin, Card::CardRank end) {
    Cards range_cards;
    
    for (int rank = begin; rank < end; ++rank) {
        int count = cards_.rank_count((Card::CardRank)rank);
        
        Cards cards = find_same_rank_cards((Card::CardRank)rank, count);
        
        range_cards << cards;
    }
    
    return range_cards;
}

QVector<Cards> Strategy::find_cards_by_type(PlayAHand hand, bool beat) {
    PlayAHand::HandType hand_type = hand.hand_type();
    Card::CardRank card_rank = hand.card_rank();
    int extra_info = hand.extra_info();
    
    Card::CardRank rank_begin = beat ? (Card::CardRank)(card_rank + 1) : Card::CardRank::kCard3;
    
    switch (hand_type) {
        case PlayAHand::HandType::kHandSingle:
            return get_satisfied_cards(rank_begin, 1);
        case PlayAHand::HandType::kHandPair:
            return get_satisfied_cards(rank_begin, 2);
        case PlayAHand::HandType::kHandTriple:
            return get_satisfied_cards(rank_begin, 3);
        case PlayAHand::HandType::kHandTripleSingle:
            return get_triple_single_or_pair(rank_begin, PlayAHand::HandType::kHandSingle);
        case PlayAHand::HandType::kHandTriplePair:
            return get_triple_single_or_pair(rank_begin, PlayAHand::HandType::kHandPair);
        case PlayAHand::HandType::kHandPlane:
            return get_plane(rank_begin);
        case PlayAHand::HandType::kHandPlaneTwoSingle:
            return get_plane_two_single_or_two_pair(rank_begin, PlayAHand::HandType::kHandSingle);
        case PlayAHand::HandType::kHandPlaneTwoPair:
            return get_plane_two_single_or_two_pair(rank_begin, PlayAHand::HandType::kHandPair);
        case PlayAHand::HandType::kHandSeqPair: {
            SeqCardsInfo info;
            info.rank_begin = rank_begin;
            info.rank_end = Card::CardRank::kCardQ;
            info.search_number = 2;
            info.base_count = 3;
            info.extra_info = extra_info;
            info.beat = beat;
            info.get_base_seq = &Strategy::get_base_seq_pair;
            
            return get_seq_pair_or_seq_single(info);
        }
        case PlayAHand::HandType::kHandSeqSingle: {
            SeqCardsInfo info;
            info.rank_begin = rank_begin;
            info.rank_end = Card::CardRank::kCard10;
            info.search_number = 1;
            info.base_count = 5;
            info.extra_info = extra_info;
            info.beat = beat;
            info.get_base_seq = &Strategy::get_base_seq_single;
            
            return get_seq_pair_or_seq_single(info);
        }
        case PlayAHand::HandType::kHandBomb:
            return get_bomb(rank_begin);
        default:
            return QVector<Cards>();
    }
}

void Strategy::pick_seq_singles(QVector<QVector<Cards> >& all_seq_list_record, const QVector<Cards>& current_seq_single_list,
                                const Cards& cards) {
    QVector<Cards> all_seq_list = Strategy(player_, cards).find_cards_by_type(PlayAHand(PlayAHand::HandType::kHandSeqSingle,
                                                                                        Card::CardRank::kRankBegin, 0), false);
    
    if (all_seq_list.empty()) {
        all_seq_list_record << current_seq_single_list;
    } else {
        Cards current_cards = cards;
        for (int i = 0; i < all_seq_list.size(); ++i) {
            Cards seq = all_seq_list.at(i);
            
            Cards remaining_cards = current_cards;
            remaining_cards.remove(seq);
            
            QVector<Cards> seq_single_list = current_seq_single_list;
            seq_single_list << seq;
            
            pick_seq_singles(all_seq_list_record, seq_single_list, remaining_cards);
        }
    }
}

QVector<Cards> Strategy::pick_optimal_seq_singles() {
    QVector<QVector<Cards>> all_seq_list_record;
    QVector<Cards> current_seq_single_list;
    
    Cards filter_cards = cards_;
    filter_cards.remove(find_cards_by_count(4));
    filter_cards.remove(find_cards_by_count(3));
    
    pick_seq_singles(all_seq_list_record, current_seq_single_list, filter_cards);
    
    if (all_seq_list_record.empty()) {
        return QVector<Cards>();
    }
    
    QMap<int, int> remaining_rank_map;
    for (int i = 0; i < all_seq_list_record.size(); ++i) {
        Cards remaining_cards = cards_;
        QVector<Cards> seq_single_list = all_seq_list_record.at(i);
        
        remaining_cards.remove(seq_single_list);
        
        QVector<Cards> single_list = Strategy(player_, remaining_cards).find_cards_by_count(1);
        Card::CardList card_list;
        
        for (int j = 0; j < single_list.size(); ++j) {
            card_list << single_list[j].to_card_list();
        }
        
        int remaining_single_total_rank = 0;
        for (int j = 0; j < card_list.size(); ++j) {
            remaining_single_total_rank += card_list[j].rank() + 15;
        }
        
        remaining_rank_map.insert(i, remaining_single_total_rank);
    }
    
    int optimal_seq_index = 0;
    int min_remaining_total_rank = INT16_MAX;
    
    for (auto it = remaining_rank_map.constBegin(); it != remaining_rank_map.constEnd(); ++it) {
        if (it.value() < min_remaining_total_rank) {
            min_remaining_total_rank = it.value();
            optimal_seq_index = it.key();
        }
    }
    
    return all_seq_list_record[optimal_seq_index];
}

QVector<Cards> Strategy::get_satisfied_cards(Card::CardRank rank_begin, int count) {
    QVector<Cards> find_cards_list;
    
    for (int rank = rank_begin; rank < Card::CardRank::kRankEnd; ++rank) {    
        if (cards_.rank_count((Card::CardRank)rank) == count) {
            Cards cards = find_same_rank_cards((Card::CardRank)rank, count);
            
            find_cards_list << cards;
        }
    }
    
    return find_cards_list;
}

QVector<Cards> Strategy::get_triple_single_or_pair(Card::CardRank rank_begin, PlayAHand::HandType hand_type) {
    QVector<Cards> find_cards_list = get_satisfied_cards(rank_begin, 3);
    
    if (!find_cards_list.empty()) {
        Cards remain_cards = cards_;
        
        remain_cards.remove(find_cards_list);
        
        Strategy strategy(player_, remain_cards);
        QVector<Cards> cards_list = strategy.find_cards_by_type(PlayAHand(hand_type, Card::CardRank::kRankBegin, 0),
                                                                false);
        
        if (!cards_list.empty()) {
            for (int i = 0; i < find_cards_list.size(); ++i) {
                find_cards_list[i].add(cards_list[0]);
            }
        } else {
            find_cards_list.clear();
        }
    }
    
    return find_cards_list;
}

QVector<Cards> Strategy::get_plane(Card::CardRank rank_begin) {
    QVector<Cards> find_cards_list;
    
    for (int rank = rank_begin; rank <= Card::CardRank::kCardK; ++rank) {
        Cards prev_cards = find_same_rank_cards((Card::CardRank)rank, 3);
        Cards next_cards = find_same_rank_cards((Card::CardRank)(rank + 1), 3);
        
        if (!prev_cards.is_empty() && !next_cards.is_empty()) {
            Cards cards;
            
            cards << prev_cards << next_cards;
            find_cards_list << cards;
        }
    }
    
    return find_cards_list;
}

QVector<Cards> Strategy::get_plane_two_single_or_two_pair(Card::CardRank rank_begin, PlayAHand::HandType hand_type) {
    QVector<Cards> find_cards_list = get_plane(rank_begin);
    
    if (!find_cards_list.empty()) {
        Cards remain_cards = cards_;
        
        remain_cards.remove(find_cards_list);
        
        Strategy strategy(player_, remain_cards);
        QVector<Cards> cards_list = strategy.find_cards_by_type(PlayAHand(hand_type, Card::CardRank::kRankBegin, 0),
                                                                false);
        
        if (cards_list.size() >= 2) {
            for (int i = 0; i < find_cards_list.size(); ++i) {
                Cards cards;
                
                cards << cards_list[0] << cards_list[1];
                
                find_cards_list[i].add(cards);
            }
        } else {
            find_cards_list.clear();
        }
    }
    
    return find_cards_list;
}

QVector<Cards> Strategy::get_seq_pair_or_seq_single(const SeqCardsInfo& info) {
    QVector<Cards> find_cards_list;
    
    if (info.beat) {
        for (int rank = info.rank_begin; rank <= info.rank_end; ++rank) {            
            if (rank + info.extra_info >= Card::CardRank::kCard2) {
                break;
            }
            
            bool is_found = true;
            Cards seq_cards;
            for (int i = 0; i < info.extra_info; ++i) {
                Cards cards = find_same_rank_cards((Card::CardRank)(rank + i), info.search_number);
                
                if (cards.is_empty()) {
                    is_found = false;
                    seq_cards.clear();
                    break;
                } else {
                    seq_cards << cards;
                }
            }
            
            if (is_found) {
                find_cards_list << seq_cards;
                return find_cards_list;
            }
        }
    } else {
        for (int rank = info.rank_begin; rank <= info.rank_end; ++rank) {
            Cards base_seq_cards = (this->*info.get_base_seq)((Card::CardRank)rank);
            
            if (base_seq_cards.is_empty()) {
                continue;
            }
                
            find_cards_list << base_seq_cards;
            
            int followed_rank = rank + info.base_count;
            
            while (true) {
                if (followed_rank >= Card::CardRank::kCard2) {
                    break;
                }
                
                Cards followed_cards = find_same_rank_cards((Card::CardRank)followed_rank, info.search_number);
                
                if (followed_cards.is_empty()) {
                    break;
                } else {
                    base_seq_cards << followed_cards;
                    find_cards_list << base_seq_cards;
                    followed_rank++;
                }
            }
        }
    }
    
    return find_cards_list;
}

Cards Strategy::get_base_seq_pair(Card::CardRank rank) {
    Cards base_cards0 = find_same_rank_cards(rank, 2);
    Cards base_cards1 = find_same_rank_cards((Card::CardRank)(rank + 1), 2);
    Cards base_cards2 = find_same_rank_cards((Card::CardRank)(rank + 2), 2);
    Cards base_seq;
    
    if (!base_cards0.is_empty() && !base_cards1.is_empty() && !base_cards2.is_empty()) {
        base_seq << base_cards0 << base_cards1 << base_cards2;
    }
    
    return base_seq;
}

Cards Strategy::get_base_seq_single(Card::CardRank rank) {
    Cards base_cards0 = find_same_rank_cards(rank, 1);
    Cards base_cards1 = find_same_rank_cards((Card::CardRank)(rank + 1), 1);
    Cards base_cards2 = find_same_rank_cards((Card::CardRank)(rank + 2), 1);
    Cards base_cards3 = find_same_rank_cards((Card::CardRank)(rank + 3), 1);
    Cards base_cards4 = find_same_rank_cards((Card::CardRank)(rank + 4), 1);
    Cards base_seq;
    
    if (!base_cards0.is_empty() && !base_cards1.is_empty() && !base_cards2.is_empty() && !base_cards3.is_empty()
        && !base_cards4.is_empty()) {
        base_seq << base_cards0 << base_cards1 << base_cards2 << base_cards3 << base_cards4;
    }
    
    return base_seq;
}

QVector<Cards> Strategy::get_bomb(Card::CardRank rank_begin) {
    QVector<Cards> find_cards_list;
    
    for (int rank = rank_begin; rank < Card::CardRank::kRankEnd; ++rank) {
        Cards cards = find_same_rank_cards((Card::CardRank)rank, 4);
        
        if (!cards.is_empty()) {
            find_cards_list << cards;
        }
    }
    
    return find_cards_list;
}
