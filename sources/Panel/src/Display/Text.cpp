#include "defines.h"
#include "Text.h"
#include "Display/Primitives.h"
#include "Menu/MenuItems.h"
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

    if (text)
    {
        x = WriteSymbols(text, (int)std::strlen(text), x, y);
    }

    return x;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Text::WriteSymbols(char* start, int num, int x, int y) const
{
    char* p = start;

    for (int i = 0; i < num; i++)
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

    int delta = (width - length*2) / 2;

    Write(x + delta, y, color);
    return x;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Text::WriteSymbols(char *start, int num, int x, int y, int width, Color color) const
{
    color.SetAsCurrent();

    int length = Font::GetLengthSymbols(start, num);

    int delta = (width - length*2) / 2;

    WriteSymbols(start, num, x + delta, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Text::WriteInCenterRect(int x, int y, int width, int height, Color color)
{
    volatile int numWords = NumWords();

    if (numWords == 1)
    {
        int dY = (height - Font::GetSize()) / 2;
        Write(x, y + dY, width, color);
    }
    else if (numWords == 2)
    {
        char* start = 0;
        int num = 0;

        GetWord(0, start, num);
        
        int dY = (height - Font::GetSize()) / 2 - Font::GetSize() / 2 - 6;
        WriteSymbols(start, num, x, y + dY, width, color);

        GetWord(1, start, num); 

        WriteSymbols(start, num, x, y + 6 + dY + static_cast<int>(1.5F * (float)Font::GetSize()), width);
    }
    else
    {
        // остальные варианты пока не трогаем
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// Пропускает пробелы
static void SkipSpaces(char** p)
{
    while (true)
    {
        if (**p == 0)
        {
        }
        else if (**p == ' ')
        {
            (*p)++;
            continue;
        }
        else
        {
            // остальные варианты не рассматриваем
        }

        break;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// Пропускает символы
static void SkipSymbols(char** p)
{
    while (true)
    {
        if (**p == 0)
        {
        }
        else if (**p != ' ')
        {
            (*p)++;
            continue;
        }
        else
        {
            // остальные варианты не рассматриваем
        }

        break;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Text::NumWords() const
{
    int result = 0;

    char* p = text;

    bool run = true;

    while (run)
    {
        SkipSpaces(&p);

        if (*p == 0)
        {
            run = false;
            continue;
        }

        result++;

        SkipSymbols(&p);

        if (*p == 0)
        {
            run = false;
            continue;
        }
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Text::GetWord(int numWord, char *(&start), int &num)
{
    char *p = text;

    int word = 0;

    while (true) //-V2530
    {
        num = 0;
        start = 0;

        SkipSpaces(&p);

        if (*p == 0)
        {
            num = 0;
            start = 0;
            break;
        }

        start = p;

        while (*p && *p != ' ')
        {
            num++;
            p++;
        }

        if (numWord == word)
        {

            break;
        }

        word++;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Text::WriteSymbol(int x, int y, uint8 chr) const
{
    int height = font->height;
    int width = font->GetLengthSymbol((char)chr);
    
    Symbol symbol = font->symbol[chr];

    for (int i = 0; i < height; i++)
    {
        uint8 row = symbol.bytes[i];
        
        int z = 0;
        
        for (int j = 0; j < width; j++)
        {
            if (_GET_BIT(row, 8 - j))
            {
                Point().Draw(x + j + z, y);
                Point().Draw(x + j + z, y + 1);
                z++;
                Point().Draw(x + j + z, y);
                Point().Draw(x + j + z, y + 1);
            }
            else
            {
                z++;
                
            }
        }

        y++;
        y++;
    }

    return x + symbol.width*2;
}
