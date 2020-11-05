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
        int Write(int x, int y, Color color);
        int Write(int x, int y);
        int Write(int x, int y, int width, Color color);
        int Write(int x, int y, int width);
        void WriteInCenterRect(int x, int y, int width, int height, Color color);
        void WriteInCenterRect(int x, int y, int width, int height);
    private:
        char *text;

        void Create(const char *text);
        int WriteSymbol(int x, int y, uint8 symbol) const;
        // ���������� ���������� ���� � ������
        int NumWords() const;
        // ���������� ���������� � ����� numWord
        void GetWord(int numWord, char *(&start), int &num);
        // ���������� num ��������, ������� � ������ start
        int WriteSymbols(char *start, int num, int x, int y) const;
        // ���������� num ��������, ������� � ������ �����, � ���� ������� width ��������
        void WriteSymbols(char *start, int num, int x, int y, int width, Color color) const;
        void WriteSymbols(char *start, int num, int x, int y, int width) const;
    };
}
