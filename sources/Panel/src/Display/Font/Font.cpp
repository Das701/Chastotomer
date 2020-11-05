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
    uint8 width;        // ������ ������� � ��������
    uint8 height;       // ������ ������� � ��������
    uint8 firstRow;     // ����� ������ �������� ������. ������ � ������ ���� �������� � data
//    uint8 data;     // ������ ���� ������ ������ �������

    // ���������� ���������� ���� � ������
    int BytesInRow()
    {
        int result = width / 8;

        if (width % 8)
        {
            result++;
        }

        return result;
    }
    // ���������� ��������� �� ������ ���� ������
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
    // ���������� ��������� �� ������ ���� ������
    uint8 *Data();

    bool BitIsExist(int r, int b)
    {
        uint8 *row = GetRow(r);

        if (row == nullptr)
        {
            return false;
        }

        while (b > 7)       // ������������� � �����, ����������� ��� ���
        {
            row++;
            b -= 8;
        }

        return ((*row) & (1 << (7 - b))) != 0;
    }
};


// ���������� ���������
struct HeaderFont
{
    uint16       offsets[256];  // �������� 256 �������� �������. 0 ��������, ��� ������ �����������
    NativeSymbol symbol;        // ������ ������ � ������� ��� �������� 256

    // ���������� ��������� �� ������, ���� �� ������������ � ������� � nullptr � �������� ������
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


uint8 Font::GetWidth(uint8 num)
{
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
