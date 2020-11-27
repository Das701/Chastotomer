#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Settings.h"


struct PageSettingsA
{
    static MenuPage *self;
    static InputCouple couple;
    static InputImpedance impedance;
    static ModeFilter modeFilter;
    static ModeFront modeFront;
    static Divider divider;
    static TypeSynch typeSynch;
    static void FixPress();
    static Switch *switchTypeSynch;     // ”казатель на переключатель типа синхронизации
};


struct PageSettingsB
{
    static MenuPage *self;
    static InputCouple couple;
    static InputImpedance impedance;
    static ModeFilter modeFilter;
    static ModeFront modeFront;
    static Divider divider;
    static TypeSynch typeSynch;
    static void FixPress();
    static Switch *switchTypeSynch;     // ”казатель на переключатель типа синхронизации
};


struct PageSettingsC
{
    static MenuPage *self;
};

struct PageSettingsD
{
    static MenuPage *self;
};
