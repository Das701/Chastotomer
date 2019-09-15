#pragma once
#include "Menu/MenuItems.h"
#include "PageChannelA.h"


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

    static Switch *switchTypeSynch;     ///< ��������� �� ������������� ���� �������������
};

