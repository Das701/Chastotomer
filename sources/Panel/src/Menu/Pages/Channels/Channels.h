#pragma once
#include "Menu/Pages/Modes/Modes.h"


class Page;
class Switch;


#define CURRENT_CHANNEL             Channel::Current()
#define CURRENT_CHANNEL_IS_A        (CURRENT_CHANNEL->IsA())
#define CURRENT_CHANNEL_IS_B        (CURRENT_CHANNEL->IsB())
#define CURRENT_CHANNEL_IS_A_OR_B   (CURRENT_CHANNEL_IS_A || CURRENT_CHANNEL_IS_B)
#define CURRENT_CHANNEL_IS_C        (CURRENT_CHANNEL->IsC())
#define CURRENT_CHANNEL_IS_D        (CURRENT_CHANNEL->IsD())

#define NUMBER_CHANNEL(ch)          (ch->Number())
#define NUMBER_CURRENT_CHANNEL      (NUMBER_CHANNEL(CURRENT_CHANNEL))


class PageModes : public Page
{
public:
    PageModes(Item **items, void (*onEvent)(EventType::E),
        Switch *switchModeFrequency, Switch *switchModeCountPulse, Switch *switchModePeriod, Switch *switchModeDuration,
        const bool *enabledMeasures, const bool *enabledModeFrequency, const bool *enabledModeCountPulse);

    // ������� ������������� ��� ������� ������� �������
    TypeMeasure *GetTypeMeasure() const;
    int GetModeMeasure() const;
    bool ExistTypeMeasure(uint8 type) const;
    void ResetTypeAndModeMeasure();
    bool ExistModeMeasure(int mode) const;
    void ResetModeMeasure();

    TypeMeasure    typeMeasure;
    ModeFrequency  modeFrequency;    // ����� ��������� �������
    ModePeriod     modePeriod;       // ����� ��������� �������
    ModeDuration   modeDuration;     // ����� ��������� ������������
    ModeCountPulse modeCountPulse;   // ����� ����� ���������

    Switch *switchModeFrequency;
    Switch *switchModeCountPulse;
    Switch *switchModePeriod;
    Switch *switchModeDuration;
};


struct SettingsChannel
{
    SettingsChannel();

    InputCouple    couple;
    InputImpedance impedance;
    ModeFilter     modeFilter;
    ModeFront      modeFront;
    Divider        divider;
    TypeSynch      typeSynch;
};


struct Channel
{
    static Channel *A;
    static Channel *B;
    static Channel *C;
    static Channel *D;

    static const int Count = 4;

    Channel(int number, Page *pSettings, PageModes *pModes);

    static void Create();

    static Channel *Current() { return current; };
    static void SetCurrent(Channel *channel) { current = channel; }
    static void SetCurrent(int num);

    // ���������� true, ���� ������� �������� ������� �������� ����� ���������
    bool ConsistTimeMeasure();

    void DrawParameters(int x, int y);

    bool IsA() const { return this == A; }
    bool IsB() const { return this == B; }
    bool IsC() const { return this == C; }
    bool IsD() const { return this == D; }

    int Number() const { return number; }

    // ��������� ����� � ����������
    void LoadToFPGA();

    // ���������� ��� ���������� ���� ���������
    void OnChanged_TypeMeasure();

    void PressSetup();

    Page *pageSettings;
    PageModes *mod;

    static TimeMeasure       timeMeasure;    // ����� �����
    static NumberPeriods     numberPeriods;  // ����� �������� ���������
    static PeriodTimeLabels  timeLabels;     // ������ ����� �������

    static Switch *switchTimeMeasue;
    static Switch *switchNumberPeriods;
    static Switch *switchTimeLabels;

    SettingsChannel set;

private:

    static Channel *current;                // ������� �����

    int number;
};
