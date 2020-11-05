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
        _5,
        _8,
        _UGO,
        _UGO2,
        _GOST28,
        _GOST72bold,    // Используется для вывода показаний мультиметра
        _OMEGA72,       // Используется в мультиметре для вывода обозначения ом. Имеет только один символ
        Count,
        None
    } value;
};

struct PFont
{
    int _height;
    PSymbol symbols[256];

public:
    // Устанавливает текущий шрифт. Ранее установленный можно восстановить функцией Pop()
    static void Set(const PTypeFont::E typeFont);
    static PTypeFont::E Current();
    // Восстанавливает шрифт, бывший текущим перед последним вызовом SetCurrent()
    static void Pop();
    static bool IsBig();
    static uint8 GetWidth(uint8 symbol);
    static uint8 GetWidth(char symbol);
    static uint8 GetHeight();
    static bool RowNotEmpty(uint8 symbol, int row);
    static bool BitIsExist(uint8 symbol, int row, int bit);
    // Устанавливает количество пустых пикселей между символами.
    static void SetSpacing(int spacing);
};
