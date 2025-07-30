#include "taskqueue.h"

TaskQueue TaskQueue::task_queue_;

TaskQueue::TaskQueue(QObject *parent) : QObject{parent} {}

TaskQueue *TaskQueue::get_instance() {
    return &task_queue_;
}

void TaskQueue::add(Task &task) {
    mutex_.lock();
    queue_.enqueue(task);
    mutex_.unlock();
    condition_.notify_all();
}

Task TaskQueue::take() {
    Task task;
    mutex_.lock();
    while (true) {
        if (!queue_.empty()) {
            task = queue_.dequeue();
            break;
        } else {
            condition_.wait(&mutex_);
        }   
    }
    mutex_.unlock();
    
    return task;
}

int TaskQueue::size() {
    mutex_.lock();
    int size = queue_.size();
    mutex_.unlock();
    
    return size;
}

void TaskQueue::clear() {
    mutex_.lock();
    queue_.clear();
    mutex_.unlock();
}
