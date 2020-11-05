#pragma once
#include "defines.h"


struct Symbol
{
    uint8 width;
    uint8 bytes[8];
};


struct Font
{
    struct Type
    {
        enum E
        {
            _8,
            Number,
            None
        } value;
        explicit Type(E v = None) : value(v) {};
        operator uint8() const { return (uint8)value; };
    };

    static int GetSize();
    static int GetLengthText(const char *text);
    static int GetLengthSymbols(const char *text, int num);
    static int GetHeightSymbol(char symbol);
    static int GetLengthSymbol(char symbol);
    static void SetType(Type::E typeFont);
    
    int height;
    Symbol symbol[256];
};


// Текущий шрифт
extern const Font *font;
