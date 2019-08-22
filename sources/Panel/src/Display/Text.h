#pragma once
#include "Display/Font/Font.h"
#include "Display/Colors.h"


namespace Display
{
    class Text
    {
    public:
        Text(char *text, Font::Type type = Font::Type::_8);
        ~Text();
        void Write(int x, int y, Color color = Color::Number);
    private:
        char *text;
        Font::Type type;

        int WriteSymbol(int x, int y, uint8 symbol);
    };
}
