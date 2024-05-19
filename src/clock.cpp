#include "clock.h"

engix::Clock::Clock() noexcept : _isRunning(false)
{
    restart();
}

void engix::Clock::start() noexcept
{
    if (!_isRunning)
    {
        restart();
    }
}

void engix::Clock::restart() noexcept
{
    begin = std::chrono::high_resolution_clock::now();
    _isRunning = true;
}

void engix::Clock::pause() noexcept
{
    end = std::chrono::high_resolution_clock::now();
    _isRunning = false;
}

void engix::Clock::stop() noexcept
{
    begin = {};
    end = {};
    _isRunning = false;
}

long double engix::Clock::millis() const noexcept
{
    std::chrono::high_resolution_clock::time_point end;
    if (_isRunning)
    {
        end = std::chrono::high_resolution_clock::now();
    }
    else
    {
        end = this->end;
    }
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    return duration * 0.001;
}
