#pragma once
#include "Menu/MenuItems.h"


struct MeasureFrequency
{
    enum E
    {
        Freq,       ///< Частота
        AC,         ///< f(A) / f(C)
        AB,         ///< f(A) / f(B)
        T_1,        ///< f = 1 / T
        Tachometer  ///< Тахометр
    };
};

struct MeasurePeriod
{
    enum E
    {
        Period,     ///< Период
        F_1         ///< T = 1 / f
    };
};

class PageModes
{
public:
    static Page *self;
    /// Возвращает режим измерения частоты
    static MeasureFrequency::E ModeMeasureFrequency();
    /// Возвращает режим измерения периода
    static MeasurePeriod::E ModeMeasurePeriod();
};
