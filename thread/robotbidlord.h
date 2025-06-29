#ifndef ROBOTBIDLORD_H
#define ROBOTBIDLORD_H

#include <QObject>
#include <QRunnable>

#include "robot.h"

class RobotBidLord : public QObject, public QRunnable{
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
