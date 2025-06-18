#ifndef ROBOTPLAYAHAND_H
#define ROBOTPLAYAHAND_H

#include <QThread>

#include "robot.h"

class RobotPlayAHand : public QThread {
    Q_OBJECT
  public:
    explicit RobotPlayAHand(Robot* robot, QObject *parent = nullptr);
    
  protected:
    void run() override;

  signals:
    
  private:
    Robot* robot_;
};

#endif  // ROBOTPLAYAHAND_H
