#pragma once

#include "clock.h"

namespace engix
{
    class Timer : public Clock
    {
    public:
        Timer() noexcept;
        Timer(long double delay) noexcept;

        void update() noexcept;
    private:
        long double delay;
    };
}