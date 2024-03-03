// C++ implementation of the above approach
#include <condition_variable>
#include <mutex>
#include <queue>

// Thread-safe queue
template <typename T>
class SafeQueue {
private:
    // Underlying queue 
    std::queue<T> _queue;

    // mutex for thread synchronization 
    std::mutex _mutex;

    // Condition variable for signaling 
    std::condition_variable _cond;

public:
    // Pushes an element to the queue 
    void enqueue(T item);

    // Pops an element off the queue 
    T dequeue();

    size_t size();
};

template<typename T>
inline void SafeQueue<T>::enqueue(T item)
{

    // Acquire lock 
    std::unique_lock<std::mutex> lock(_mutex);

    // Add item 
    _queue.push(item);

    // Notify one thread that 
    // is waiting 
    _cond.notify_one();
}

template<typename T>
inline T SafeQueue<T>::dequeue()
{

    // acquire lock 
    std::unique_lock<std::mutex> lock(_mutex);

    // wait until queue is not empty 
    _cond.wait(lock,
        [this]() { return !_queue.empty(); });

    // retrieve item 
    T item = _queue.front();
    _queue.pop();

    // return item 
    return item;
}

template<typename T>
inline size_t SafeQueue<T>::size()
{
    return _queue.size();
}
