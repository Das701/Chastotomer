#pragma once
#include "defines.h"


struct Color
{
    uint8 value;

    static Color BLACK;
    static Color WHITE;

    static Color GRAY_10;   // 10% серого
    static Color GRAY_20;   // 20% серого
    static Color GRAY_50;   // 50% серого
    static Color GRAY_75;   // 75% серого

    static Color RED;

    static Color GREEN;
    static Color GREEN_5;
    static Color GREEN_10;
    static Color GREEN_12;
    static Color GREEN_15;
    static Color GREEN_20;
    static Color GREEN_25;
    static Color GREEN_50;

    static Color BLUE;

    static Color YELLOW;

    static const int Count = 256;

    explicit Color(uint8 val) : value(val) {};

    void SetAsCurrent();
    static Color GetCurrent();
};


extern uint colors[Color::Count];

#ifdef WIN32
    #define MAKE_COLOR(r, g, b)     ((uint)((b) | ((g) << 8) | ((r) << 16)))
    #define RED_FROM_COLOR(color)   ((uint8)(((color) >> 16) & 0xFF))
    #define GREEN_FROM_COLOR(color) ((uint8)(((color) >> 8) & 0xFF))
    #define BLUE_FROM_COLOR(color)  ((uint8)(color & 0xFF))
#else
    #define MAKE_COLOR(r, g, b) ((uint)((r) | ((g) << 8) | ((b) << 16)))
#endif

#define COLOR(x) colors[x]
