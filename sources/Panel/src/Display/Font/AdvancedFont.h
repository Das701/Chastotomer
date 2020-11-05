#pragma once
#include "Display/Font/Font.h"


struct PAdvancedFont
{
    PAdvancedFont() {};
    // Установить действующий шрифт
    PAdvancedFont(PTypeFont::E type);
    // Возвращает высоту символа
    uint8 GetHeight(uint8 symbol);

private:
    static PTypeFont::E currentType;
};
