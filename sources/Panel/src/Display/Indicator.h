#pragma once
#include "Display/Colors.h"


struct Indicator
{
//    static void Draw(int x, int y);

    static void DrawData(pString text, int x, int y, Color color);

private:

    static void DrawDigit(int x, int y, uint8 symbol);
};
