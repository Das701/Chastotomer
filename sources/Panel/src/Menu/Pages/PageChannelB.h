#pragma once
#include "Menu/MenuItems.h"
#include "PageChannelA.h"
#include "PageModes.h"
#include "PageModesB.h"
#include "PageModesC.h"
#include "PageModesD.h"

class PageChannelB
{
public:
    static Page *self;
    static InputCouple couple;
    static InputImpedance impedance;
    static ModeFilter modeFilter;
    static ModeFront modeFront;
    static Divider divider;
    static TypeSynch typeSynch;
    static void FixPress();
    static Switch *switchTypeSynch;     ///< ”казатель на переключатель типа синхронизации
};

