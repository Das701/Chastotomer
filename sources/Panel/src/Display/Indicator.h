#pragma once
#include "Display/Colors.h"


struct LED
{
    static void DrawData(pString text, int x, int y, Color colorDraw, Color colorBack);
    static void DrawDataAboutRight(pString text, int xRight, int y, Color colorDraw, Color colorBack);

    static void Test(int x, int y, Color colorDraw, Color colorBack);

private:

    enum DirectV
    {
        Left,
        Right
    };


    enum DirectH
    {
        Up,
        Mid,
        Down
    };

    static void DrawDigit(int x, int y, uint8 symbol);

    static int DeltaX(uint8 symbol);

    // visible - если true, то сегмент должен быть "зажжён"
    static void DrawVLine(int x, int y, DirectV direct, DirectH directH, bool visible);

    static void DrawHLine(int x, int y, DirectH direct, bool visible);

    static Color colorDraw;
    static Color colorBack;
};
