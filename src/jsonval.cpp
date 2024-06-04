#include "jsonval.h"

json::Value json::Value::loadFromFile(const assets::Path &path)
{
    std::ifstream file(path.str());
    std::stringstream ss;
    ss << file.rdbuf();
    file.close();
    return loadFromText(ss.str());
}

json::Value json::Value::loadFromText(std::string text)
{
    Value newVal;
    newVal._value = std::move(text);
    newVal.optimize();
    newVal.updateValues();
    return newVal;
}

void json::Value::saveAsFile(const assets::Path &path, const Value& val)
{
    std::ofstream file(path.str());
    file << toString(val);
    file.close();
}

static size_t findStrEnd(size_t strStart, const std::string& text)
{
    auto textSize = text.size();
    for (size_t i = strStart + 1; i < textSize; i++)
    {
        if (text[i] != '\"')
            continue;

        auto slashCount = 0;
        for (size_t j = i - 1; j >= 0; j--)
        {
            if (text[j] == '\\')
                slashCount++;
            else
                break;
        }
        if (slashCount ^ 1)//odd
            return i;
    }
    return textSize;
}

static size_t findBlockEnd(const char startChar, const char endChar, size_t start, const std::string& text)
{
    auto textSize = text.size();
    for (size_t i = start, nesting = 0; i < textSize; i++)
    {
        char currentCh = text[i];
        char prevCh = text[i - 1];
        if (currentCh == '\"')
        {
            i = findStrEnd(i, text);
            continue;
        }
        if (currentCh == startChar)
            nesting++;
        else if (currentCh == endChar)
            nesting--;
        
        if (nesting == 0)
            return i;
    }
    return textSize;
}

static size_t findObjEnd(size_t objectStart, const std::string& text)
{
    return findBlockEnd('{', '}', objectStart, text);
}

static size_t findArrayEnd(size_t arrayStart, const std::string& text)
{
    return findBlockEnd('[', ']', arrayStart, text);
}

static size_t findPropertyEnd(size_t propertyStart, const std::string& text)
{
    auto textSize = text.size();
    for (size_t i = propertyStart; i < textSize; i++)
    {
        char currentCh = text[i];
        if (currentCh == ',' || currentCh == '}' || currentCh == ']')
            return i - 1;
    }
    return textSize;
}

size_t json::Value::findEnd(size_t start, const std::string &text)
{
    if (text[start] == '{')
        return findObjEnd(start, text);
    if (text[start] == '[')
        return findArrayEnd(start, text);
    if (text[start] == '\"')
        return findStrEnd(start, text);
    return findPropertyEnd(start, text);
}

json::Value::Value(const Value &value)
{
    _value = value._value;
    _values = value._values;
    _type = value._type;
}

json::Value::Value(Value &&value)
{
    _value = std::move(value._value);
    _values = std::move(value._values);
    _type = value._type;
}

void json::Value::operator=(const Value &value)
{
    _value = value._value;
    _values = value._values;
    _type = value._type;
}

void json::Value::operator=(Value &&value)
{
    _value = std::move(value._value);
    _values = std::move(value._values);
    _type = value._type;
}

bool json::Value::operator==(const json::Value &val) const
{
    if (_type == Type::OBJECT)
        return _values == val._values;
    return _value == _value;
}

void json::Value::optimize()
{
    for (size_t i = 0; i < _value.size(); i++)
    {
        auto curChar = _value[i];
        if (curChar == ' ' || curChar == '\n' || curChar == '\r' || curChar == '\t' || curChar == '\0')
            _value.erase(i--, 1);
        else if (curChar == '\"')
            i = findStrEnd(i, _value);
    }
}

void json::Value::refactor(std::string &str)
{
    if (str.size() == 0)
        return;
    
    char prevChar = '\0';
    for (size_t i = 0; i < str.size(); i++)
    {
        if (prevChar == '\\')
        {
            str.erase(i - 1, 1);
            auto& currentChar = str[i - 1];
            switch (currentChar)
            {
            case 'n':
                currentChar = '\n';
                break;
            case 't':
                currentChar = '\t';
                break;
            case 'v':
                currentChar = '\v';
                break;
            case 'b':
                currentChar = '\b';
                break;
            case 'r':
                currentChar = '\r';
                break;
            case '0':
                currentChar = '\0';
                break;
            case 'f':
                currentChar = '\f';
                break;
            case 'a':
                currentChar = '\a';
                break;
            }
        }
        prevChar = str[i];
    }
}

void json::Value::updateValues()
{
    updateType();
    if (_type != Type::OBJECT)
        return;
    
    auto textSize = _value.size();
    size_t i = 1;
    while (i < textSize)
    {
        auto keyEnd = findStrEnd(i, _value);
        Key key = _value.substr(i + 1, keyEnd - i - 1);
        auto valStart = keyEnd + 2;
        auto valEnd = findEnd(valStart, _value);
        auto value = fromString<Value>(_value.substr(valStart, valEnd - valStart + 1));
        _values.insert(std::pair(std::move(key), std::move(value)));
        i = valEnd + 2;
    }
}

void json::Value::updateType()
{
    if (_value == "")
        _type = Type::NULL_VAL;
    else if (_value[0] == '{')
        _type = Type::OBJECT;
    else if (_value[0] == '[')
        _type = Type::ARRAY;
    else if (_value[0] == '\"')
        _type = Type::STRING;
    else if (_value == "true" || _value == "false")
        _type = Type::BOOLEAN;
    else
        _type = Type::NUMBER;
}