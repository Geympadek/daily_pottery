#pragma once

#include <numbers>
#include "jsonval.h"
#include "rotation.h"

namespace engix
{
    constexpr double DEG_TO_RADIANS = std::numbers::pi_v<float> / 180;
    constexpr double RADIANS_TO_DEG = 180 / std::numbers::pi_v<float>;

    enum class Direction {HORIZONTAL, VERTICAL};
    
    template <arithmetic T>
    class Vector2
    {
    public:
        T x = 0;
        T y = 0;
    public:
        constexpr Vector2() noexcept {}
        template <typename _T, typename __T>
        constexpr Vector2(_T x, __T y) noexcept : x(static_cast<T>(x)), y(static_cast<T>(y))
        {
            
        }
        template <typename _T>
        constexpr Vector2 operator+(const _T& a) const noexcept
        {
            return {x + a, y + a};
        }
        template <typename _T>
        constexpr Vector2 operator+(const Vector2<_T>& a) const noexcept 
        {
            return {x + a.x, y + a.y};
        }
        template <typename _T>
        constexpr void operator+=(const _T& a) noexcept
        { 
            x += a; y += a;
        }
        template <typename _T>
        constexpr void operator+=(const Vector2<_T>& a) noexcept
        { 
            x += a.x; y += a.y;
        }
        template <typename _T>
        constexpr Vector2 operator-(const _T& a) const noexcept
        {
            return {x - a, y - a};
        }
        template <typename _T>
        constexpr Vector2 operator-(const Vector2<_T>& a) const noexcept
        {
            return {x - a.x, y - a.y};
        }
        template <typename _T>
        constexpr void operator-=(const _T& a) noexcept
        { 
            x -= a; y -= a;
        }
        template <typename _T>
        constexpr void operator-=(const Vector2<_T>& a) noexcept
        { 
            x -= a.x; y -= a.y;
        }
        template <typename _T>
        constexpr Vector2 operator*(const _T& a) const noexcept
        {
            return {x * a, y * a};
        }
        template <typename _T>
        constexpr Vector2 operator*(const Vector2<_T>& a) const noexcept 
        {
            return {x * a.x, y * a.y};
        }
        template <typename _T>
        constexpr void operator*=(const Vector2<_T>& a) noexcept
        { 
            x *= a.x; y *= a.y;
        }
        template <typename _T>
        constexpr void operator*=(const _T& a) noexcept
        {
            x = static_cast<T>(x * a); y = static_cast<T>(y * a);
        }
        template <typename _T>
        constexpr Vector2 operator/(const _T& a) const noexcept
        {
            return {x / a, y / a};
        }
        template <typename _T>
        constexpr Vector2 operator/(const Vector2<_T>& a) const noexcept 
        {
            return {x / a.x, y / a.x};
        }
        template <typename _T>
        constexpr void operator/=(const _T& a) noexcept
        {
            x /= a; y /= a;
        }
        template <typename _T>
        constexpr void operator/=(const Vector2<_T>& a) noexcept
        {
            x /= a.x; y /= a.y;
        }
        constexpr Vector2 operator-() const noexcept
        {
            return {-x, -y};
        }

        template <typename _T>
        constexpr bool operator==(const Vector2<_T>& a) noexcept
        {
            return x == a.x && y == a.y;
        }
        template <typename _T>
        constexpr bool operator!=(const Vector2<_T>& a) noexcept
        {
            return x != a.x || y != a.y;
        }

        std::string asString() const noexcept
        {
            std::stringstream ss;
            ss << x << ' ' << y;
            return ss.str();
        }

        template <typename _T>
        constexpr operator Vector2<_T>() const noexcept 
        {
            return {static_cast<_T>(x), static_cast<_T>(y)};
        }
        operator std::string() const noexcept
        {
            return asString();
        }
        template <typename _T>
        constexpr static T dotProduct(Vector2 a, Vector2<_T> b) noexcept
        {
            return (a.x * b.x) + (a.y * b.y);
        }
        template <typename _T>
        static Vector2 rotate(Vector2 point, Vector2<_T> center, Rotation rotation) noexcept
        {
            point -= center;

            double s = std::sin(rotation.rads());
            double c = std::cos(rotation.rads());

            point = {
                point.x * c - point.y * s,
                point.x * s + point.y * c
            };
            point += center;
            return point;
        }
        template <typename _T>
        static T findDistance(const Vector2& a, const Vector2<_T>& b) noexcept
        {
            auto delta = a - b;
            delta *= delta;
            return std::sqrt(delta.x + delta.y);
        }
        //if vector is not zero, returns normalized vector, else zero
        static Vector2 normalize(Vector2 vector) noexcept
        {
            auto length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
            if (length == 0)
            {
                return vector;
            }
            return vector / length;
        }
        T& operator[](Direction direction)
        {
            return direction == Direction::HORIZONTAL ? x : y;
        }
        T operator[](Direction direction) const
        {
            return direction == Direction::HORIZONTAL ? x : y;
        }
        friend std::ostream& operator<<(std::ostream &os, const Vector2 &dt) noexcept
        {
            os << dt.x << ' ' << dt.y;
            return os;
        }
        json::Value toJson() const noexcept
        {
            json::Value json;
            json["x"] = x;
            json["y"] = y;
            return json;
        }
        operator json::Value() const noexcept
        {
            return toJson();
        }
        static Vector2 fromJson(const json::Value& json) noexcept
        {
            using JsonType = json::Value::Type;

            T x = 0, y = 0;
            switch (json.type())
            {
            case JsonType::OBJECT:
                x = json.get("x", 0).as<T>();
                y = json.get("y", 0).as<T>();
                break;
            case JsonType::NUMBER:
                x = y = json.as<T>();
                break;
            case JsonType::ARRAY:
                auto arr = json.as<std::vector<T>>();
                if (arr.size() == 0)
                    break;
                
                x = arr[0];
                if (arr.size() == 1)
                    break;

                y = arr[1];
                break;
            }
            return Vector2<T>(x, y);
        }
    };
    using Vector2i = Vector2<int>;
    using Vector2f = Vector2<float>;
    using Vector2d = Vector2<double>;
    using Vector2s = Vector2<size_t>;
}