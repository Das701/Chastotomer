#pragma once
#include "Display/Font/Font.h"
#include "Display/Colors.h"


class String;


class Text
{
public:
    Text(pCHAR text);
    Text(const String &text);
    ~Text();
    int Write(int x, int y, Color color);
    int Write(int x, int y);
    int Write(int x, int y, int width, Color color);
    int Write(int x, int y, int width);
    void WriteInCenterRect(int x, int y, int width, int height, Color color);
    void WriteInCenterRect(int x, int y, int width, int height);
private:
    char *text;

    void Create(pCHAR text);
    int WriteSymbol(int x, int y, uint8 symbol) const;
    // Возвращает количество слов в строке
    int NumWords() const;
    // Возвращает информацию о слове numWord
    void GetWord(int numWord, pCHAR (&start), int &num);
    // Нарисовать num символов, начиная с адреса start
    int WriteSymbols(pCHAR start, int num, int x, int y) const;
    // Нарисовать num символов, начиная с адреса старт, в поле шириной width символов
    void WriteSymbols(pCHAR start, int num, int x, int y, int width, Color color) const;
    void WriteSymbols(pCHAR start, int num, int x, int y, int width) const;
};
