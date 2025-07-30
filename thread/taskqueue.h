#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

#include "cards.h"

struct Task {    
    int points = 0;
    Cards cards;
};

class TaskQueue : public QObject {
    Q_OBJECT
  public:
    TaskQueue(const TaskQueue& other) = delete;
    TaskQueue& operator=(const TaskQueue& other) = delete;
    
    static TaskQueue* get_instance();
    
    void add(Task& task);
    Task take();
    int size();
    void clear();

  signals:
    
  private:
    explicit TaskQueue(QObject *parent = nullptr);
    
    static TaskQueue task_queue_;
    QQueue<Task> queue_;
    QMutex mutex_;
    QWaitCondition condition_;
};

#endif  // TASKQUEUE_H
