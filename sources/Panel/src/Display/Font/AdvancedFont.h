#pragma once
#include "Display/Font/Font.h"


struct PAdvancedFont
{
    PAdvancedFont() {};
    // ���������� ����������� �����
    PAdvancedFont(PTypeFont::E type);

private:
    static PTypeFont::E currentType;
};
