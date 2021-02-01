#pragma once
#include "Hardware/HAL/HAL.h"
#include "Menu/MenuItems.h"


struct TypeMeasure : public Enumeration
{
    enum E
    {
        /* A B C D */  Frequency,      // Измерение частоты
        /* A B     */  Period,         // Измерение периода
        /* A B     */  Duration,       // Измерение длительности
        /* A B C   */  CountPulse,     // Подсчёт импульсов
                       Count
    };

    explicit TypeMeasure(E v, const bool *correct, int numStates) : Enumeration((uint8)v, correct, numStates) {};
    bool IsFrequency() const { return (value == Frequency); }
    bool IsCountPulse() const { return (value == CountPulse); }
    bool IsPeriod() const { return (value == Period); }
    bool IsDuration() const { return (value == Duration); }
    void Draw(int x, int y) const;

    bool IsActiveTimeLabels(int mode);
    bool IsActiveTimeMeasure(int mode);
    bool IsActiveNumberPeriods(int mode);
    bool IsActiveTimeComparator(int mode);
};

// Режим измерения частоты
struct ModeFrequency : public Enumeration
{
    enum E
    {
        /* A B C D */  Frequency,  // Частота
        /* A B     */  T_1,        // f = 1 / T
        /* A       */  RatioAB,    // f(A) / f(B)
        /* A       */  RatioAC,    // f(A) / f(C)
        /*   B     */  RatioBA,    // f(B) / f(A)
        /*   B     */  RatioBC,    // f(B) / f(C)
        /*     C   */  RatioCA,    // f(C) / f(A)
        /*     C   */  RatioCB,    // f(C) / f(B)
        /* A B     */  Tachometer, // Тахометр
        /* A       */  Comparator,
                       Count
    };

    explicit ModeFrequency(E v, const bool *correct, int numStates) : Enumeration((uint8)v, correct, numStates) {};

    bool IsFrequency()  const { return (value == Frequency); }
    bool IsRatioAB()    const { return (value == RatioAB); }
    bool IsRatioAC()    const { return (value == RatioAC); }
    bool IsRatioBA()    const { return (value == RatioBA); }
    bool IsRatioBC()    const { return (value == RatioBC); }
    bool IsRatioCA()    const { return (value == RatioCA); }
    bool IsRatioCB()    const { return (value == RatioCB); }
    bool IsRatio()      const;
    bool IsT_1()        const { return (value == T_1); }
    bool IsTachometer() const { return (value == Tachometer); }
    bool IsComparator() const { return (value == Comparator); }

    static const ModeFrequency &Current();
    static void LoadToFPGA();
};


// Режим счёта импульсов
struct ModeCountPulse : public Enumeration
{
    enum E
    {
        /* A     */ AtB,      // A(tB)
        /* A     */ ATB,      // A(TB)
        /*   B   */ BtA,      // B(tA)
        /*   B   */ BTA,      // B(TA)
        /*     C */ CtA,      // C(tA)
        /*     C */ CtB,      // C(tB)
        /*     C */ CTA,      // C(TA)
        /*     C */ CTB,      // C(TB)
        /* A B   */ StartStop,
        Count
    };

    explicit ModeCountPulse(E v, const bool *correct, int numStates) : Enumeration((uint8)v, correct, numStates) {};
    // Возвращает true, если включён режим измерения по периоду (большая T в названии)
    bool IsFromPeriod() const;
    bool Is_AtB() const { return (value == AtB); }
    bool Is_ATB() const { return (value == ATB); }
    bool Is_BtA() const { return (value == BtA); }
    bool Is_BTA() const { return (value == BTA); }
    bool Is_CtA() const { return (value == CtA); }
    bool Is_CtB() const { return (value == CtB); }
    bool Is_CTA() const { return (value == CTA); }
    bool Is_CTB() const { return (value == CTB); }
    bool Is_StartStop() const { return (value == StartStop); }

    static const ModeCountPulse &Current();
    static void LoadToFPGA();
};


// Режим измерения периода
struct ModePeriod : public Enumeration
{
    enum E
    {
        Period,     // Период
        F_1,        // T = 1 / f
        Count
    };

    explicit ModePeriod(E v) : Enumeration((uint8)v) {};

    bool IsPeriod() const { return (value == Period); }
    bool IsF_1()    const { return (value == F_1); }

    static const ModePeriod &Current();
    static void LoadToFPGA();
};

// Режим измерения длительности
struct ModeDuration : public Enumeration
{
    enum E
    {
        Ndt,        // ndt
        Ndt_1ns,    // ndt_1нс
        StartStop,  // ndt2
        FillFactor, // Коэффициет заполнения
        Phase,      // Фаза
        Count
    };

    explicit ModeDuration(E v) : Enumeration((uint8)v) {};

    bool IsNdt() const        { return (value == Ndt); }
    bool IsNdt_1ns() const    { return (value == Ndt_1ns); }
    bool IsStartStop() const  { return (value == StartStop); }
    bool IsFillFactor() const { return (value == FillFactor); }
    bool IsPhase() const      { return (value == Phase); }

    static const ModeDuration &Current();
    static void LoadToFPGA();
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
    int ToZeros() const;
    bool IsT_3() const { return (value == T_3); }
    bool IsT_4() const { return (value == T_4); }
    bool IsT_5() const { return (value == T_5); }
    bool IsT_6() const { return (value == T_6); }
    bool IsT_7() const { return (value == T_7); }
    bool IsT_8() const { return (value == T_8); }

    static void LoadToFPGA();

    static void Set(E v);

    static const PeriodTimeLabels &Current();
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

    static void LoadToFPGA();

    static void Set(E v);

    static const NumberPeriods &Current();
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
        _100s,      // 100s
        _1000s,
        Count
    };

    explicit TimeMeasure(E v) : Enumeration((uint8)v) {};

    int ToMS() const;

    bool Is10ms() const { return (value == _10ms); }

    static void LoadToFPGA();

    static void Set(E v);

    static const TimeMeasure &Current();
};


// Время измерения компаратора
struct TimeComparator : public Enumeration
{
    enum E
    {
        _1s,
        _10s,
        Count
    };

    explicit TimeComparator(E v) : Enumeration((uint8)v) {};

    static void LoadToFPGA();

    bool Is_1s()  const { return (value == _1s); }
    bool Is_10s() const { return (value == _10s); }
};


struct CurrentModeMeasure
{
    static Enumeration &ToEnumeration();
    static void Draw(int x, int y);
};


// Открытый/закрытый вход
struct InputCouple : public Enumeration
{
    enum E
    {
        AC,         // Открытый
        DC,         // Закрытый
        Count
    };

    explicit InputCouple(E v) : Enumeration((uint8)v) {};
    bool IsDC() const { return (value == InputCouple::DC); }

    void Set(E v);

    static void LoadToFPGA();
};


// Входное сопротивление
struct InputImpedance : public Enumeration
{
    enum E
    {
        _1MOmh,         // 1 МОм
        _50Omh,         // 50 Ом
        Count
    };

    explicit InputImpedance(E v) : Enumeration((uint8)v) {};

    bool Is_1MOhm() const { return (value == _1MOmh); }
    bool Is_50Ohm() const { return (value == _50Omh); }

    static void LoadToFPGA();
};


// ФНЧ
struct ModeFilter : public Enumeration
{
    enum E
    {
        On,             // Включен
        Off,            // Выключен
        Count
    };

    explicit ModeFilter(E v) : Enumeration((uint8)v) {};
    bool IsOff() const { return (value == ModeFilter::Off); }

    void Set(E v);

    static void LoadToFPGA();
};


// Фронт синхронизации
struct ModeFront : public Enumeration
{
    enum E
    {
        Front,          // Фронт
        Slice,          // Срез
        Count
    };

    explicit ModeFront(E v) : Enumeration((uint8)v) {};
    bool IsFront() const { return (value == Front); }

    static void LoadToFPGA();
};


// Выбор типа синхронизации - ТТЛ или ЭСЛ
struct TypeSynch : public Enumeration
{
    enum E
    {
        Manual,         // Ручн
        Holdoff,        // Holdoff
        Count
    };

    explicit TypeSynch(E v) : Enumeration((uint8)v) {};
    bool IsHoldoff() const { return (value == Holdoff); }
    bool IsManual() const { return (value == Manual); }

    static void LoadToFPGA();
};


// Делитель напряжения
struct Divider : public Enumeration
{
    enum E
    {
        _1,           // 1:1
        _10,          // 1:10
        Count
    };

    explicit Divider(E v) : Enumeration((uint8)v) {};
    bool Is1() const { return (value == _1); }
    int ToAbs() const { return Is1() ? 1 : 10; }

    static void LoadToFPGA();
};


struct ModeStartStop
{
    static void Toggle();
    static bool IsEnabled() { return enabled; }
    static void LoadToFPGA();
private:
    static bool enabled;
};


struct Relation
{
    static void On();
    static void Off();
    static bool IsEnabled();
private:
    static bool enabled;
};
