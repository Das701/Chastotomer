#include "defines.h"
#include "Display/Indicator.h"
#include "Display/Primitives.h"


using namespace Primitives;


static const int sizeLine = 26;

static const int width = sizeLine + 2;
static const int height = sizeLine * 2;

static const int bold = 2;


Color Indicator::colorDraw(Color::WHITE);
Color Indicator::colorBack(Color::BLACK);



void Indicator::DrawVLine(int x, int y, DirectV direct, DirectH directH, bool visible)
{
    if (!visible)
    {
        return;
    }

    if (visible)  { colorDraw.SetAsCurrent(); }
    else          { colorBack.SetAsCurrent(); }

    y--;

    if (direct == Right)
    {
        x -= 2;
    }

    if (directH == Down)
    {
        y -= 2;
    }

    VLine(sizeLine - 4).Draw(x, y);
    for (int i = 0; i < bold; i++)
    {
        VLine(sizeLine - 6 - i * 2).Draw(x - i - 1, y + 1 + i);
        VLine(sizeLine - 6 - i * 2).Draw(x + i + 1, y + 1 + i);
    }
}


void Indicator::DrawHLine(int x, int y, DirectH direct, bool visible)
{
    if (!visible)
    {
        return;
    }

    if (visible) { colorDraw.SetAsCurrent(); }
    else         { colorBack.SetAsCurrent(); }

    x--;

    if (direct == Mid)
    {
        y -= 2;
    }
    else if (direct == Down)
    {
        y -= 5;
    }

    HLine(sizeLine - 4).Draw(x, y);
    for (int i = 0; i < bold; i++)
    {
        HLine(sizeLine - 6 - i * 2).Draw(x + 1 + i, y - i - 1);
        HLine(sizeLine - 6 - i * 2).Draw(x + 1 + i, y + i + 1);
    }
}


void Indicator::Test(int x, int y, Color cDraw, Color cBack)
{
    DrawData("-123456789.0", x, y, cDraw, cBack);
}


void Indicator::DrawData(pString text, int x, int y, Color cDraw, Color cBack)
{
    colorDraw = cDraw;
    colorBack = cBack;

    pCHAR pointer = text;

    while (*pointer != '\0')
    {
        if ((*pointer >= '0' && *pointer <= '9'))
        {
            DrawDigit(x, y, (uint8)(*pointer - 0x30));

            x += DeltaX((uint8)*pointer);
        }
        else if (*pointer == '-')
        {
            DrawDigit(x, y, '-');
            x += DeltaX('-');
        }
        else if (*pointer == '.')
        {
            Rectangle(8, 8).Fill(x - 5, y + (int)(sizeLine * 1.7F));
            x += DeltaX('.');
        }

        pointer++;
    }
}


void Indicator::DrawDataAboutRight(pString text, int xRight, int y, Color cDraw, Color cBack)
{
    pCHAR pointer = text;

    int length = 0;

    while (*pointer != '\0')
    {
        length += DeltaX((uint8)*pointer);
        pointer++;
    }

    DrawData(text, xRight - length, y, cDraw, cBack);
}


int Indicator::DeltaX(uint8 symbol)
{
    if (symbol >= '0' && symbol <= '9')
    {
        return width + 7;
    }
    else if (symbol == '-')
    {
        return width + 7;
    }
    else if (symbol == '.')
    {
        return 7;
    }

    return 0;
}


void Indicator::DrawDigit(int x, int y, uint8 symbol)
{
    int d = 1;

#define SEG_A 0
#define SEG_B 1
#define SEG_C 2
#define SEG_D 3
#define SEG_E 4
#define SEG_F 5
#define SEG_G 6

    static const bool segments[11][7] =
    { //   A      B      C      D      E      F      G
        {true,  true,  true,  true,  true,  true,  false},    // 0
        {false, true,  true,  false, false, false, false},    // 1
        {true,  true,  false, true,  true,  false, true},     // 2
        {true,  true,  true,  true,  false, false, true},     // 3
        {false, true,  true,  false, false, true,  true},     // 4
        {true,  false, true,  true,  false, true,  true},     // 5
        {true,  false, true,  true,  true,  true,  true},     // 6
        {true,  true,  true,  false, false, false, false},    // 7
        {true,  true,  true,  true,  true,  true,  true},     // 8
        {true,  true,  true,  true,  false, true,  true},     // 9
        {false, false, false, false, false, false, true}      // '-'
    };

    if (symbol == '-')
    {
        symbol = 10;
    }

    DrawVLine(x + d, y + d + 2, Left, Up, segments[symbol][SEG_F]);
    DrawVLine(x + d, y + height / 2 + 2, Left, Down, segments[symbol][SEG_E]);

    DrawVLine(x + width - 2, y + d + 2, Right, Up, segments[symbol][SEG_B]);
    DrawVLine(x + width - 2, y + height / 2 + 2, Right, Down, segments[symbol][SEG_C]);

    DrawHLine(x + d + 2, y + d, Up, segments[symbol][SEG_A]);
    DrawHLine(x + d + 2, y + height / 2, Mid, segments[symbol][SEG_G]);
    DrawHLine(x + d + 2, y + height, Down, segments[symbol][SEG_D]);
}
