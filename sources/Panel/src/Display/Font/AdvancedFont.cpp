#include "defines.h"
#include "Display/Font/AdvancedFont.h"
#include "Display/Font/fontGOST16B.inc"


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


HeaderFont *HeaderFont::Sefl()
{
    return reinterpret_cast<HeaderFont *>(const_cast<uint8 *>(font));
}


uint8 *NativeSymbol::Data()
{
    return reinterpret_cast<uint8 *>(this) + sizeof(*this);
}
