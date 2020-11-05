#pragma once
#include "Display/Font/Font.h"


struct PAdvancedFont
{
    // Установить действующий шрифт
    PAdvancedFont(PTypeFont::E type);

private:
    static PTypeFont::E currentType;
};
