#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/PagesModes.h"


struct Channel
{
    enum E
    {
        _A,
        _B,
        _C,
        _D,
        Count
    };

    static E current;                // ������� �����

    // ��������� ������� ����� � ����������
    static void LoadCurrentToFPGA();

    static Page *PageForChannel(Channel::E);

    static Switch *switchTimeMeasue;
    static Switch *switchNumberPeriods;
    static Switch *switchTimeLabels;

    static TimeMeasure       timeMeasure;    // ����� �����
    static NumberPeriods     numberPeriods;  // ����� �������� ���������
    static PeriodTimeLabels  timeLabels;     // ������ ����� �������

    static bool IsActiveTimeLabels(TypeMeasure *type, int mode);
    static bool IsActiveTimeMeasure(TypeMeasure *type, int mode);
    static bool IsActiveNumberPeriods(TypeMeasure *type, int mode);

    static Channel &Current();

    Channel(Page *pSettings, const bool *enabledMeasures);

    Page *settings;

    InputCouple    couple;
    InputImpedance impedance;
    ModeFilter     modeFilter;
    ModeFront      modeFront;
    Divider        divider;
    TypeSynch      typeSynch;
    TypeMeasure    typeMeasure;

    static Channel A;
    static Channel B;
    static Channel C;
    static Channel D;

    bool IsA() { return this == &A; }
    bool IsB() { return this == &B; }
};


extern Page pageChannelA;
extern Page pageChannelB;
extern Page pageChannelC;
extern Page pageChannelD;


struct PageSettingsA
{
    static void FixPress();
    static Switch *switchTypeSynch;     // ��������� �� ������������� ���� �������������
};


struct PageSettingsB
{
    static void FixPress();
    static Switch *switchTypeSynch;     // ��������� �� ������������� ���� �������������
};


struct PageSettingsC
{
    int i;
};

struct PageSettingsD
{
    int i;
};
