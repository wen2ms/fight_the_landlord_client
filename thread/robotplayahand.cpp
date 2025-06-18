#include "robotplayahand.h"

RobotPlayAHand::RobotPlayAHand(Robot* robot, QObject *parent) : QThread{parent} {
    robot_ = robot;
}

void RobotPlayAHand::run() {
    sleep(2);
    
    robot_->thinking_play_a_hand();
}
