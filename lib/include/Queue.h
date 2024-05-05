#ifndef SERVERCORE_QUEUE_H
#define SERVERCORE_QUEUE_H


#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>

template <class T>
class Queue {
public:
    bool push(std::unique_ptr<T> item) {
        std::unique_lock<std::mutex> ul(mutex);

        qu.push(std::move(item));
        cv.notify_one();
        return true;
    }
    bool pop(std::unique_ptr<T>& item) {
        std::unique_lock<std::mutex> ul(mutex);

        cv.wait(ul, [this]() { return !qu.empty(); });
        item = std::move(qu.front());
        qu.pop();
        return true;
    }
    bool empty() {
        return qu.empty();
    }
private:
    std::queue<std::unique_ptr<T>>  qu;
    std::mutex                      mutex;
    std::condition_variable         cv;
};


#endif //SERVERCORE_QUEUE_H
