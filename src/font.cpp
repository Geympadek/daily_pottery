#include "font.h"

using namespace engix;

static int getCharWidth(const engix::Texture& texture, engix::Rect clip)
{
    const auto& pixels = texture.pixels();
    int maxX = clip.start.x;
    for (int y = clip.start.y; y < clip.start.y + clip.height; y++)
    {
        for (int x = clip.start.x + 1; x < clip.start.x + clip.width; x++)
        {
            if (pixels[x + y * texture.width()].alpha != 0 && maxX < x)
                maxX = x;
        }
    }
    return maxX - clip.start.x;
}

Font engix::Font::load(const assets::Path &path)
{
    Font font;
    font.sheet = Texture::load(path + ".png");
    auto json = json::Value::loadFromFile(path + ".json");

    auto start = Vector2i::fromJson(json["start"]);

    encoding::Format format("utf8", json["format"].as<std::string>().c_str());
    
    auto utf8charList = json["list"].as<std::string>();
    auto charList = encoding::convert<char, char16_t>(utf8charList, format);

    font.cellWidth = json["cell_width"].as<int>();
    font.cellHeight = json["cell_height"].as<int>();
    auto cellShift = Vector2i::fromJson(json["cell_shift"]);

    font._height = json["height"].as<int>();

    font._cursorOffset = Vector2i::fromJson(json["cursor_offset"]);

    font._nextCharShift = json["next_char_shift"].as<int>();

    int row = 0, column = 0;
    for (auto ch : charList)
    {
        if (ch == u'\n')
        {
            column = 0;
            row++;
            continue;
        }

        auto& currentChar = font._chars[ch];
        currentChar.clip.start = start + Vector2i(column * (font.cellWidth + cellShift.x), row * (font.cellHeight + cellShift.y));
        currentChar.clip.width = font.cellWidth;
        currentChar.clip.height = font.cellHeight;
        currentChar.width = getCharWidth(font.sheet, currentChar.clip);

        column++;
    }

    auto charsJson = json["_chars"].as<std::vector<json::Value>>();
    for (const auto& charJson : charsJson)
    {
        auto utf8 = charJson["char"].as<std::string>();
        auto result = encoding::convert<char, char16_t>(utf8, format);
        auto& charInfo = font._chars[result[0]];
        charInfo.width = charJson["width"].as<int>();
    }
    font._isLoaded = true;
    return font;
}

void engix::Font::free() noexcept
{
    if (!_isLoaded)
        return;
    _isLoaded = false;
    
    sheet.free();
}

Texture engix::Font::createTexture(const std::u16string &text, Color color) const
{
    auto size = findSize(text);

    auto width = size.x;
    auto height = size.y;
    PixelImage pixelImage(width, height);

    Vector2i cursor;
    for (auto c : text)
    {
        if (c == u'\0')
        {
            continue;
        }
        if (c == u'\n')
        {
            cursor.y += static_cast<int>(height);
            cursor.x = 0;
            continue;
        }
        CharInfo charInfo = _chars[c];
        
        pixelImage.draw(sheet.pixelImage(charInfo.clip), cursor);
        cursor.x += static_cast<int>(charInfo.width) + _nextCharShift;
    }
    return Texture(pixelImage);
}

Vector2i engix::Font::findSize(const std::u16string &text) const
{
    int height = cellHeight;
    int width = 1;
    for (auto c : text)
    {
        if (c == u'\0')
        {
            continue;
        }
        if (c == u'\n')
        {
            height += cellHeight;
            continue;
        }
        width +=  _chars[c].width + _nextCharShift;
    }
    return {width, height};
}

size_t engix::Font::findClosest(const std::u16string &text, Vector2i point) const
{
    int width = 0;
    int height = 0;

    for (size_t i = 0; i < text.size(); i++)
    {
        auto c = text[i];
        if (c == u'\0')
        {
            continue;
        }
        if (c == u'\n')
        {
            height += cellHeight;
            continue;
        }
        auto info = _chars[c];
        Rect rect(width, height, info.width + _nextCharShift, cellHeight);

        if (rect.isAbove(point))
        {
            return i;
        }

        width += info.width + _nextCharShift;
    }
    return text.size();
}

void engix::Font::render(const std::u16string &text, Vector2i position, Color color) const
{
    if (!_isLoaded)
        return;

    const_cast<Font*>(this)->sheet.setColor(color);

    auto cursor = position;

    for (auto c : text)
    {
        if (c == u'\0')
        {
            continue;
        }
        if (c == u'\n')
        {
            cursor.y += static_cast<int>(_height);
            cursor.x = position.x;
            continue;
        }
        CharInfo charInfo = _chars[c];
        sheet.render(cursor, charInfo.clip);
        cursor.x += static_cast<int>(charInfo.width) + _nextCharShift;
    }
}