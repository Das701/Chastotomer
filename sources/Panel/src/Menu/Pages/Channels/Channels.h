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

    Channel(Page *pSettings, Page *pModes, const bool *enabledMeasures, const bool *enabledModeFrequency, const bool *enabledModeCountPulse);

    Page *pageSettings;
    Page *pageModes;

    InputCouple    couple;
    InputImpedance impedance;
    ModeFilter     modeFilter;
    ModeFront      modeFront;
    Divider        divider;
    TypeSynch      typeSynch;

    TypeMeasure    typeMeasure;
    ModeFrequency  modeFrequency;           // Режим измерения частоты
    ModePeriod     modePeriod;              // Режим измерения периода
    ModeDuration   modeDuration;            // Режим измерения длительности
    ModeCountPulse modeMeasureCountPulse;   // Режим счёта импульсов

    // Возвращает true, если текущая страница режимов содержит время измерения
    bool ConsistTimeMeasure();

    void DrawParameters(int x, int y);

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

    // Вызывается при измеенении вида измерения
    static void OnChanged_TypeMeasure();

    // Вызываются при изменении режимов измерения
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModeFrequencyA();
    static void OnChanged_ModeFrequencyB();
    static void OnChanged_ModeFrequencyC();

    static void OnChanged_ModeCountPulse();
    static void OnChanged_ModeCountPulseA();
    static void OnChanged_ModeCountPulseB();
    static void OnChanged_ModeCountPulseC();

    static void OnChanged_ModePeriod();
    static void OnChanged_ModePeriodA();
    static void OnChanged_ModePeriodB();

    static void OnChanged_ModeDuration();
    static void OnChanged_ModeDurationA();
    static void OnChanged_ModeDurationB();

    static void PressSetup();
    static void PressSetupA();
    static void PressSetupB();

    static void RelationOn();
    static void RelationOff();
    static bool RelationCheck();
};
