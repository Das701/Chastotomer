#include "defines.h"
#include "Display/Primitives.h"
#include "Display/Font/Font.h"
#include "Utils/Buffer.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>


const char * const String::_ERROR = "---.---";


String::String() : buffer(nullptr)
{
    Set("");
}


String::String(const String &rhs) : buffer(nullptr)
{
    Set("");

    Allocate(static_cast<int>(std::strlen(rhs.c_str()) + 1));

    if(buffer != nullptr)
    {
        std::strcpy(buffer, rhs.c_str());
    }
}


String::String(char symbol) : buffer(nullptr)
{
    Set("");

    Allocate(2);

    if(buffer != nullptr)
    {
        buffer[0] = symbol;
        buffer[1] = 0;
    }
}


String::String(pCHAR format, ...) : buffer(nullptr)
{
    Free();

    std::va_list args;
    va_start(args, format);

    ParseArguments(format, args);

    va_end(args);
}


void String::Set(pCHAR format, ...)
{
    Free();

    std::va_list args;
    va_start(args, format);

    ParseArguments(format, args);

    va_end(args);
}


void String::ParseArguments(pCHAR format, std::va_list args)
{
    int sizeBuffer = std::vsnprintf(nullptr, 0, format, args) + 1;

    Buffer buf(sizeBuffer);

    std::vsnprintf(buf.DataChar(), (uint)(sizeBuffer), format, args);

    Allocate(sizeBuffer);

    if (buffer != nullptr)
    {
        std::strcpy(buffer, buf.DataChar());
    }

}


void String::Append(pCHAR str)
{
    if (!str || *str == '\0')
    {
        return;
    }

    String old(*this);

    Free();

    Allocate(static_cast<int>(old.Size() + std::strlen(str) + 1)); //-V2513

    std::strcpy(buffer, old.c_str()); //-V2513
    std::strcat(buffer, str); //-V2513
}


void String::Append(pCHAR str, int numSymbols)
{
    if (!str || *str == '\0')
    {
        return;
    }

    String old(*this);

    Free();

    int size = numSymbols + old.Size() + 1;

    Allocate(size);

    std::strcpy(buffer, old.c_str()); //-V2513
    std::memcpy(buffer + old.Size(), str, static_cast<uint>(numSymbols)); //-V2563
    buffer[size - 1] = '\0'; //-V2563
}


void String::Append(char symbol)
{
    char b[2] = { symbol, '\0' };
    Append(b);
}


String::~String()
{
    std::free(buffer);
}


void String::Free()
{
    if(buffer)
    {
        std::free(buffer);
        buffer = nullptr;
        Set("");
    }
}


char *String::c_str() const
{
    return buffer;
}


void String::Allocate(int size)
{
    std::free(buffer);

    buffer = static_cast<char *>(std::malloc(static_cast<uint>(size)));
}


void String::RemoveFromBegin(int numSymbols)
{
    if (std::strlen(buffer) == static_cast<uint>(numSymbols))
    {
        Free();
    }
    else
    {
        String old(buffer);

        Free();

        Allocate(old.Size() - numSymbols + 1);

        std::strcpy(buffer, old.c_str() + numSymbols);
    }
}


void String::RemoveFromEnd()
{
    if(Size() > 0)
    {
        buffer[Size() - 1] = '\0'; //-V2563
    }
}


int String::Size() const
{
    if (buffer == nullptr)
    {
        return 0;
    }

    return static_cast<int>(std::strlen(buffer)); //-V2513
}


int String::Length() const
{
    return Font::GetLengthText(c_str());
}


char &String::operator[](int i) const
{
    static char result = 0;

    if (buffer == nullptr || Size() < i)
    {
        return result;
    }

    return buffer[i]; //-V2563
}
