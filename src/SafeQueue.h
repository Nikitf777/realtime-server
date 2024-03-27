// C++ implementation of the above approach
#include <condition_variable>
#include <mutex>
#include <queue>

// Thread-safe queue
template <typename T>
class SafeQueue : public std::queue<T> {
private:
    // Underlying queue 
    //std::queue<T> _queue;

    // mutex for thread synchronization 
    std::mutex _mutex;

    // Condition variable for signaling 
    std::condition_variable _cond;

public:
    // Pushes an element to the queue 
    void safeEnqueue(T item);

    // Pops an element off the queue 
    T safeDequeue();
};

template<typename T>
inline void SafeQueue<T>::safeEnqueue(T item)
{

    // Acquire lock 
    std::unique_lock<std::mutex> lock(_mutex);

    // Add item 
    this->push(item);

    // Notify one thread that 
    // is waiting 
    _cond.notify_one();
}

template<typename T>
inline T SafeQueue<T>::safeDequeue()
{

    // acquire lock 
    std::unique_lock<std::mutex> lock(_mutex);

    // wait until queue is not empty 
    _cond.wait(lock,
        [this]() { return !this->empty(); });

    // retrieve item 
    T item = this->front();
    this->pop();

    // return item 
    return item;
}
