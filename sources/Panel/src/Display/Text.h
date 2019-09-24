#pragma once
#include "Display/Font/Font.h"
#include "Display/Colors.h"


namespace Display
{
    class Text
    {
    public:
        Text(char *text);
        Text(const char *text);
        ~Text();
        int Write(int x, int y, Color color = Color::Number);
        int Write(int x, int y, int width, Color color = Color::Number);
        void WriteInCenterRect(int x, int y, int width, int height, Color color = Color::Number);
    private:
        char *text;

        void Create(const char *text);
        int WriteSymbol(int x, int y, uint8 symbol) const;
        /// Возвращает количество слов в строке
        int NumWords() const;
        /// Возвращает информацию о слове numWord
        void GetWord(int numWord, char *(&start), int &num);
        /// Нарисовать num символов, начиная с адреса start
        int WriteSymbols(char *start, int num, int x, int y) const;
        /// Нарисовать num символов, начиная с адреса старт, в поле шириной width символов
        void WriteSymbols(char *start, int num, int x, int y, int width, Color color = Color::Number) const;
    };
}
