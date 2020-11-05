#pragma once
#include "Display/Font/Font.h"


struct PAdvancedFont
{
    PAdvancedFont() {};
    // Установить действующий шрифт
    PAdvancedFont(PTypeFont::E type);
    // Возвращает ширину символа
    uint8 GetWidth(uint8 symbol);
    // Возвращает высоту символа
    uint8 GetHeight(uint8 symbol);
    // Возвращает высоту самого высокого символа
    uint8 GetHeight();

    bool BitIsExist(uint8 symbol, int row, int bit);

private:
    static PTypeFont::E currentType;
};
