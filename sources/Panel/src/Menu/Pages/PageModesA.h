#pragma once
#include "Menu/MenuItems.h"


struct CurrentTypeMeasure
{
    static bool IsFrequency();
    static bool IsPeriod();
    static bool IsDuration();
    static bool IsCountPulse();
};

struct CurrentModeMeasureFrequency
{
    static bool IsTachometer();
    static bool IsComparator();
    static bool IsT_1();
    static bool Is_RatioAC_or_RatioBC();
    static bool Is_RatioAB_or_RatioBA();
    static bool Is_RatioCA_or_RatioCB();
};


struct CurrentModeMeasureCountPulse
{
    static bool IsBig_T();
};


struct TypeMeasureAB : public Enumeration
{
    enum E
    {
        Frequency,      // Измерение частоты
        Period,         // Измерение периода
        Duration,       // Измерение длительности
        CountPulse,     // Подсчёт импульсов
        Count
    };

    explicit TypeMeasureAB(E v) : Enumeration((uint8)v) {};
    bool IsFrequency() const  { return (value == Frequency); }
    bool IsCountPulse() const { return (value == CountPulse); }
    bool IsPeriod() const     { return (value == Period); }
    bool IsDuration() const   { return (value == Duration); }
};

// Режим измерения частоты
struct ModeMeasureFrequencyA : public Enumeration
{
    enum E
    {
        Frequency,  // Частота
        RatioAB,    // f(A) / f(B)
        RatioAC,    // f(A) / f(C)
        T_1,        // f = 1 / T
        Tachometer, // Тахометр
        Comparator
    };

    explicit ModeMeasureFrequencyA(E v) : Enumeration((uint8)v) {};

    bool IsFrequency()  const { return (value == Frequency); }
    bool IsRatioAB()    const { return (value == RatioAB); }
    bool IsRatioAC()    const { return (value == RatioAC); }
    bool IsT_1()        const { return (value == T_1); }
    bool IsTachometer() const { return (value == Tachometer); }
    bool IsComparator() const { return (value == Comparator); }
};

// Режим измерения периода
struct ModeMeasurePeriod : public Enumeration
{
    enum E
    {
        Period,     // Период
        F_1         // T = 1 / f
    };

    explicit ModeMeasurePeriod(E v) : Enumeration((uint8)v) {};

    bool IsPeriod() const { return (value == Period); }
    bool IsF_1()    const { return (value == F_1); }
    static ModeMeasurePeriod &Current();
};

// Режим измерения длительности
struct ModeMeasureDuration : public Enumeration
{
    enum E
    {
        Ndt,        // ndt
        Ndt_1ns,    // ndt_1нс
        Ndt2,       // ndt2
        Dcycle,     // Скважность
        Phase       // Фаза
    };

    explicit ModeMeasureDuration(E v) : Enumeration((uint8)v) {};

    bool Is_Ndt() const     { return (value == Ndt); }
    bool Is_Ndt_1ns() const { return (value == Ndt_1ns); }
    bool Is_Ndt2() const    { return (value == Ndt2); }
    bool Is_Dcycle() const  { return (value == Dcycle); }
    bool Is_Phase() const   { return (value == Phase); }

    static ModeMeasureDuration &Current();
};

// Режим счёта импульсов
struct ModeMeasureCountPulseA : public Enumeration
{
    enum E
    {
        AtC,        // A(tC)
        ATB,        // A(TB)
        StartStop
    };

    explicit ModeMeasureCountPulseA(E v) : Enumeration((uint8)v) {};
};

// Период меток времени
struct PeriodTimeLabels : public Enumeration
{
    enum E
    {
        T_3,        // 10-3
        T_4,        // 10-4
        T_5,        // 10-5
        T_6,        // 10-6
        T_7,        // 10-7
        T_8,        // 10-8
        Count
    };

    explicit PeriodTimeLabels(E v) : Enumeration((uint8)v) {};
    // Возвращает PeriodTimeLabels для текущего канала
    static PeriodTimeLabels &Current();
    int ToZeros() const;
    bool IsT_3() const { return (value == T_3); }
    bool IsT_4() const { return (value == T_4); }
    bool IsT_5() const { return (value == T_5); }
    bool IsT_6() const { return (value == T_6); }
    bool IsT_7() const { return (value == T_7); }
    bool IsT_8() const { return (value == T_8); }
};

// Время счёта
struct TimeMeasure : public Enumeration
{
    enum E
    {
        _1ms,       // 1ms
        _10ms,      // 10ms
        _100ms,     // 100ms
        _1s,        // 1s
        _10s,       // 10s
        _100s,       // 100s
        _1000s,
        Count
    };

    explicit TimeMeasure(E v) : Enumeration((uint8)v) {};
    int ToMS() const;
    bool Is10ms() const { return (value == _10ms); }
    // Возвращает TimeMeasure::E для текущего канала
    static TimeMeasure &Current();
};

// Число периодов
struct NumberPeriods : public Enumeration
{
    enum E
    {
        _1,         // 1
        _10,        // 10
        _100,       // 100
        _1K,        // 1K
        _10K,       // 10K
        _100K,      // 100K
        _1000K,
        Count
    };

    explicit NumberPeriods(E v) : Enumeration((uint8)v) {};
    int ToAbs() const;
    // Возвращает NumberPeriods для текущего канала
    static NumberPeriods &Current();
};


// Здесь функции общие для всех страниц режимов каналов
struct PageModes
{
    // Сброс режима текущего измерения
    static void ResetModeCurrentMeasure();
};


struct PageModesA
{
    static Page *self;
    
    static void Init();

    static void InterpolateOn();
    static void InterpolateOff();
    static bool InterpolateCheck();

    static void DCycleOn();
    static void DCycleOff();
    static bool DCycleCheck();

    static void RelationOn();
    static void RelationOff();
    static bool RelationCheck();

    static void PressSetup();
    static bool StartStop();
    static void ToggleStartStop();

    // Сброс режима измерения
    static void ResetModeCurrentMeasure();

    // Вызывается при измеенении вида измерения
    static void OnChanged_TypeMeasure();

    // Вызываются при изменении режимов измерения
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModePeriod();
    static void OnChanged_ModeDuration();
    static void OnChanged_ModeCountPulse();
    
    static TypeMeasureAB            typeMeasure;                // Тип измерения
    static ModeMeasureFrequencyA    modeMeasureFrequency;       // Режим измерения частоты
    static ModeMeasurePeriod        modeMeasurePeriod;          // Режим измерения периода
    static ModeMeasureDuration      modeMeasureDuration;        // Режим измерения длительности
    static ModeMeasureCountPulseA   modeMeasureCountPulse;      // Режим счёта импульсов
    static PeriodTimeLabels         periodTimeLabels;           // Период меток времени
    static TimeMeasure              timeMeasure;                // Время счета
    static NumberPeriods            numberPeriods;              // Число периодов измерения
};
