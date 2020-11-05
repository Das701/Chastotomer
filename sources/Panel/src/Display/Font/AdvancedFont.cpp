#include "defines.h"
#include "Display/Font/AdvancedFont.h"
#include "Display/Font/fontGOST16B.inc"


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
    
    if(currentType == PTypeFont::GOST16B)
    {
        font = fontGOST16B;
    }
    else
    {
        font = nullptr;
    }
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


HeaderFont *HeaderFont::Sefl()
{
    return reinterpret_cast<HeaderFont *>(const_cast<uint8 *>(font));
}


uint8 *NativeSymbol::Data()
{
    return reinterpret_cast<uint8 *>(this) + sizeof(*this);
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
