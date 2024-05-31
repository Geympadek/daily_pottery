#include "boxTemplate.h"

using namespace engix;

BoxTemplate BoxTemplate::load(const assets::Path &path)
{
    BoxTemplate result;
    result._isLoaded = true;

    Texture sheet(path + ".png");
    auto json = json::Value::loadFromFile(path + ".json");

    result.left = sheet.pixelImage(Rect::fromJson(json["left"]));
    result.right = sheet.pixelImage(Rect::fromJson(json["right"]));
    result.up = sheet.pixelImage(Rect::fromJson(json["up"]));
    result.down = sheet.pixelImage(Rect::fromJson(json["down"]));

    auto leftUp = Rect::fromJson(json["leftUp"]);
    leftUp.width = result.left.width;
    leftUp.height = result.up.height;
    auto rightUp = Rect::fromJson(json["rightUp"]);
    rightUp.width = result.right.width;
    rightUp.height = result.up.height;
    auto leftDown = Rect::fromJson(json["leftDown"]);
    leftDown.width = result.left.width;
    leftDown.height = result.down.height;
    auto rightDown = Rect::fromJson(json["rightDown"]);
    rightDown.width = result.right.width;
    rightDown.height = result.down.height;

    result.leftUp = sheet.pixelImage(leftUp);
    result.rightUp = sheet.pixelImage(rightUp);
    result.leftDown = sheet.pixelImage(leftDown);
    result.rightDown = sheet.pixelImage(rightDown);

    result.fill = sheet.pixelImage(Rect::fromJson(json["fill"]));
    return result;
}

Texture engix::BoxTemplate::genTexture(int width, int height) const
{
    return Texture::load(width, height, left, up, right, down, leftUp, rightUp, leftDown, rightDown, fill);
}
