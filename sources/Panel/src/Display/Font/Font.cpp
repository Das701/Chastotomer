#include "defines.h"
#include "Display/Font/Font.h"
#include "Display/Font/fontGOST16B.inc"
//#include "Display/Font/fontGOSTB18B.inc"
//#include "Display/Font/fontGOSTB20B.inc"
#include "Display/Font/fontGOSTB28B.inc"
#include "Hardware/HAL/HAL.h"
#include <cstring>


struct NativeSymbol
{
    uint8 width;        // Ширина символа в пикселях
    uint8 height;       // Высота символа в пикселях
    uint8 firstRow;     // Номер первой непустой строки. Именно её первый байт хранится в data

    // Возвращает количество байт в строке
    int BytesInRow()
    {
        int result = width / 8;

        if (width % 8)
        {
            result++;
        }

        return result;
    }
    // Возвращает указатель на первый байт строки
    uint8 *GetRow(int row)
    {
        if (row > height - 1)
        {
            return nullptr;
        }

        if (row < firstRow)
        {
            return nullptr;
        }

        return Data() + (row - firstRow) * BytesInRow();
    }
    // Возвращает указатель на первый байт данных
    uint8 *Data();

    bool BitIsExist(int r, int b)
    {
        uint8 *row = GetRow(r);

        if (row == nullptr)
        {
            return false;
        }

        while (b > 7)       // Перемещаеммся к байту, содержащему наш бит
        {
            row++;
            b -= 8;
        }

        return ((*row) & (1 << (7 - b))) != 0;
    }
};


// Структрура заголовка
struct HeaderFont
{
    uint16       offsets[256];  // Смещения 256 символов таблицы. 0 означает, что символ отсутствует
    NativeSymbol symbol;        // Первый символ в таблице его смещение 256

    // Возвращает указатель на символ, если он присутствует в таблице и nullptr в обратном случае
    NativeSymbol *GetSymbol(uint8 num)
    {
        HeaderFont *header = HeaderFont::Sefl();

        if (header->offsets[num] == 0)
        {
            return nullptr;
        }

        uint8 *offset = reinterpret_cast<uint8 *>(header) + header->offsets[num];

        return reinterpret_cast<NativeSymbol *>(offset);
    }

    static HeaderFont *Sefl();
};


static int spacing = 1;


static const unsigned char *font = nullptr;


HeaderFont *HeaderFont::Sefl()
{
    return reinterpret_cast<HeaderFont *>(const_cast<uint8 *>(font));
}


uint8 *NativeSymbol::Data()
{
    return reinterpret_cast<uint8 *>(this) + sizeof(*this);
}


void Font::Set(const TypeFont::E typeFont)
{
    switch (typeFont)
    {
    case TypeFont::GOST16B:   font = fontGOST16B;   break;
//    case TypeFont::GOSTB18B:  font = fontGOSTB18B;  break;
//    case TypeFont::GOSTB20B:  font = fontGOSTB20B;  break;
    case TypeFont::GOSTB28B:  font = fontGOSTB28B;  break;
    case TypeFont::GOSTB18B:
    case TypeFont::GOSTB20B:
    case TypeFont::None:
    case TypeFont::Count:
        break;
    }
}


uint8 Font::GetWidth(uint8 num)
{
    if (num == 0x20)
    {
        if (font == fontGOST16B)
        {
            return 10;
        }
    }

    NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(num);

    return symbol ? symbol->width : 0U;
}


uint8 Font::GetWidth(char symbol)
{
    return GetWidth(static_cast<uint8>(symbol));
}


uint8 Font::GetHeight()
{
    uint8 result = 0;

    for (int i = 0; i < 256; i++)
    {
        NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(static_cast<uint8>(i));

        if (symbol && symbol->height > result)
        {
            result = symbol->height;
        }
    }

    return result;
}


uint8 Font::GetHeight(uint8 s)
{
    NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(s);

    return symbol ? symbol->height : 0U;
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


int Font::GetLengthSymbols(char *start, int num)
{
    int result = 0;

    for (int i = 0; i < num; i++)
    {
        result += GetWidth(*start++);
        result += spacing;
    }

    if (result > spacing)
    {
        result -= spacing;
    }

    return result;
}


int Font::GetLengthText(char *text)
{
    return GetLengthSymbols(text, (int)std::strlen(text));
}
