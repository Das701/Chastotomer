#pragma once
#include "Menu/MenuItems.h"


enum MeasureFrequency
{
    _Freq,
    _AC,
    _AB,
    _fT,
    _tachometer
    
};

enum MeasurePeriod
{
    _Period,
    _Tf
    
};

class PageModes
{
public:
    static Page *self;
    /// Возвращает режим измерения частоты
    static MeasureFrequency ModeMeasureFrequency();
    static MeasurePeriod ModeMeasurePeriod();
};
