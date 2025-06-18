#ifndef BGMCONTROL_H
#define BGMCONTROL_H

#include <QAudioOutput>
#include <QMediaPlayer>
#include <QObject>

#include "cards.h"

class BGMControl : public QObject {
    Q_OBJECT
  public:
    enum PlayerSex {kMale, kFemale};
    
    enum CardType
    {
        kThree,
        kFour,
        kFive,
        kSix,
        kSeven,
        kEight,
        kNine,
        kTen,
        kJack,
        kQueen,
        kKing,
        kAce,
        kTow,
        kSmallJoker,
        kBigJoker,

        kThreeDouble,
        kFourDouble,
        kFiveDouble,
        kSixDouble,
        kSevenDouble,
        kEightDouble,
        kNineDouble,
        kTenDouble,
        kJackDouble,
        kQueenDouble,
        kKingDouble,
        kAceDouble ,
        kTowDouble,
        
        kThreeTriple,
        kFourTriple,
        kFiveTriple,
        kSixTriple,
        kSevenTriple,
        kEightTriple,
        kNineTriple,
        kTenTriple,
        kJackTriple,
        kQueenTriple,
        kKingTriple,
        kAceTriple,
        kTowTriple,
        
        kPlane,
        kSequencePair,
        kThreeBindOne,
        kThreeBindPair,
        kSequence,
        kFourBindTwo,
        kFourBind2Pair,
        kBomb,
        kJokerBomb,
        kPass1,
        kPass2,
        kPass3,
        kPass4,
        kMoreBiger1,
        kMoreBiger2,
        kBiggest,

        kNoOrder,
        kNoRob, 
        kOrder,
        kRob1, 
        kRob2,
        kLast1, 
        kLast2
    };
    
    enum AuxiliaryMusic {
        kDealingCard,
        kSelectCard,
        kPlaneBackground,
        kBombBackground,
        kAlert
    };
    
    struct PlayerWrapper {
        enum PlayMode {kOnce, kLoop};
        
        void play() {
            if (play_list.empty()) {
                return;
            }
            
            player->setSource(play_list[current_index]);
            player->play();
        }
        
        void stop() {
            player->stop();
        }
        
        void set_index(int index) {
            current_index = index;
        }
        
        QMediaPlayer* player;
        QAudioOutput* audio_output;
        QVector<QUrl> play_list;
        int current_index = 0;
        PlayMode mode;
    };
        
    explicit BGMControl(QObject *parent = nullptr);
        
    void init_play_list();
    
    void start_bgm(float volume);
    
    void stop_bgm();
    
    void player_bidding_music(int points, PlayerSex sex, bool is_first);
    
    void play_card_music(Cards& cards, PlayerSex sex, bool is_first);
    void play_last_cards_musice(CardType type, PlayerSex sex);
    
    void play_pass_music(PlayerSex sex);
    
    void play_auxiliary_music(AuxiliaryMusic type);
    
    void stop_auxiliary_music();
    
    void play_ending_music(bool is_win);

  signals:
    
  private:
    QVector<PlayerWrapper*> wrappers_list_;  
};

#endif  // BGMCONTROL_H
