#pragma once
#include "Display/Font/Font.h"


struct PAdvancedFont
{
    PAdvancedFont() {};
    // ���������� ����������� �����
    PAdvancedFont(PTypeFont::E type);
    // ���������� ������ �������
    uint8 GetWidth(uint8 symbol);
    // ���������� ������ �������
    uint8 GetHeight(uint8 symbol);
    // ���������� ������ ������ �������� �������
    uint8 GetHeight();

private:
    static PTypeFont::E currentType;
};
