#include "defines.h"
#include "Display/Indicator.h"
#include "Display/Primitives.h"


using namespace Primitives;


static const int sizeLine = 30;

static const int width = sizeLine + 2;
static const int height = sizeLine * 2;

static const bool BOLD = true;

static const int bold = 2;


enum DirectV
{
    Left,
    Right
};

static void DrawVLine(int x, int y, DirectV direct)
{
    VLine(sizeLine - 4).Draw(x, y);
    for (int i = 0; i < bold; i++)
    {
        VLine(sizeLine - 6 - i * 2).Draw(x - i - 1, y + 1 + i);
        VLine(sizeLine - 6 - i * 2).Draw(x + i + 1, y + 1 + i);
    }
}


enum DirectH
{
    Up,
    Mid,
    Down
};

static void DrawHLine(int x, int y, DirectH direct)
{
    HLine(sizeLine - 4).Draw(x, y);
    for (int i = 0; i < bold; i++)
    {
        HLine(sizeLine - 6 - i * 2).Draw(x + 1 + i, y - i - 1);
        HLine(sizeLine - 6 - i * 2).Draw(x + 1 + i, y + i + 1);
    }
}


void Indicator::Draw(int x, int y)
{
    static const int numSymbols = 10;

    static const int dX = 5;

    for (int i = 0; i < numSymbols; i++)
    {
        DrawDigit(x + (width + dX) * i, y, (uint8)i);
    }
}


void Indicator::DrawDigit(int x, int y, uint8 symbol)
{
    Rectangle(width, height).Draw(x, y, Color::GRAY_20);

    Color::WHITE.SetAsCurrent();

    int d = 1;

#define SEG_A 0
#define SEG_B 1
#define SEG_C 2
#define SEG_D 3
#define SEG_E 4
#define SEG_F 5
#define SEG_G 6

    static const bool segments[10][7] =
    { //   A      B      C      D      E      F      G
        {true,  true,  true,  true,  true,  true,  false},     // 0
        {false, true,  true,  false, false, false, false},     // 1
        {true,  true,  false, true,  true,  false, true},     // 2
        {true,  true,  true,  true,  false, false, true},     // 3
        {false, true,  true,  false, false, true,  true},     // 4
        {true,  false, true,  true,  false, true,  true},     // 5
        {true,  false, true,  true,  true,  true,  true},     // 6
        {true,  true,  true,  false, false, false, false},     // 7
        {true,  true,  true,  true,  true,  true,  true},     // 8
        {true,  true,  true,  true,  false,  true,  true}      // 9
    };

    if (segments[symbol][SEG_F])    DrawVLine(x + d, y + d + 2, Left);
    if (segments[symbol][SEG_E])    DrawVLine(x + d, y + height / 2 + 2, Left);

    if (segments[symbol][SEG_B])    DrawVLine(x + width - 2, y + d + 2, Right);
    if (segments[symbol][SEG_C])    DrawVLine(x + width - 2, y + height / 2 + 2, Right);

    if (segments[symbol][SEG_A])    DrawHLine(x + d + 2, y + d, Up);
    if (segments[symbol][SEG_G])    DrawHLine(x + d + 2, y + height / 2, Mid);
    if (segments[symbol][SEG_D])    DrawHLine(x + d + 2, y + height, Down);
}
