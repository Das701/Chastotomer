#pragma once
#include "Display/Colors.h"
#include "Utils/String.h"


struct TypeConversionString
{
    enum E
    {
        None,           // Преобразование строки не производится
        FirstUpper      // Первый символ - в вернем регистре, остальные - в нижнем
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
    
    // Удаляет numSymbols из начала строки
    void RemoveFromBegin(int numSymbols);

    void RemoveFromEnd();

    int Size() const;

    char &operator[](int i) const;

private:

    bool Allocate(int size);
    void Conversion(TypeConversionString::E conv);

    char *buffer;
};
