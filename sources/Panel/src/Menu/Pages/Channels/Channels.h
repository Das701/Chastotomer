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

    Channel(Page *pSettings, const bool *enabledMeasures, const bool *enabledModeFrequency);

    Page *pageSettings;

    InputCouple    couple;
    InputImpedance impedance;
    ModeFilter     modeFilter;
    ModeFront      modeFront;
    Divider        divider;
    TypeSynch      typeSynch;

    TypeMeasure    typeMeasure;
    ModeFrequency  modeFrequency;   // Режим измерения частоты
    ModePeriod     modePeriod;      // Режим измерения периода

    bool IsA() { return this == &A; }
    bool IsB() { return this == &B; }

    static E current;                // Текущий канал

    static Channel A;
    static Channel B;
    static Channel C;
    static Channel D;

    static Switch *switchTimeMeasue;
    static Switch *switchNumberPeriods;
    static Switch *switchTimeLabels;

    static TimeMeasure       timeMeasure;    // Время счета
    static NumberPeriods     numberPeriods;  // Число периодов измерения
    static PeriodTimeLabels  timeLabels;     // Период меток времени

    // Загрузить текущий канал в аппаратуру
    static void LoadCurrentToFPGA();

    static Page *PageForChannel(Channel::E);

    static bool IsActiveTimeLabels(TypeMeasure *type, int mode);
    static bool IsActiveTimeMeasure(TypeMeasure *type, int mode);
    static bool IsActiveNumberPeriods(TypeMeasure *type, int mode);

    static Channel &Current();

};


extern Page pageChannelA;
extern Page pageChannelB;
extern Page pageChannelC;
extern Page pageChannelD;


struct PageSettingsA
{
    static void FixPress();
    static Switch *switchTypeSynch;     // Указатель на переключатель типа синхронизации
};


struct PageSettingsB
{
    static void FixPress();
    static Switch *switchTypeSynch;     // Указатель на переключатель типа синхронизации
};


struct PageSettingsC
{
    int i;
};

struct PageSettingsD
{
    int i;
};
