#include "bgmcontrol.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QTimer>

#include "playahand.h"

BGMControl::BGMControl(QObject *parent) : QObject{parent} {    
    for (int i = 0; i < 5; ++i) {
        PlayerWrapper* wrapper = new PlayerWrapper;
        QMediaPlayer* player = new QMediaPlayer(this);
        QAudioOutput* audio_output = new QAudioOutput(this);
        
        if (i < 2 || i == 4) {
            wrapper->mode = PlayerWrapper::PlayMode::kOnce;
        }
        
        if (i == 2) {
            wrapper->mode = PlayerWrapper::PlayMode::kLoop;
            
            connect(player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status) {
                if (status == QMediaPlayer::MediaStatus::EndOfMedia) {
                    wrapper->set_index((wrapper->current_index + 1) % wrapper->play_list.size());
                    wrapper->play(); 
                }
            });
        }
        
        audio_output->setVolume(1.0);
        player->setAudioOutput(audio_output);
        
        wrapper->player = player;
        wrapper->audio_output = audio_output;
        
        wrappers_list_.push_back(wrapper);
    }
    
    init_play_list();
}

void BGMControl::init_play_list() {
    QStringList list;
    list << "Man" << "Woman" << "BGM" << "Other" << "Ending";
    
    QFile infile(":/conf/playList.json");
    infile.open(QFile::ReadOnly);
    
    QByteArray json = infile.readAll();
    infile.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject obj = doc.object();
    
    for (int i = 0; i < list.size(); ++i) {
        QString key = list.at(i);
        QJsonArray array = obj.value(key).toArray();
        for (int j = 0; j < array.size(); ++j) {
            wrappers_list_[i]->play_list.push_back(QUrl(array.at(j).toString()));
        }
    }
}

void BGMControl::start_bgm(float volume) {
    wrappers_list_[2]->set_index(0);
    wrappers_list_[2]->audio_output->setVolume(volume);
    wrappers_list_[2]->play();   
}

void BGMControl::stop_bgm() {
    wrappers_list_[2]->stop();
}

void BGMControl::player_bidding_music(int points, PlayerSex sex, bool is_first) {
    int index = (sex == PlayerSex::kMale ? 0 : 1);
    if (is_first && points > 0) {
        wrappers_list_[index]->set_index(CardType::kOrder);
    } else if (points == 0) {
        if (is_first) {
            wrappers_list_[index]->set_index(CardType::kNoOrder);
        } else {
            wrappers_list_[index]->set_index(CardType::kNoRob);
        }
    } else if (points == 2) {
        wrappers_list_[index]->set_index(CardType::kRob1);
    } else if (points == 3) {
        wrappers_list_[index]->set_index(CardType::kRob2);
    }
    
    wrappers_list_[index]->play();
}

void BGMControl::play_card_music(Cards& cards, PlayerSex sex, bool is_first) {
    int index = (sex == PlayerSex::kMale ? 0 : 1);
    
    Card::CardRank rank = Card::CardRank::kRankBegin;
    PlayAHand hand(cards);
    PlayAHand::HandType hand_type = hand.hand_type();
    
    if (hand_type == PlayAHand::HandType::kHandSingle || hand_type == PlayAHand::HandType::kHandPair
        || hand_type == PlayAHand::HandType::kHandTriple) {
        rank = cards.take_random_card().rank();
    }
    
    int type_index = 0;
    switch(hand_type) {
        case PlayAHand::HandType::kHandSingle:
            type_index = rank - 1;
            break;
        case PlayAHand::HandType::kHandPair:
            type_index = rank - 1 + 15;
            break;
        case PlayAHand::HandType::kHandTriple:
            type_index = rank - 1 + 15 + 13;
            break;
        case PlayAHand::HandType::kHandTripleSingle:
            type_index = CardType::kThreeBindOne;
            break;
        case PlayAHand::HandType::kHandTriplePair:
            type_index = CardType::kThreeBindPair;
            break;
        case PlayAHand::HandType::kHandPlane:
        case PlayAHand::HandType::kHandPlaneTwoSingle:
        case PlayAHand::HandType::kHandPlaneTwoPair:
            type_index = CardType::kPlane;
            break;
        case PlayAHand::HandType::kHandSeqPair:
            type_index = CardType::kSequencePair;
            break;
        case PlayAHand::HandType::kHandSeqSingle:
            type_index = CardType::kSequence;
            break;
        case PlayAHand::HandType::kHandBomb:
            type_index = CardType::kBomb;
            break;
        case PlayAHand::HandType::kHandBombJokers:
            type_index = CardType::kJokerBomb;
            break;
        case PlayAHand::HandType::kHandBombSingle:
        case PlayAHand::HandType::kHandBombPair:
        case PlayAHand::HandType::kHandBombTwoSingle:
        case PlayAHand::HandType::kHandBombJokersSingle:
        case PlayAHand::HandType::kHandBombJokersPair:
        case PlayAHand::HandType::kHandBombJokersTwoSingle:
            type_index = CardType::kFourBindTwo;
            break;
        default:
            break;
    }
    
    if (!is_first && (type_index >= CardType::kPlane && type_index <= CardType::kFourBindTwo)) {
        wrappers_list_[index]->set_index(CardType::kMoreBiger1 + QRandomGenerator::global()->bounded(2));
    } else {
        wrappers_list_[index]->set_index(type_index);
    }
    
    wrappers_list_[index]->play();
    
    if (type_index == CardType::kBomb || type_index == CardType::kJokerBomb) {
        play_auxiliary_music(AuxiliaryMusic::kBombBackground);
    } else if (type_index == CardType::kPlane) {
        play_auxiliary_music(AuxiliaryMusic::kPlaneBackground);   
    }
}

void BGMControl::play_last_cards_musice(CardType type, PlayerSex sex) {
    int index = (sex == PlayerSex::kMale ? 0 : 1);
    if (wrappers_list_[index]->player->playbackState() == QMediaPlayer::StoppedState) {
        wrappers_list_[index]->set_index(type);
        wrappers_list_[index]->play();
    } else {
        QTimer::singleShot(1500, this, [=]() {
            wrappers_list_[index]->set_index(type);
            wrappers_list_[index]->play();
        });
    }
}

void BGMControl::play_pass_music(PlayerSex sex) {
    int index = (sex == PlayerSex::kMale ? 0 : 1);
    int random = QRandomGenerator::global()->bounded(4);
    
    wrappers_list_[index]->set_index(CardType::kPass1 + random);
    wrappers_list_[index]->play();
}

void BGMControl::play_auxiliary_music(AuxiliaryMusic type) {
    PlayerWrapper* wrapper = wrappers_list_[3];
    
    if (type == AuxiliaryMusic::kDealingCard) {
        wrapper->mode = PlayerWrapper::PlayMode::kLoop;
        if (wrapper->mode != PlayerWrapper::PlayMode::kLoop) {
            connect(wrapper->player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status) {
                if (status == QMediaPlayer::MediaStatus::EndOfMedia) {
                    wrapper->set_index((wrapper->current_index + 1) % wrapper->play_list.size());
                    wrapper->play(); 
                }
            });
        }
    } else {
        wrapper->mode = PlayerWrapper::PlayMode::kOnce;
        connect(wrapper->player, &QMediaPlayer::mediaStatusChanged, this, [](QMediaPlayer::MediaStatus status) {});
    }
    
    wrapper->set_index(type);
    wrapper->play();   
}

void BGMControl::stop_auxiliary_music() {
    wrappers_list_[3]->stop();
}

void BGMControl::play_ending_music(bool is_win) {
    if (is_win) {
        wrappers_list_[4]->set_index(0);
    } else {
        wrappers_list_[4]->set_index(1);
    }
    
    wrappers_list_[4]->play();
}
