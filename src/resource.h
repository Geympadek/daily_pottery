#pragma once

#include "path.h"

namespace engix
{
    class Resource
    {
    public:
        bool isLoaded() const noexcept {return _isLoaded;}
    protected:
        bool _isLoaded = false;
    };
}