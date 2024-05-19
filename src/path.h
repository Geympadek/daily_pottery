#pragma once

#include <string>
#include <fstream>
#include <type_traits>

template <class T>
concept stringType = std::convertible_to<T, std::string>;

namespace assets
{
    class Path
    {
    public:
        template <stringType StringType>
        Path(StringType&& path) noexcept : _raw(std::forward<StringType>(path)) 
        {
            optimize();
        }

        Path removeExtension() const noexcept;
        Path findExtension() const noexcept;
        bool isDirectory() const noexcept
        {
            return !isFile();
        }
        bool isFile() const noexcept;
    public:
        Path operator+(const Path& a) const noexcept
        {
            return Path(_raw + a._raw);
        }
        void operator+=(const Path& a) noexcept
        {
            _raw += a._raw;
            optimize();
        }
        bool operator==(const Path& path) const noexcept
        {
            return _raw == path._raw;
        }
        friend std::ostream& operator<<(std::ostream &os, const Path &dt) noexcept
        {
            os << dt._raw;
            return os;
        }
        operator std::string() const noexcept
        {
            return _raw;
        }
        operator const char*() const noexcept
        {
            return _raw.c_str();
        }
    public:
        const std::string& str() const noexcept {return _raw;}
        const char* c_str() const noexcept {return _raw.c_str();}
    private:
        void optimize() noexcept;
    private:
        std::string _raw;
    };
}