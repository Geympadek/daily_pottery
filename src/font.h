#pragma once

#include "texture.h"
#include "jsonval.h"
#include <array>
#include "encoding.h"

namespace engix
{
    class Font : public Resource
    {
        struct CharInfo
        {
            int width = 0;
            Rect clip;
        };
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
    public:
        Vector2i cursorOffset() const {return _cursorOffset;}
    private:
        Texture sheet;
        int _height = 0;

        int cellWidth = 0;
        int cellHeight = 0;

        Vector2i _cursorOffset;
        
        int nextCharShift = 0;

        // std::array<CharInfo, NUMBER_OF_CHARACTERS> chars;
        mutable std::unordered_map<char16_t, CharInfo> chars;
    };
}