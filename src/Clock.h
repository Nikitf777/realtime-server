#pragma once
#include <chrono>
#include <ctime>
#include <cmath>

class Clock
{
public:
    using Microseconds = std::chrono::microseconds;
    using Milliseconds = std::chrono::milliseconds;
    using Seconds = std::chrono::seconds;

    void start()
    {
        _startTime = std::chrono::steady_clock::now();
        _isRunning = true;
    }

    void stop()
    {
        _endTime = std::chrono::steady_clock::now();
        _isRunning = false;
    }

    template<typename TTime>
    double getElapsedTime()
    {
        std::chrono::time_point<std::chrono::steady_clock> endTime;

        if (_isRunning)
        {
            endTime = std::chrono::steady_clock::now();
        }
        else
        {
            endTime = _endTime;
        }

        return std::chrono::duration_cast<TTime>(endTime - _startTime).count();
    }

    double elapsedMilliseconds()
    {
        std::chrono::time_point<std::chrono::steady_clock> endTime;

        if (_isRunning)
        {
            endTime = std::chrono::steady_clock::now();
        }
        else
        {
            endTime = _endTime;
        }

        return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - _startTime).count();
    }

    double elapsedSeconds()
    {
        return elapsedMilliseconds() / 1000.0;
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> _startTime;
    std::chrono::time_point<std::chrono::steady_clock> _endTime;
    bool                                               _isRunning = false;
};

