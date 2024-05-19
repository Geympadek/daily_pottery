#pragma once

#include "rect.h"

namespace engix
{
    struct Box
    {
        Rect left, up, right, down;
        Rect luCorner, ruCorner, ldCorner, rdCorner;
        Rect fill;
    };
}