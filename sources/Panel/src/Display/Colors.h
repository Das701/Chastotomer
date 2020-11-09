#pragma once
#include "defines.h"


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
    static Color GREEN_50;
    static Color GREEN_25;
    static Color GREEN_10;
    static Color GREEN_20;

    static const int Count = 256;

    explicit Color(uint8 val) : value(val) {};

    void SetAsCurrent();
    static Color GetCurrent();
};


extern uint colors[Color::Count];

#ifdef WIN32
    #define MAKE_COLOR(r, g, b) ((uint)((b) | ((g) << 8) | ((r) << 16)))
#else
    #define MAKE_COLOR(r, g, b) ((uint)((r) | ((g) << 8) | ((b) << 16)))
#endif

#define COLOR(x) colors[x]
