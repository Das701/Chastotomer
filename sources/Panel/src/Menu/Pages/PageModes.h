#pragma once
#include "Menu/MenuItems.h"


struct MeasureFrequency
{
    enum E
    {
        Freq,       ///< �������
        AC,         ///< f(A) / f(C)
        AB,         ///< f(A) / f(B)
        T_1,        ///< f = 1 / T
        Tachometer  ///< ��������
    };
};

struct MeasurePeriod
{
    enum E
    {
        Period,     ///< ������
        F_1         ///< T = 1 / f
    };
};

class PageModes
{
public:
    static Page *self;
    /// ���������� ����� ��������� �������
    static MeasureFrequency::E ModeMeasureFrequency();
    /// ���������� ����� ��������� �������
    static MeasurePeriod::E ModeMeasurePeriod();
};
