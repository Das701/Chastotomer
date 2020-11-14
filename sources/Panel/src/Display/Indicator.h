#pragma once


struct Indicator
{
    static void Draw(int x, int y);

private:

    static void DrawDigit(int x, int y, uint8 symbol);
};
