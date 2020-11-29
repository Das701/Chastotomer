#pragma once
#include "Menu/Pages/Modes/Modes.h"


class Page;
class Switch;


#define CURRENT_CHANNEL             Channel::Current()
#define CURRENT_CHANNEL_IS_A        (CURRENT_CHANNEL.IsA())
#define CURRENT_CHANNEL_IS_B        (CURRENT_CHANNEL.IsB())
#define CURRENT_CHANNEL_IS_A_OR_B   (CURRENT_CHANNEL_IS_A || CURRENT_CHANNEL_IS_B)
#define CURRENT_CHANNEL_IS_C        (CURRENT_CHANNEL.IsC())
#define CURRENT_CHANNEL_IS_D        (CURRENT_CHANNEL.IsD())

#define NUMBER_CHANNEL(ch)          (ch.Number())
#define NUMBER_CURRENT_CHANNEL      (NUMBER_CHANNEL(CURRENT_CHANNEL))


struct SettingsChannel
{
    SettingsChannel() :
        couple(InputCouple::AC),
        impedance(InputImpedance::_1MOmh),
        modeFilter(ModeFilter::Off),
        modeFront(ModeFront::Front)
    {}
    InputCouple    couple;
    InputImpedance impedance;
    ModeFilter     modeFilter;
    ModeFront      modeFront;
};


struct Channel
{
    Channel(Page *pSettings, Page *pModes, Switch *switchModeFrequency, Switch *switchModeCountPulse, Switch *switchModePeriod, Switch *switchModeDuration,
        const bool *enabledMeasures, const bool *enabledModeFrequency, const bool *enabledModeCountPulse);

    // ���������� true, ���� ������� �������� ������� �������� ����� ���������
    bool ConsistTimeMeasure();

    void DrawParameters(int x, int y);

    bool IsA() const { return this == &A; }
    bool IsB() const { return this == &B; }
    bool IsC() const { return this == &C; }
    bool IsD() const { return this == &D; }

    int Number() const;

    // ��������� ������� ����� � ����������
    void LoadToFPGA();

    // ���������� ��� ���������� ���� ���������
    void OnChanged_TypeMeasure();

    void PressSetup();

    Page *pageSettings;
    Page *pageModes;

    SettingsChannel set;

    Divider        divider;
    TypeSynch      typeSynch;

    TypeMeasure    typeMeasure;
    ModeFrequency  modeFrequency;           // ����� ��������� �������
    ModePeriod     modePeriod;              // ����� ��������� �������
    ModeDuration   modeDuration;            // ����� ��������� ������������
    ModeCountPulse modeMeasureCountPulse;   // ����� ����� ���������

    static Channel &Current();
    static void SetCurrent(Channel &channel) { current = &channel; }

    static Channel A;
    static Channel B;
    static Channel C;
    static Channel D;

    static const int Count = 4;

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

    static Channel *current;                // ������� �����
};
