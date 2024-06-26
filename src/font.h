#pragma once

#include "texture.h"
#include "jsonval.h"
#include <array>
#include "encoding.h"
#include <functional>
#include <set>
#include "substr.h"

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

        Texture createTexture(Substr<char16_t> text, Color color = Color::WHITE) const;

        Vector2i findSize(Substr<char16_t> text) const;
        size_t findClosest(Substr<char16_t> text, Vector2i point) const;

        void map(Substr<char16_t> text, const std::function<void(Vector2i, char16_t, size_t, const CharInfo&)>& handle) const;

        void render(Substr<char16_t> text, Vector2i position, Color color = Color::WHITE) const;
        void render(Substr<char16_t> text, Vector2i position, Color color, const std::set<size_t>& alternativeChars, Color altColor) const;
    public:
        Vector2i cursorOffset() const {return _cursorOffset;}

        int height() const {return _height;}
        int nextCharShift() const {return _nextCharShift;}        

        int selectedLeft() const {return _selectedLeft;}
        int selectedRight() const {return _selectedRight;}
        int selectedHeight() const {return _selectedHeight;}
        Vector2i selectionOffset() const {return _selectionOffset;}

        const Characters& chars() const {return _chars;}
        Characters& chars() {return _chars;}
    private:
        Texture sheet;
        int _height = 0;

        int cellWidth = 0;
        int cellHeight = 0;

        Vector2i _cursorOffset;
        
        int _nextCharShift = 0;

        int _selectedLeft = 0;
        int _selectedRight = 0;
        int _selectedHeight = 0;
        Vector2i _selectionOffset;

        mutable Characters _chars;
    };
}