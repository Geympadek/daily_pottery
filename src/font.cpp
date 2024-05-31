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

    font.cellWidth = json["cell_width"].as<int>();
    font.cellHeight = json["cell_height"].as<int>();

    font._height = json["height"].as<int>();

    int row = 0, column = 0;
    int i = 0;
    auto it = CHAR_LIST;
    for (; *it != u'\0'; it++)
    {
        if (*it == u'\n')
        {
            column = 0;
            row++;
            continue;
        }

        font.chars[i].character = *it;
        Rect clip;
        clip.start = {column * (font.cellWidth + 1) + 1, row * (font.cellHeight + 1) + 1};
        clip.width = font.cellWidth;
        clip.height = font.cellHeight;

        font.chars[i].width = getCharWidth(font.sheet, clip);
        font.chars[i].clip = clip;
        i++;
        column++;
    }

    auto charsJson = json["chars"].as<std::vector<json::Value>>();
    for (const auto& charJson : charsJson)
    {
        auto c = static_cast<char16_t>(charJson["char"].as<int>());
        auto& charInfo = font.getCharInfo(c);
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
        CharInfo charInfo = getCharInfo(c);
        
        pixelImage.draw(sheet.pixelImage(charInfo.clip), cursor);
        cursor.x += static_cast<int>(charInfo.width) + 1;
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
        CharInfo charInfo = getCharInfo(c);
        width += charInfo.width + 1;
    }
    return {width, height};
}

void engix::Font::render(const std::u16string &text, Vector2i position, Color color) const
{
    if (!_isLoaded)
        return;

    const_cast<Font*>(this)->sheet.setColor(color);

    auto cursor = position;

    for (auto c : text)
    {
        if (c == u'\n')
        {
            cursor.y += static_cast<int>(_height);
            cursor.x = position.x;
            continue;
        }
        CharInfo charInfo = getCharInfo(c);
        sheet.render(cursor, charInfo.clip);
        cursor.x += static_cast<int>(charInfo.width) + 1;
    }
}

static unsigned countDigits(unsigned number)
{
    unsigned digits = 1, pten = 10;
    while (pten <= number)
    {
        digits++;
        pten *= 10;
    }
    return digits;
}

void engix::Font::render(unsigned number, unsigned numberOfDigits, Vector2i position, Color color) const
{
    const_cast<Font*>(this)->sheet.setColor(color);

    for (unsigned i = 0; i < numberOfDigits; i++)
    {
        unsigned short digit = number / static_cast<unsigned>(std::pow(10u, numberOfDigits - (i + 1))) % 10u;
        auto charInfo = getCharInfo(static_cast<char16_t>(48 + digit));//48 is decimal value of char u'0'
        sheet.render(position, charInfo.clip);
        position.x += static_cast<int>(charInfo.width) + 1;
    }
}

void engix::Font::render(unsigned number, Vector2i position, Color color) const
{
    render(number, countDigits(number), position, color);
}

engix::Font::CharInfo& engix::Font::getCharInfo(char16_t c)
{
    for (auto& it : chars)
    {
        if (c == it.character)
        {
            return it;
        }
    }
    return chars.back();
}