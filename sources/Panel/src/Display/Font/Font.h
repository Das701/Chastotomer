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
            _5,
            _7,
            _8,
            Number,
            None
        } value;
        Type(E v) : value(v) {};
        operator uint8() const { return (uint8)value; };
    };

    static int GetSize();
    static int GetLengthText(const char *text);
    static int GetHeightSymbol(char symbol);
    static int GetLengthSymbol(char symbol);
    static void SetType(Type typeFont);
    
    int height;
    Symbol symbol[256];
};


extern const Font *font;
extern const Font *fonts[Font::Type::Number];

extern const uint8 font5display[3080];
extern const uint8 font8display[3080];
extern const uint8 fontUGOdisplay[3080];
extern const uint8 fontUGO2display[3080];
