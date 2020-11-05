#pragma once
#include "defines.h"


struct PSymbol
{
    uchar width;
    uchar bytes[8];
};

struct PTypeFont
{
    enum E
    {
        GOST16B,
        Count,
        None
    } value;
};

struct Font
{
    int _height;
    PSymbol symbols[256];

public:
    // Устанавливает текущий шрифт. Ранее установленный можно восстановить функцией Pop()
    static void Set(const PTypeFont::E typeFont);
    static PTypeFont::E Current();
    // Восстанавливает шрифт, бывший текущим перед последним вызовом SetCurrent()
    static void Pop();
    static uint8 GetWidth(uint8 symbol);
    static uint8 GetWidth(char symbol);
    static uint8 GetHeight();
    static uint8 GetHeight(uint8 symbol);
    static bool RowNotEmpty(uint8 symbol, int row);
    static bool BitIsExist(uint8 symbol, int row, int bit);
    // Устанавливает количество пустых пикселей между символами.
    static void SetSpacing(int spacing);
    static int GetSpacing();
};
