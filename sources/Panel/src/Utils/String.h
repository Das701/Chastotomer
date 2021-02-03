#pragma once
#include "Display/Colors.h"
#include "Utils/String.h"


class String // -V690
{
public:
    explicit String();
             String(const String &);
    explicit String(char symbol);
             String(pCHAR format, ...);
    ~String();

    void Set(pCHAR format, ...);

    char *c_str() const;

    static pString _ERROR; //-V2573

    void Free();

    void Append(pCHAR str);

    void Append(pCHAR str, int numSymbols);

    void Append(char symbol);
    
    // Удаляет numSymbols из начала строки
    void RemoveFromBegin(int numSymbols);

    void RemoveFromEnd();

    int Length() const;

    int Size() const;

    char &operator[](int i) const;

private:

    void Allocate(int size);

    char *buffer;
};
