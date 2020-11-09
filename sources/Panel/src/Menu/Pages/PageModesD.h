#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/PageModesA.h"


struct TypeMeasureD : public Enumeration
{
    enum E
    {
        Frequency,
        Count
    };

    explicit TypeMeasureD(E v) : Enumeration((uint8)v) {};
};


class PageModesD
{
public:
    static Page *self;

    static TypeMeasureD typeMeasure;
    static TimeMeasure  timeMeasure;     // Время счета
};
