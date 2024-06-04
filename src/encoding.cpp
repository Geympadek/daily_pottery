#include "encoding.h"

using namespace engix;
using namespace engix::encoding;

static constexpr char toLower(char ch)
{
    return (ch >= 65 && ch <= 90) ? ch + 32 : ch;
}

static constexpr bool caseInsensCmp(const char* str1, const char* str2) noexcept
{
    for (size_t i = 0; ; i++)
    {
        if (toLower(str1[i]) != toLower(str2[i]))
            return false;
        if (str1[i] == '\0')
            break;
    }
    return true;
}

Encoding engix::encoding::Format::encodingFromStr(const char *str) noexcept
{
    if (caseInsensCmp(str, "UTF-16LE"))
        return Encoding::UTF16LE;
    if (caseInsensCmp(str, "UTF-16BE"))
        return Encoding::UTF16BE;
    if (caseInsensCmp(str, "UTF-32LE"))
        return Encoding::UTF32LE;
    if (caseInsensCmp(str, "UTF-32BE"))
        return Encoding::UTF32BE;
    return Encoding::UTF8;
}

const char *engix::encoding::Format::strFromEncoding(Encoding encoding) noexcept
{
    switch (encoding)
    {
    case UTF16LE:
        return "UTF-16LE";
    case UTF16BE:
        return "UTF-16BE";
    case UTF32LE:
        return "UTF-32LE";
    case UTF32BE:
        return "UTF-32BE";
    default:
        return "UTF-8";
    }
}