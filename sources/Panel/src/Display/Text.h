#pragma once
#include "Display/Font/Font.h"
#include "Display/Colors.h"


namespace Display
{
    class Text
    {
    public:
        Text(char *text);
        ~Text();
        void Write(int x, int y, Color color = Color::Number);
        void Write(int x, int width, int y, Color color = Color::Number);
    private:
        char *text;

        int WriteSymbol(int x, int y, uint8 symbol);
    };
}
