#pragma once
#include "Display/Font/Font.h"


struct PAdvancedFont
{
    PAdvancedFont() {};
    // ���������� ����������� �����
    PAdvancedFont(PTypeFont::E type);
    // ���������� ������ �������
    uint8 GetHeight(uint8 symbol);

private:
    static PTypeFont::E currentType;
};
