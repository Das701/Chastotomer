#pragma once
#include "Menu/MenuItems.h"


enum MeasureFrequency
{
    _Freq,
    _AC,
    _AB,
    _fT,
    _tachometer,
    
};

class PageModes
{
public:
    static Page *self;
    /// ���������� ����� ��������� �������
    static MeasureFrequency ModeMeasureFrequency();
};
