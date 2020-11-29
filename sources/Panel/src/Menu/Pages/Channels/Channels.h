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

    // ���������� true, ���� ������� �������� ������� �������� ����� ���������
    bool ConsistTimeMeasure();

    void DrawParameters(int x, int y);

    bool IsA() { return this == &A; }
    bool IsB() { return this == &B; }
    bool IsC() { return this == &C; }
    bool IsD() { return this == &D; }

    // ��������� ������� ����� � ����������
    void LoadToFPGA();

    bool IsActiveNumberPeriods(TypeMeasure *type, int mode);

    // ���������� ��� ���������� ���� ���������
    void OnChanged_TypeMeasure();

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

    static Channel &Current();

    static void PressSetup();
    static void PressSetupA();
    static void PressSetupB();

    static void RelationOn();
    static void RelationOff();
    static bool RelationCheck();

    static bool StartStop();
    static void ToggleStartStop();

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

private:

    Switch *switchModeFrequency;
    Switch *switchModeCountPulse;
    Switch *switchModePeriod;
    Switch *switchModeDuration;
};
