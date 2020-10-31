#pragma once
#include "Display/Text.h"


using namespace Display;
using namespace Display::Primitives;


Text::Text(char *text)
{
    Create(text);
}


Text::Text(const char *text)
{
    Create(text);
}


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


Text::~Text()
{
    delete[]text;
}


int Text::Write(int x, int y, Color color)
{
    color.SetAsCurrent();

    if (text == nullptr)
    {
        return x;
    }

    char *p = text;

    while (*p)
    {
        x = WriteSymbol(x, y, (uint8)(*p++)) + 1;
    }
    return x;
}


int Text::Write(int x, int width, int y, Color color)
{
    int length = Font::GetLengthText(text);

    int delta = (width - length) / 2;

    Write(x + delta, y, color);
    return x;
}


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
