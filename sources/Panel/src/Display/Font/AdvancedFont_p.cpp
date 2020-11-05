#include "defines.h"
#include "AdvancedFont_p.h"
#include "fontGOST28.inc"
#include "fontGOST72bold.inc"


struct NativeSymbol
{
    uint8 width;        // ������ ������� � ��������
    uint8 height;       // ������ ������� � ��������
    uint8 firstRow;     // ����� ������ �������� ������. ������ � ������ ���� �������� � data
//    uint8 data;     // ������ ���� ������ ������ �������

    // ���������� ���������� ���� � ������
    int BytesInRow();
    // ���������� ��������� �� ������ ���� ������
    uint8 *GetRow(int row);
    // ���������� ��������� �� ������ ���� ������
    uint8 *Data();

    bool BitIsExist(int row, int bit);
};

// ���������� ���������
struct HeaderFont
{
    uint16       offsets[256];  // �������� 256 �������� �������. 0 ��������, ��� ������ �����������
    NativeSymbol symbol;        // ������ ������ � ������� ��� �������� 256

    // ���������� ��������� �� ������, ���� �� ������������ � ������� � nullptr � �������� ������
    NativeSymbol *GetSymbol(uint8 num);

    static HeaderFont *Sefl();
};


PTypeFont::E PAdvancedFont::currentType = PTypeFont::None;


static const unsigned char * font = nullptr;


PAdvancedFont::PAdvancedFont(PTypeFont::E t)
{
    currentType = t;
    
    if(currentType == PTypeFont::_GOST28)
    {
        font = fontGOST28;
    }
    else if(currentType == PTypeFont::_GOST72bold)
    {
        font = fontGOST72bold;
    }
    else
    {
        font = nullptr;
    }
}


bool PAdvancedFont::RowNotEmpty(uint8 s, int r)
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


uint8 PAdvancedFont::GetWidth(uint8 num)
{
    NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(num);

    return symbol ? symbol->width : 0U;
}

uint8 PAdvancedFont::GetHeight(uint8 num)
{
    NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(num);

    return symbol ? symbol->height : 0U;
}


uint8 PAdvancedFont::GetHeight()
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


int NativeSymbol::BytesInRow()
{
    int result = width / 8;

    if (width % 8)
    {
        result++;
    }

    return result;
}


uint8 *NativeSymbol::GetRow(int row)
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


NativeSymbol *HeaderFont::GetSymbol(uint8 num)
{
    HeaderFont *header = HeaderFont::Sefl();

    if (header->offsets[num] == 0)
    {
        return nullptr;
    }

    uint8 *offset = reinterpret_cast<uint8 *>(header) + header->offsets[num];

    return reinterpret_cast<NativeSymbol *>(offset);
}


HeaderFont *HeaderFont::Sefl()
{
    return reinterpret_cast<HeaderFont *>(const_cast<uint8 *>(font));
}


uint8 *NativeSymbol::Data()
{
    return reinterpret_cast<uint8 *>(this) + sizeof(*this);
}


bool PAdvancedFont::BitIsExist(uint8 s, int row, int bit)
{
    NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(s);

    return symbol ? symbol->BitIsExist(row, bit) : false;
}


bool NativeSymbol::BitIsExist(int r, int b)
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
