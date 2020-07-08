#pragma once
#include "Menu/MenuItems.h"

/// Время счёта
struct TimeMeasureD : public Enumeration
{
    enum E
    {
        _1ms,       ///< 1ms
        _10ms,      ///< 10ms
        _100ms,     ///< 100ms
        _1s,        ///< 1s
        _10s,       ///< 10s
        _100s,       ///< 100s
        _1000s
    };

    explicit TimeMeasureD(E v) : Enumeration((uint8)v) {};
};

class PageModesD
{
public:
    static Page *self;

    /// Время счета
    static TimeMeasureD timeMeasureD;
};
