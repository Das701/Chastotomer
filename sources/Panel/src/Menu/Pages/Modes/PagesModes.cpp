#include "defines.h"
#include "Settings.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Modes/PagesModes.h"


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
