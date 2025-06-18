#include "player.h"

Player::Player(QObject *parent) : QObject{parent}, score_(0), is_win_(false), prev_player_(nullptr), next_player_(nullptr),
      pending_player_(nullptr) {}

Player::Player(QString name, QObject *parent) : Player(parent) {
    name_ = name;
}

void Player::set_name(QString name) {
    name_ = name;
}

QString Player::name() const {
    return name_;
}

void Player::set_role(Role role) {
    role_ = role;
}

Player::Role Player::role() const {
    return role_;
}

void Player::set_sex(Sex sex) {
    sex_ = sex;
}

Player::Sex Player::sex() const {
    return sex_;
}

void Player::set_direction(Direction direction) {
    direction_ = direction;
}

Player::Direction Player::direction() const {
    return direction_;
}

void Player::set_type(Type type) {
    type_ = type;
}

Player::Type Player::type() const {
    return type_;
}

void Player::set_score(int score) {
    score_ = score;
}

int Player::score() const {
    return score_;
}

void Player::set_win(bool flag) {
    is_win_ = flag;
}

bool Player::is_win() const {
    return is_win_;
}

void Player::set_prev_player(Player* player) {
    prev_player_ = player;
}

void Player::set_next_player(Player* player) {
    next_player_ = player;
}

Player* Player::prev_player() {
    return prev_player_;
}

Player* Player::next_player() {
    return next_player_;
}

void Player::bid_lord(int points) {
    emit notify_bid_lord(this, points);
}

void Player::store_dealt_card(const Card& card) {
    cards_.add(card);
    
    Cards cards(card);
    
    emit notify_got_cards(this, cards);
}

void Player::store_dealt_card(const Cards& cards) {
    cards_.add(cards);
    
    emit notify_got_cards(this, cards);
}

Cards Player::cards() const {
    return cards_;
}

void Player::clear_cards() {
    cards_.clear();
}

void Player::play_a_hand(Cards& cards) {
    cards_.remove(cards);
    
    emit notify_play_a_hand(this, cards);
}

void Player::set_pending_info(Player* player, const Cards& cards) {
    pending_player_ = player;
    pending_cards_ = cards;
}

Cards Player::pending_cards() const {
    return pending_cards_;
}

Player* Player::pending_player() {
    return pending_player_;
}

void Player::prepare_bid_lord() {}

void Player::prepare_play_a_hand() {}
