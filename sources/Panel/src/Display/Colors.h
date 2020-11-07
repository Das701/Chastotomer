#pragma once
#include "defines.h"

struct SColor
{
//    uint8 r;
//    uint8 g;
//    uint8 b;

    static uint MakeFromUint8(uint8 red, uint8 green, uint8 blue);
};

struct Color
{
    uint8 value;

    static Color BLACK;
    static Color WHITE;
    static Color YELLOW;
    static Color BLUE;
    static Color RED;
    static Color GREEN;

    static const int Count = 256;

    explicit Color(uint8 val) : value(val) {};

    void SetAsCurrent();
    static Color GetCurrent();

    static uint MakeFromFloat(float r, float g, float b);
};


extern uint colors[Color::Count];

#define COLOR(x) colors[x]
