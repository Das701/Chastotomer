#pragma once
#include "Display/Colors.h"
#include "Utils/String.h"


struct TypeConversionString
{
    enum E
    {
        None,           // �������������� ������ �� ������������
        FirstUpper      // ������ ������ - � ������ ��������, ��������� - � ������
    };
};


class String // -V690
{
public:
    explicit String();
             String(const String &);
    explicit String(char symbol);
    explicit String(pCHAR format, ...);
    ~String();

    void Set(TypeConversionString::E conv, pCHAR format, ...);

    char *c_str() const;

    static pString _ERROR;

    void Free();

    void Append(pCHAR str);

    void Append(pCHAR str, int numSymbols);

    void Append(char symbol);
    
    // ������� numSymbols �� ������ ������
    void RemoveFromBegin(int numSymbols);

    void RemoveFromEnd();

    int Size() const;

    char &operator[](int i) const;

private:

    bool Allocate(int size);
    void Conversion(TypeConversionString::E conv);

    char *buffer;
};
