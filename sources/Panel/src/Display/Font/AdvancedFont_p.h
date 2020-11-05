#pragma once
#include "Font_p.h"



struct PAdvancedFont
{
    PAdvancedFont() {};
    // ���������� ����������� �����
    PAdvancedFont(PTypeFont::E type);
    // ���������� true, ���� � ������ row ���� �������
    bool RowNotEmpty(uint8 symbol, int row);
    // ���������� ������ �������
    uint8 GetWidth(uint8 symbol);
    // ���������� ������ �������
    uint8 GetHeight(uint8 symbol);
    // ���������� ������ ������ �������� �������
    uint8 GetHeight();

    bool BitIsExist(uint8 symbol, int row, int bit);

private:
    static PTypeFont::E currentType;
};
