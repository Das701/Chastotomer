#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/Modes.h"


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

    Channel(Page *pSettings, Page *pModes, Switch *switchModeFrequency, Switch *switchModeCountPulse, Switch *switchModePeriod, Switch *switchModeDuration,
        const bool *enabledMeasures, const bool *enabledModeFrequency, const bool *enabledModeCountPulse);

    Page *pageSettings;
    Page *pageModes;

    InputCouple    couple;
    InputImpedance impedance;
    ModeFilter     modeFilter;
    ModeFront      modeFront;
    Divider        divider;
    TypeSynch      typeSynch;

    TypeMeasure    typeMeasure;
    ModeFrequency  modeFrequency;           // ����� ��������� �������
    ModePeriod     modePeriod;              // ����� ��������� �������
    ModeDuration   modeDuration;            // ����� ��������� ������������
    ModeCountPulse modeMeasureCountPulse;   // ����� ����� ���������

    // ���������� true, ���� ������� �������� ������� �������� ����� ���������
    bool ConsistTimeMeasure();

    void DrawParameters(int x, int y);

    bool IsA() { return this == &A; }
    bool IsB() { return this == &B; }

    static E current;                // ������� �����

    static Channel A;
    static Channel B;
    static Channel C;
    static Channel D;

    static Switch *switchTimeMeasue;
    static Switch *switchNumberPeriods;
    static Switch *switchTimeLabels;

    static TimeMeasure       timeMeasure;    // ����� �����
    static NumberPeriods     numberPeriods;  // ����� �������� ���������
    static PeriodTimeLabels  timeLabels;     // ������ ����� �������

    // ��������� ������� ����� � ����������
    static void LoadCurrentToFPGA();

    static Page *PageForChannel(Channel::E);

    static bool IsActiveTimeLabels(TypeMeasure *type, int mode);
    static bool IsActiveTimeMeasure(TypeMeasure *type, int mode);
    static bool IsActiveNumberPeriods(TypeMeasure *type, int mode);

    static Channel &Current();

    // ���������� ��� ���������� ���� ���������
    void OnChanged_TypeMeasure();

    static void PressSetup();
    static void PressSetupA();
    static void PressSetupB();

    static void RelationOn();
    static void RelationOff();
    static bool RelationCheck();

    static bool StartStop();
    static void ToggleStartStop();

private:

    Switch *switchModeFrequency;
    Switch *switchModeCountPulse;
    Switch *switchModePeriod;
    Switch *switchModeDuration;
};
