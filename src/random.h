#pragma once

#include <random>
#include <cassert>
#include <concepts>
#include <iostream>

namespace engix
{
    class Random
    {
    public:
        Random() : _engine(), _seed(_engine.default_seed) {}
        Random(uint32_t seed) : _engine(seed), _seed(seed) {}

        void seed(uint32_t seed)
        {
            _engine.seed(seed);
            _seed = seed;
        }
        uint32_t seed() const { return _seed; }

        template <std::integral T>
        T next_in(T min, T max_in) 
        {
            return next_ex(min, max_in + 1);
        }
        template <std::integral T>
        T next_ex(T min, T max_ex)
        {
            assert(min < max_ex);
            auto difference = max_ex - min;
            return min + _engine() % difference;
        }
        //value from 0.0 to 1.0 exclusively
        template <std::floating_point T = long double>
        T next()
        {
            return static_cast<T>(_engine()) / static_cast<T>(_engine.max());
        }
    private:
        std::mt19937 _engine;
        uint32_t _seed;
    };
}