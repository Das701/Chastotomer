#pragma once
#include "defines.h"
#include "FontBig.h"
#include "FontMid.h"


struct TypeFont
{
    enum E
    {
        GOSTAU16BOLD,
        GOSTB28B,
        Count,
        None
    } value;
};


struct Font
{
    // ������������� ������� �����. ����� ������������� ����� ������������ �������� Pop()
    static void Set(const TypeFont::E typeFont);
    static uint8 GetWidth(uint8 symbol);
    static uint8 GetWidth(char symbol);
    static uint8 GetHeight();
    static uint8 GetHeight(uint8 symbol);
    static bool RowNotEmpty(uint8 symbol, int row);
    static bool BitIsExist(uint8 symbol, int row, int bit);
    // ������������� ���������� ������ �������� ����� ���������.
    static void SetSpacing(int spacing);
    static int GetSpacing();
    static int GetLengthSymbols(pchar start, int num);
    static int GetLengthText(pchar text);
};
