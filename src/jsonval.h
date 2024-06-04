#pragma once

#include <sstream>
#include <fstream>
#include <memory>
#include <iostream>
#include <map>
#include <vector>
#include <type_traits>
#include <cassert>

#include "path.h"

#define DEBUG

template <class T>
concept container = requires(T t) {
    std::begin(t);
    std::end(t);
};
template <class T>
concept arithmetic = std::is_arithmetic_v<T>;

namespace json
{
    class Value
    {
        //Types
        struct Key
        {
            Key(std::string str) : str(std::move(str)) {}
            Key(const char* c_str) : str(c_str) {}

            Key(Key&& key) : str(std::move(key.str)) {}
            Key(const Key& key) : str(key.str) {}

            bool operator<(const Key& key) const {return str < key.str;}

            bool operator==(const Key& key) const {return str == key.str;}

            std::string str;
        };
    public:
        enum class Type {NULL_VAL, BOOLEAN, NUMBER, OBJECT, ARRAY, STRING};

        Value() : _type(Type::NULL_VAL) {}

        template <class T>
        Value(const T& value);

        Value(const Value& value);
        Value(Value& value) : Value(static_cast<const Value&>(value)) {}
        Value(Value&& value);

        static Value loadFromFile(const assets::Path& path);
        static Value loadFromText(std::string text);

        static void saveAsFile(const assets::Path& path, const Value& val);

        bool isMember(const Key& key) const {return const_cast<Value*>(this)->_values[key]._type != Type::NULL_VAL;}

        template <class T>
        T as() const;

        const std::string& str() const {return _value;}
        Type type() const {return _type;}
        
        void operator=(const Value& value); 
        void operator=(Value&& value); 
    
        const Value& operator[](const Key& key) const {return const_cast<Value*>(this)->_values[key];}
        Value& operator[](const Key& key) 
        {
            _type = Type::OBJECT; 
            return _values[key];
        }
        Value get(const Key& key, Value value) const
        {
            return isMember(key) ? operator[](key) : std::move(value);
        }
        bool operator==(const Value& val) const;
    private://member functions
        void optimize();
        static void refactor(std::string& str);
        void updateValues();
        void updateType();

        template <class T>
        static std::string toString(const T& val) = delete;
        template <container T>
        static std::string toString(const T& container);
        template <arithmetic T>
        static std::string toString(const T& val);
        template <size_t N>
        static std::string toString(const char (&str)[N]);

        template <class T>
        static T fromString(const std::string& str) = delete;
        template <container T>
        static T fromString(const std::string& str);
        template <arithmetic T>
        static T fromString(const std::string& str);
        
        static size_t findEnd(size_t start, const std::string& text);
    private://members
        std::string _value;
        Type _type;
        
        std::map<Key, Value> _values;
    };
    
    template <class T>
    inline Value::Value(const T &value)
    {
        _value = toString(value);
        updateValues();
    }

    template <class T>
    inline T Value::as() const
    {
        return fromString<T>(_value);
    }

    template <container T>
    inline std::string Value::toString(const T &val)
    {
        std::string result = "[";
        for (const auto& v : val)
            result += toString(v) + ',';
        result.back() = ']';
        return result;
    }
    template <>
    inline std::string Value::toString<std::string>(const std::string &val)
    {
        return '\"'+val+'\"';
    }
    template <size_t N>
    inline std::string Value::toString(const char (&str)[N])
    {
        return '\"'+std::string(str)+'\"';
    }
    template <>
    inline std::string Value::toString<Value>(const Value &val)
    {
        if (val._type != Type::OBJECT)
            return val._value;
        
        std::string result("{");
        for (const auto& value : val._values)
        {
            if (value.second._type == Type::NULL_VAL)
                continue;
                
            result += toString<std::string>(value.first.str) + ':' + toString<Value>(value.second) + ','; 
        }
        result[result.size() - 1] = '}';
        return result;
    }
    template <arithmetic T>
    inline std::string Value::toString(const T &val)
    {
        std::stringstream ss;
        ss << val;
        return ss.str();
    }
    template <>
    inline std::string Value::toString<bool>(const bool& val)
    {
        return val ? "true" : "false";
    }
    typedef const char* c_str;
    template <>
    inline std::string Value::toString<c_str>(const c_str& val)
    {
        return toString<std::string>(val);
    }

    template <>
    inline Value Value::fromString<Value>(const std::string& str)
    {
        Value newVal;
        newVal._value = str;
        newVal.updateValues();
        return newVal;
    }
    template <arithmetic T>
    inline T Value::fromString(const std::string& str)
    {
        if (str.size() == 0)
            return 0;
        
        std::istringstream ss(str);
        T t;
        ss >> t;
        return t;
    }
    template <>
    inline bool Value::fromString<bool>(const std::string& str)
    {
        return str.size() > 0 && str != "false" && str != "0";
    }
    template <container T>
    inline T Value::fromString(const std::string& str)
    {
        T array;
        auto textSize = str.size();
        if (textSize == 0)
            return array;
        
        textSize--;
        size_t i = 1;
        while (i < textSize)
        {
            auto valEnd = findEnd(i, str);
            const auto& valStr = str.substr(i, valEnd - i + 1);
            auto value = fromString<typename T::value_type>(valStr);
            array.push_back(value);
            i = valEnd + 2;
        }
        return array;
    }
    template <>
    inline std::string Value::fromString<std::string>(const std::string& str)
    {
        auto result = str.size() > 0 ? str.substr(1, str.size() - 2) : "";
        refactor(result);
        return result;
    }
}