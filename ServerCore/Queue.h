#ifndef SERVERCORE_QUEUE_H
#define SERVERCORE_QUEUE_H


#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>

template <class T>
class Queue {
public:
    bool push(const std::unique_ptr<T> item);
    bool pop(std::unique_ptr<T>& item);
    bool empty();
private:
    std::queue<std::unique_ptr<T>>  qu;
    std::mutex                      mutex;
    std::condition_variable         cv;
};


#endif //SERVERCORE_QUEUE_H
