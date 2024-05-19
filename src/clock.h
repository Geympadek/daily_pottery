#pragma once

#include <chrono>

namespace engix
{
    class Clock
    {
    public:
        Clock() noexcept;

        void start() noexcept;
        void restart() noexcept;

        void pause() noexcept;
        void stop() noexcept;

        long double millis() const noexcept;
    public:
        bool isRunning() const noexcept {return _isRunning;}
    protected:
        std::chrono::high_resolution_clock::time_point begin;
        std::chrono::high_resolution_clock::time_point end;
        bool _isRunning;
    };
}