#include "Queue.h"

template <class T>
bool Queue<T>::push(const std::unique_ptr<T> item) {
    std::unique_lock<std::mutex> ul(mutex);

    qu.push(std::move(item));
    cv.notify_one();
    return true;
}

template <class T>
bool Queue<T>::pop(std::unique_ptr<T>& item) {
    std::unique_lock<std::mutex> ul(mutex);

    cv.wait(ul, [this]() { return !qu.empty(); });
    item = std::move(qu.front());
    qu.pop();
    return true;
}

template <class T>
bool Queue<T>::empty() {
    return qu.empty();
}