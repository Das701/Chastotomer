#pragma once
#include "Display/Colors.h"


struct Indicator
{
    static void DrawData(pString text, int x, int y, Color color);
    static void DrawDataAboutRight(pString text, int xRight, int y, Color color);

    static void Test(int x, int y, Color color);

private:

    static void DrawDigit(int x, int y, uint8 symbol);
    static int DeltaX(uint8 symbol);
};
