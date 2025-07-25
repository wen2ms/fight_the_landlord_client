#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QByteArray>

#include "communication.h"

class DataManager {
  public:
    DataManager(const DataManager &other) = delete;
    DataManager &operator=(const DataManager &other) = delete;

    static DataManager *get_instance();

    void set_user_name(QByteArray name);
    void set_ip(QByteArray ip);
    void set_port(QByteArray port);
    
    void set_communication(Communication* comm);

    QByteArray user_name();
    QByteArray ip();
    QByteArray port();
    
    Communication* communication();

  private:
    DataManager() = default;

    static DataManager *data_;

    QByteArray user_name_;
    QByteArray ip_;
    QByteArray port_;
    
    Communication* comm_;
};

#endif // DATAMANAGER_H
