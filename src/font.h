#pragma once

#include "texture.h"
#include "jsonval.h"
#include <array>
#include "encoding.h"

namespace engix
{
    class Font : public Resource
    {
    public:
        struct CharInfo
        {
            int width = 0;
            Rect clip;
        };
        using Characters = std::unordered_map<char16_t, CharInfo>;
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
        size_t findClosest(const std::u16string& text, Vector2i point) const;

        void render(const std::u16string& text, Vector2i position, Color color = Color::WHITE) const
        {
            render(text.begin(), text.end(), position, color);
        }
        void render(const char16_t* text, Vector2i position, Color color = Color::WHITE) const
        {
            auto len = std::char_traits<char16_t>::length(text);
            render(text, text + len, position, color);
        }
        void render(const std::u16string& text, size_t number_of_chars, Vector2i position, Color color = Color::WHITE) const
        {
            render(text.begin(), text.begin() + number_of_chars, position, color);
        }
        void render(const char16_t* text, size_t number_of_chars, Vector2i position, Color color = Color::WHITE) const
        {
            render(text, text + number_of_chars, position, color);
        }
        template <class Iterator>
        void render(const Iterator begin, const Iterator end, Vector2i position, Color color = Color::WHITE) const
        {
            if (!_isLoaded)
                return;

            const_cast<Font*>(this)->sheet.setColor(color);

            auto cursor = position;

            for (auto it = begin; it != end; it++)
            {
                char16_t c = *it;
                if (c == u'\0')
                {
                    continue;
                }
                if (c == u'\n')
                {
                    cursor.y += _height;
                    cursor.x = position.x;
                    continue;
                }
                CharInfo charInfo = _chars[c];
                sheet.render(cursor, charInfo.clip);
                cursor.x += charInfo.width + _nextCharShift;
            }
        }
    public:
        Vector2i cursorOffset() const {return _cursorOffset;}

        int height() const {return _height;}
        int nextCharShift() const {return _nextCharShift;}        

        const Characters& chars() const {return _chars;}
        Characters& chars() {return _chars;}
    private:
        Texture sheet;
        int _height = 0;

        int cellWidth = 0;
        int cellHeight = 0;

        Vector2i _cursorOffset;
        
        int _nextCharShift = 0;

        mutable Characters _chars;
    };
}