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
    static Color _1;
    static Color _2;
    static Color _3;
    static Color _4;
    static Color _5;
    static Color _6;
    static Color _7;
    static Color _8;
    static Color _9;
    static Color _10;
    static Color _11;
    static Color _12;
    static Color _13;
    static Color _14;
    static Color WHITE;

    static const int Count = 256;

    explicit Color(uint8 val) : value(val) {};

    void SetAsCurrent();
    static Color GetCurrent();

    static uint MakeFromFloat(float r, float g, float b);
};


extern uint colors[Color::Count];

#define COLOR(x) colors[x]
