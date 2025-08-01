#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QByteArray>

#include "communication.h"
#include "cards.h"

class DataManager {
  public:
    enum GameModeType {
        kStandalone,
        kOnline
    };

    DataManager(const DataManager &other) = delete;
    DataManager &operator=(const DataManager &other) = delete;

    static DataManager *get_instance();

    void set_user_name(QByteArray name);
    void set_room_name(QByteArray room_name);
    void set_ip(QByteArray ip);
    void set_port(QByteArray port);
    
    void set_communication(Communication* comm);
    void set_cards(Cards cards, Cards last_three_cards);
    void set_game_mode_type(GameModeType mode_type);

    QByteArray user_name();
    QByteArray room_name();
    QByteArray ip();
    QByteArray port();
    
    Communication* communication();
    Cards cards();
    Cards last_three_cards();
    GameModeType game_mode_type();

  private:
    DataManager() = default;

    static DataManager *data_;

    QByteArray user_name_;
    QByteArray room_name_;
    QByteArray ip_ = "<ip>";
    QByteArray port_ = "<port>";
    
    Communication* comm_;
    Cards cards_;
    Cards last_three_cards_;
    GameModeType game_mode_type_; 
};

#endif // DATAMANAGER_H
