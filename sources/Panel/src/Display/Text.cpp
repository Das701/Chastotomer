#include "defines.h"
#include "Text.h"
#include "Display/Primitives.h"
#include <cstring>


using namespace Display;
using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Text::Text(char *t)
{
    Create(t);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::Text(const char *t)
{
    Create(t);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::~Text()
{
    delete []text;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Text::Create(const char *_text)
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
int Text::Write(int x, int y, Color color)
{
    color.SetAsCurrent();
    
    char *p = text;
    
    while (text && *p)
    {
        x = WriteSymbol(x, y, (uint8)(*p++)) + 1;
    }
    return x;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Text::Write(int x, int y, int width, Color color)
{
    color.SetAsCurrent();

    int length = Font::GetLengthText(text);

    int delta = (width - length) / 2;

    Write(x + delta, y, color);
    return x;
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
