#include "defines.h"
#include "Display/Font/AdvancedFont.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


const Font *fonts[PTypeFont::Count] = {nullptr};
const Font *font = nullptr;

PTypeFont::E pushedFont = PTypeFont::None;
PTypeFont::E currentFont = PTypeFont::None;


static int spacing = 1;


PTypeFont::E Font::Current()
{
    return currentFont;
}


void Font::Set(const PTypeFont::E typeFont)
{
    pushedFont = currentFont;

    if (typeFont != currentFont)
    {
        switch (typeFont)
        {
        case PTypeFont::GOST16B:
        {
            font = nullptr;
            volatile PAdvancedFont f(typeFont);
        }       
            break;
        case PTypeFont::None:
        case PTypeFont::Count:
            break;
        }

        currentFont = typeFont;
    }
}


void Font::Pop()
{
    Set(pushedFont);
}


uint8 Font::GetWidth(uint8 symbol)
{
    return PAdvancedFont().GetWidth(symbol);
}


uint8 Font::GetWidth(char symbol)
{
    return GetWidth(static_cast<uint8>(symbol));
}


uint8 Font::GetHeight()
{
    return PAdvancedFont().GetHeight();
}


bool Font::RowNotEmpty(uint8 symbol, int row)
{
    return PAdvancedFont().RowNotEmpty(symbol, row);
}


bool Font::BitIsExist(uint8 symbol, int row, int bit)
{
    return PAdvancedFont().BitIsExist(symbol, row, bit);
}


void Font::SetSpacing(int s)
{
    spacing = s;
}


int Font::GetSpacing()
{
    return spacing;
}
