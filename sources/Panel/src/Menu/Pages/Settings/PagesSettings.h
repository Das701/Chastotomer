#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Settings.h"


struct PageSettingsA
{
    static Page *self;
    static InputCouple couple;
    static InputImpedance impedance;
    static ModeFilter modeFilter;
    static ModeFront modeFront;
    static Divider divider;
    static TypeSynch typeSynch;
    static void FixPress();
    static Switch *switchTypeSynch;     // ��������� �� ������������� ���� �������������
};


struct PageSettingsB
{
    static Page *self;
    static InputCouple couple;
    static InputImpedance impedance;
    static ModeFilter modeFilter;
    static ModeFront modeFront;
    static Divider divider;
    static TypeSynch typeSynch;
    static void FixPress();
    static Switch *switchTypeSynch;     // ��������� �� ������������� ���� �������������
};


struct PageSettingsC
{
    static Page *self;
};

struct PageSettingsD
{
    static Page *self;
};
