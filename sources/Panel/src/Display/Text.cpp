#include "defines.h"
#include "Text.h"
#include "Display/Primitives.h"
#include <cstring>


using namespace Display;
using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Text::Text(char *_text, Font::Type _type) : type(_type)
{
    uint numSymbols = std::strlen(_text);

    if (numSymbols)
    {
        text = new char[numSymbols + 1];
        std::strcpy(text, _text);
    }
    else
    {
        text = nullptr;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::~Text()
{
    delete []text;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Text::Write(int x, int y, Color color)
{
    color.SetAsCurrent();

    Font::SetType(type);

    char *p = text;

    while (*p)
    {
        x = WriteSymbol(x, y, (uint8)(*p++)) + 1;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Text::WriteSymbol(int x, int y, uint8 chr)
{
    int height = font->height;
    int width = font->GetLengthSymbol((char)chr);
    
    Symbol symbol = font->symbol[chr];

    for (int i = 0; i < height; i++)
    {
        uint8 row = symbol.bytes[i];

        for (int j = 0; j < width; j++)
        {
            if (GET_BIT(row, 8 - j))
            {
                Point().Draw(x + j, y);
            }
        }

        y++;
    }

    return x + symbol.width;
}
