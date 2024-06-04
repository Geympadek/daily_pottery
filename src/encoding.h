#pragma once

#include <SDL.h>
#include <string>
#include <cassert>
#include <cstring>
#include <unordered_map>
#include <memory>
#include <type_traits>

using std::strcmp;

namespace engix
{
    namespace encoding
    {
        enum Encoding : uint8_t {LE = 0b0, BE = 0b1, UTF8 = 0b10, UTF16LE = 0b100, UTF16BE = 0b101, UTF32LE = 0b110, UTF32BE = 0b111};
        struct Format
        {
            Encoding from;
            Encoding to;

            constexpr Format(Encoding from = UTF8, Encoding to = UTF8) noexcept : from(from), to(to) {}
            Format(const char* from, const char* to) noexcept : from(encodingFromStr(from)), to(encodingFromStr(to)) {}
            Format(const std::string& from, const std::string& to) noexcept : Format(from.c_str(), to.c_str()) {}

            constexpr bool operator==(const Format& format) const noexcept {return from == format.from && to == format.to;}
        
            static Encoding encodingFromStr(const char* str) noexcept;
            static const char* strFromEncoding(Encoding encoding) noexcept;
        };

        inline constexpr size_t hashFormat(const Format& format)
        {
            return static_cast<size_t>(format.from) | (static_cast<size_t>(format.to) << 8);
        }

        template <class FromChar, class ToChar>
        auto convert(const std::basic_string<FromChar>& from, Format format)
        {
            static std::unordered_map<Format, SDL_iconv_t, decltype(&hashFormat)> descriptors(0, hashFormat);

            if (!descriptors.contains(format))
            {
                descriptors[format] = SDL_iconv_open(Format::strFromEncoding(format.to), Format::strFromEncoding(format.from));
            }
            if (from.size() == 0)
            {
                return std::basic_string<ToChar>();
            }

            size_t inBytes = from.size() * sizeof(FromChar);
            size_t outBytes = std::max(from.size() * sizeof(ToChar), inBytes);

            std::basic_string<ToChar> result(outBytes, 0);
            auto out = reinterpret_cast<char*>(result.data());
            auto in = reinterpret_cast<const char*>(from.data());

            SDL_iconv(descriptors[format], &in, &inBytes, &out, &outBytes);
            result = result.c_str();
            return result;
        }
    }
}