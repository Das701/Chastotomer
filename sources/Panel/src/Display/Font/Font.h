#pragma once
#include "defines.h"


struct PTypeFont
{
    enum E
    {
        GOST16B,
        Count,
        None
    } value;
};


struct PAdvancedFont
{
    // ���������� ����������� �����
    PAdvancedFont(PTypeFont::E type);

private:
    static PTypeFont::E currentType;
};


struct Font
{
    // ������������� ������� �����. ����� ������������� ����� ������������ �������� Pop()
    static void Set(const PTypeFont::E typeFont);
    static uint8 GetWidth(uint8 symbol);
    static uint8 GetWidth(char symbol);
    static uint8 GetHeight();
    static uint8 GetHeight(uint8 symbol);
    static bool RowNotEmpty(uint8 symbol, int row);
    static bool BitIsExist(uint8 symbol, int row, int bit);
    // ������������� ���������� ������ �������� ����� ���������.
    static void SetSpacing(int spacing);
    static int GetSpacing();
};
