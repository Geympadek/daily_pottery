#pragma once

#include "texture.h"
#include "jsonval.h"
#include <array>

namespace engix
{
    class Font : public Resource
    {
        struct CharInfo
        {
            char16_t character = L'\0';
            int width = 0;

            Rect clip;
        };
    public:
        static const int NUMBER_OF_CHARACTERS = 160;
        static const char16_t* CHAR_LIST;
    public:
        Font() {}
        template <class... Args>
        Font(Args&&... params) : Font(load(std::forward<Args>(params)...)) {}

        Font(const Font& font) = default;
        Font(Font& font) : Font(static_cast<const Font&>(font)) {}

        Font(Font&& font) = default;

        Font& operator=(const Font& font) = default;
        Font& operator=(Font&& font) = default;

        static Font load(const assets::Path& path);
        void free() noexcept;

        Texture createTexture(const std::u16string& text, Color color = Color::WHITE) const;

        Vector2i findSize(const std::u16string& text) const;

        void render(const std::u16string& text, Vector2i position, Color color = Color::WHITE) const;
        void render(unsigned number, unsigned numberOfDigits, Vector2i position, Color color = Color::WHITE) const;
        void render(unsigned number, Vector2i position, Color color = Color::WHITE) const;
    private:
        CharInfo &getCharInfo(char16_t c);
        const CharInfo& getCharInfo(char16_t c) const { return const_cast<Font*>(this)->getCharInfo(c); }
    private:
        Texture sheet;
        int _height = 0;

        int cellHeight = 0;
        int cellWidth = 0;

        std::array<CharInfo, NUMBER_OF_CHARACTERS> chars;
    };
    inline const char16_t* Font::CHAR_LIST = 
        u"АаБбВвГгДдЕеЁёЖжЗзИиЙй\n"
        u"КкЛлМмНнОоПпРрСсТтУуФф\n"
        u"ХхЦцЧчШшЩщЪъЫыЬьЭэЮюЯя\n"
        u"AaBbCcDdEeFfGgHhIiJjKk\n"
        u"LlMmNnOoPpQqRrSsTtUuVv\n"
        u"WwXxYyZz0123456789~!@#\n"
        u"$%^&*()-_=+{}[]|\\/><\'\"\n"
        u";:,.? ";
}