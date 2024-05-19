#pragma once

#include <cstdint>
#include <numbers>

namespace engix
{
    class Rotation
    {
    public:
        static constexpr inline Rotation fromDegs(double degs) noexcept;
        static constexpr inline Rotation fromRads(double rads) noexcept;

        inline constexpr Rotation() noexcept;

        inline constexpr double degs() const noexcept;
        inline constexpr double rads() const noexcept;

        inline constexpr void setDegs(double degs) noexcept;
        inline constexpr void setRads(double rads) noexcept;
    private:
        static constexpr double PI = std::numbers::pi_v<double>;
        static constexpr double DEG_TO_RADS = Rotation::PI / 180.0;
        static constexpr double RAD_TO_DEGS = 180.0 / Rotation::PI;
    private:
        enum class Flag {NONE, UPDATE_DEGS = 1, UPDATE_RADS = 2};
        double _degs;
        double _rads;
        Flag flag;
    };

    constexpr Rotation Rotation::fromDegs(double degs) noexcept
    {
        Rotation newAngle;
        newAngle.setDegs(degs);
        return newAngle;
    }

    constexpr Rotation Rotation::fromRads(double rads) noexcept
    {
        Rotation newAngle;
        newAngle.setRads(rads);
        return newAngle;
    }

    constexpr engix::Rotation::Rotation() noexcept : _degs(0.0), _rads(0.0), flag(Flag::NONE)
    {
        
    }

    constexpr double Rotation::degs() const noexcept
    {
        if (flag == Flag::UPDATE_DEGS)
        {
            Rotation* angle = const_cast<Rotation*>(this);
            angle->_degs = _rads * RAD_TO_DEGS;
            angle->flag = Flag::NONE;
        }
        return _degs;
    }

    constexpr double Rotation::rads() const noexcept
    {
        if (flag == Flag::UPDATE_RADS)
        {
            Rotation* angle = const_cast<Rotation*>(this);
            angle->_rads = _degs * DEG_TO_RADS;
            angle->flag = Flag::NONE;
        }
        return _rads;
    }

    constexpr void Rotation::setDegs(double degs) noexcept
    {
        _degs = degs;
        flag = Flag::UPDATE_RADS;
    }

    constexpr void Rotation::setRads(double rads) noexcept
    {
        _rads = rads;
        flag = Flag::UPDATE_DEGS;
    }
}