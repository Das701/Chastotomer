#include "defines.h"
#include "Display/Font/AdvancedFont.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


const Font *fonts[PTypeFont::Count] = {nullptr};
const Font *font = nullptr;

PTypeFont::E pushedFont = PTypeFont::None;
PTypeFont::E currentFont = PTypeFont::None;


struct NativeSymbol
{
    uint8 width;        // Ширина символа в пикселях
    uint8 height;       // Высота символа в пикселях
    uint8 firstRow;     // Номер первой непустой строки. Именно её первый байт хранится в data
//    uint8 data;     // Первый байт первой строки символа

    // Возвращает количество байт в строке
    int BytesInRow();
    // Возвращает указатель на первый байт строки
    uint8 *GetRow(int row);
    // Возвращает указатель на первый байт данных
    uint8 *Data();

    bool BitIsExist(int row, int bit);
};


// Структрура заголовка
struct HeaderFont
{
    uint16       offsets[256];  // Смещения 256 символов таблицы. 0 означает, что символ отсутствует
    NativeSymbol symbol;        // Первый символ в таблице его смещение 256

    // Возвращает указатель на символ, если он присутствует в таблице и nullptr в обратном случае
    NativeSymbol *GetSymbol(uint8 num);

    static HeaderFont *Sefl();
};


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


bool Font::RowNotEmpty(uint8 s, int r)
{
    HeaderFont *header = HeaderFont::Sefl();

    NativeSymbol *symbol = header->GetSymbol(s);

    if (symbol)
    {
        if (r < symbol->firstRow)
        {
            return false;
        }

        uint8 *row = symbol->GetRow(r);

        if (row)
        {
            for (int i = 0; i < symbol->BytesInRow(); i++)
            {
                if (row[i] != 0)
                {
                    return true;
                }
            }
        }
    }

    return false;
}


bool Font::BitIsExist(uint8 s, int row, int bit)
{
    NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(s);

    return symbol ? symbol->BitIsExist(row, bit) : false;
}


void Font::SetSpacing(int s)
{
    spacing = s;
}


int Font::GetSpacing()
{
    return spacing;
}
