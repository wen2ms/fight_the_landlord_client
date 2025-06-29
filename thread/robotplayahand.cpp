#include "robotplayahand.h"

#include <QThread>

RobotPlayAHand::RobotPlayAHand(Robot* robot, QObject *parent) : QObject{parent}, QRunnable() {
    robot_ = robot;
    
    setAutoDelete(true);
}

void RobotPlayAHand::run() {
    QThread::sleep(2);
    
    robot_->thinking_play_a_hand();
}
