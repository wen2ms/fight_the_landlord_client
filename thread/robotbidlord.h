#ifndef ROBOTBIDLORD_H
#define ROBOTBIDLORD_H

#include <QThread>

#include "robot.h"

class RobotBidLord : public QThread {
    Q_OBJECT
  public:
    explicit RobotBidLord(Robot* robot, QObject *parent = nullptr);
    
  protected:
    void run() override;    
    
  signals:
    
  private:
    Robot* robot_;
};

#endif  // ROBOTBIDLORD_H
