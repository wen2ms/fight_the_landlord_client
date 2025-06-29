#ifndef ROBOTPLAYAHAND_H
#define ROBOTPLAYAHAND_H

#include <QObject>
#include <QRunnable>

#include "robot.h"

class RobotPlayAHand : public QObject, public QRunnable {
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
