#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/PageModesA.h"


class PageModesD
{
public:
    static Page *self;

    static TypeMeasure          typeMeasure;
    static ModeMeasureFrequency modeMeasureFrequency;
    static TimeMeasure          timeMeasure;           // Время счета
};
