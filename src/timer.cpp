#include "timer.h"

engix::Timer::Timer() noexcept : Clock(), delay(0)
{
}

engix::Timer::Timer(long double delay) noexcept : Clock(), delay(delay)
{
}

void engix::Timer::update() noexcept
{
    if (millis() > delay)
    {
        stop();
    }
}