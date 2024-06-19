#include <cmath>

namespace engix
{
    template <class T> requires std::is_arithmetic_v<T>
    constexpr T abs(T x) noexcept
    {
        return x < 0 ? -x : x;
    }
}