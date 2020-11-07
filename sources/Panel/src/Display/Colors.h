#pragma once
#include "defines.h"

struct SColor
{
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
    static Color GREY_10;   // 10% серого
    static Color GREY_50;   // 50% серого
    static Color GREY_20;   // 20% серого

    static const int Count = 256;

    explicit Color(uint8 val) : value(val) {};

    void SetAsCurrent();
    static Color GetCurrent();

    static uint MakeFromFloat(float r, float g, float b);
};


extern uint colors[Color::Count];

#define MAKE_COLOR(r, g, b) ((b) | ((g) << 8) | ((r) << 16))

#define COLOR(x) colors[x]
