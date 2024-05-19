#include "path.h"

assets::Path assets::Path::removeExtension() const noexcept
{
    if (isDirectory())
    {
        return _raw;
    }
    return _raw.substr(0, _raw.rfind('.'));
}

assets::Path assets::Path::findExtension() const noexcept
{
    if (isDirectory())
    {
        return "";
    }
    return _raw.substr(_raw.rfind('.'));
}

bool assets::Path::isFile() const noexcept
{
    auto directoryStart = _raw.rfind('/');
    directoryStart = ((directoryStart == std::string::npos) ? 0 : directoryStart + 1);

    auto extensionStart = _raw.rfind('.');
    return extensionStart != std::string::npos && directoryStart < extensionStart;
}

void assets::Path::optimize() noexcept
{
    if (_raw.size() == 0)
    {
        return;
    }
    //skip the first one to keep "./" or "../" at the start
    size_t pos = (_raw[0] == '.') ? 2 : 1;
    while (true)
    {
        pos = _raw.find("./", pos);
        if (pos == std::string::npos)
        {
            break;
        }
        if (_raw[pos - 1] == '.')//case of parent folder
        {
            pos--;//move iterator to the start of ../
            if (pos - 1 == 0)//check if there's any space before /../
            {
                pos += 3;
                continue;
            }
            size_t parentFolderPos = _raw.rfind('/', pos - 2);
            if (parentFolderPos == std::string::npos)
            {
                parentFolderPos = 0;
            }
            _raw.erase(parentFolderPos, pos - parentFolderPos + 2);
            pos = parentFolderPos;
        }
        else // case of current folder 
        {
            _raw.erase(pos, 2);
        }
    }
}