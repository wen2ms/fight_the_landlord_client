#include "robotbidlord.h"

#include <QThread>

RobotBidLord::RobotBidLord(Robot* robot, QObject *parent) : QObject{parent}, QRunnable() {
    robot_ = robot;
    
    setAutoDelete(true);
}

void RobotBidLord::run() {
    QThread::sleep(2);
    
    robot_->thinking_bid_lord();   
}
