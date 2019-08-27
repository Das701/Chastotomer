#pragma once
#include "Menu/MenuItems.h"


enum MeasureFrequency
{
    _1,
    _2,
    _3,
    _4
};

class PageModes
{
public:
    static Page *self;
    /// Возвращает режим измерения частоты
    static MeasureFrequency ModeMeasureFrequency();
};
