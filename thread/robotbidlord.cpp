#include "robotbidlord.h"


RobotBidLord::RobotBidLord(Robot* robot, QObject *parent) : QThread{parent} {
    robot_ = robot;
}

void RobotBidLord::run() {
    sleep(2);
    
    robot_->thinking_bid_lord();   
}
