#include "defines.h"
#include "PageModes.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"


using namespace Display::Primitives;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnDraw_Frequency(int x, int y)
{
    Point().Draw(x, y);
}

/// Выбор режима измерения частоты, отношения частот, "тахометра"
static Button bFrequency(OnDraw_Frequency);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnDraw_Period(int x, int y)
{
    for (int i = 0; i < 2; i++)
    {
        Point().Draw(x + i * 2, y);
    }
}

/// Выбор режима измерения периода
static Button bPeriod(OnDraw_Period);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnDraw_Duration(int x, int y)
{
    for (int i = 0; i < 3; i++)
    {
        Point().Draw(x + i * 3, y);
    }
}

/// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
static Button bDuration(OnDraw_Duration);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnDraw_CountPulse(int x, int y)
{
    for (int i = 0; i < 4; i++)
    {
        Point().Draw(x + i * 3, y);
    }
}

/// Выбор режима счёта импульсов
static Button bCountPulse(OnDraw_CountPulse);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnDraw_PeriodTimeLabels(int x, int y)
{
    for (int i = 0; i < 5; i++)
    {
        Point().Draw(x + i * 3, y);
    }
}

/// Выбор периода меток времени
static Button bPeriodTimeLabels(OnDraw_PeriodTimeLabels);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnDraw_TimeMeasure(int x, int y)
{
    for (int i = 0; i < 6; i++)
    {
        Point().Draw(x + i * 3, y);
    }
}

/// Выбор времени измерения
static Button bTimeMeasure(OnDraw_TimeMeasure);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnDraw_NumberPeriods(int x, int y)
{
    for (int i = 0; i < 7; i++)
    {
        Point().Draw(x + i * 3, y);
    }
}

/// Выбор числа усредняемых периодов входного сигнала
static Button bNumberPeriods(OnDraw_NumberPeriods);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static Item *items[] =
{
    &bFrequency,
    &bPeriod,
    &bDuration,
    &bCountPulse,
    &bPeriodTimeLabels,
    &bTimeMeasure,
    nullptr
};

static Page pageModes(items);

Page *PageModes::self = &pageModes;
