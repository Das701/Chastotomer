#include "defines.h"
#include "Font.h"
#include "font8.inc"



const Font *fonts[Font::Type::Number] = {&font8};
const Font *font = &font8;


static Font::Type::E type = Font::Type::_8;


int Font::GetSize()
{
    return font->height;
}


int Font::GetLengthText(const char *text)
{
    int result = 0;

    while (*text)
    {
        result += GetLengthSymbol(*text);
        text++;
    }

    return result;
}


int Font::GetLengthSymbols(const char *text, int num)
{
    int result = 0;

    for (int i = 0; i < num; i++)
    {
        result += GetLengthSymbol(*text);
        text++;
    }

    return result;
}


int Font::GetHeightSymbol(char)
{
    return 9;
}


int Font::GetLengthSymbol(char symbol)
{
    return font->symbol[(uint8)symbol].width + 1;
}


void Font::SetType(Type::E typeFont)
{
    type = typeFont;
    font = fonts[type];
}
