#pragma once
#include <list>
#include <condition_variable>
#include <mutex>

template<typename T>
class SafeList : public std::list<T>
{
private:

    std::mutex _mutex;

    std::condition_variable _cond;

public:
    void safePushBack(T item);
    T& operator[](size_t index);
};

template<typename T>
inline void SafeList<T>::safePushBack(T item)
{

    // Acquire lock 
    std::unique_lock<std::mutex> lock(_mutex);

    // Add item 
    this->push_back(item);

    // Notify one thread that 
    // is waiting 
    _cond.notify_one();
}

template<typename T>
inline T& SafeList<T>::operator[](size_t index)
{
    auto i = this->begin();
    std::advance(i, index);
    return *i;
}
