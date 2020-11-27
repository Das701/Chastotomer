#include "defines.h"
#include "Settings.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Modes/PagesModes.h"


bool CurrentPageModes::ConsistTimeMeasure()
{
    if (Channel::Current().typeMeasure.IsFrequency())
    {
        if (ModeFrequency::Current().IsFrequency() || ModeFrequency::Current().IsRatioAC() || ModeFrequency::Current().IsRatioBC())
        {
            return true;
        }
    }

    if (Channel::Current().typeMeasure.IsPeriod() && ModePeriod::Current().IsF_1())
    {
        return true;
    }

    return false;
}


// Выбор времени измерения
#ifdef USE_1000s

DEF_SWITCH_7(sTimeMeasure,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    Channel::timeMeasure, TimeMeasure::LoadToFPGA
);

#else

DEF_SWITCH_6(sTimeMeasure,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s",
    Channel::timeMeasure, TimeMeasure::LoadToFPGA
);

#endif


Switch *Channel::switchTimeMeasue = &sTimeMeasure;


// Выбор числа усредняемых периодов входного сигнала
#ifdef USE_1000K

DEF_SWITCH_7(sNumberPeriods,
    "N", "Число периодов измерения",
    "1", "10", "100", "1K", "10K", "100K", "1000K",
    Channel::numberPeriods, NumberPeriods::LoadToFPGA
);

#else

DEF_SWITCH_6(sNumberPeriods,
    "N", "Число периодов измерения",
    "1", "10", "100", "1K", "10K", "100K",
    Channel::numberPeriods, NumberPeriods::LoadToFPGA
);

#endif


Switch *Channel::switchNumberPeriods = &sNumberPeriods;


// Выбор периода меток времени
DEF_SWITCH_6(sTimeLabels,
    "Метки", "Длительность временных меток",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    Channel::timeLabels, PeriodTimeLabels::LoadToFPGA
);


Switch *Channel::switchTimeLabels = &sTimeLabels;


static void DrawValue(Enumeration &param, int x, int y)
{
    int width = 60;
    Primitives::Rectangle(width, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::WHITE);
    Text(param.ToString()).Write(x + 2, y + 7, width, Color::WHITE);
}


void PageModes::DrawParameters(int x, int y)
{
    Page &page = *Current();

    TypeMeasure *type = page.GetTypeMeasure();

    int mode = page.GetModeMeasure();

    int dX = 70;

    if (Channel::IsActiveTimeMeasure(type, mode))
    {
        DrawValue(Channel::timeMeasure, x, y);
    }

    if (Channel::IsActiveNumberPeriods(type, mode))
    {
        DrawValue(Channel::numberPeriods, x + dX, y);
    }

    if (Channel::IsActiveTimeLabels(type, mode))
    {
        DrawValue(Channel::timeLabels, x + 2 * dX, y);
    }
}


void PageModes::OnChanged_TypeMeasure()
{
    if (CURRENT_CHANNEL_IS_A)
    {
        PageModesA::OnChanged_TypeMeasure();
    }
    else if (CURRENT_CHANNEL_IS_B)
    {
        PageModesB::OnChanged_TypeMeasure();
    }
    else if (CURRENT_CHANNEL_IS_C)
    {
        PageModesC::OnChanged_TypeMeasure();
    }
}
