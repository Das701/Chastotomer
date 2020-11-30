#include "defines.h"
#include "Settings.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"


// ����� ������� ���������
#ifdef USE_1000s

DEF_SWITCH_7(sTimeMeasure,
    "�����", "����� �����",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    ModesChannel::timeMeasure, TimeMeasure::LoadToFPGA
);

#else

DEF_SWITCH_6(sTimeMeasure,
    "�����", "����� �����",
    "1ms", "10ms", "100ms", "1s", "10s", "100s",
    ModesChannel::timeMeasure, TimeMeasure::LoadToFPGA
);

#endif


Switch *ModesChannel::switchTimeMeasue = &sTimeMeasure;


// ����� ����� ����������� �������� �������� �������
#ifdef USE_1000K

DEF_SWITCH_7(sNumberPeriods,
    "N", "����� �������� ���������",
    "1", "10", "100", "1K", "10K", "100K", "1000K",
    ModesChannel::numberPeriods, NumberPeriods::LoadToFPGA
);

#else

DEF_SWITCH_6(sNumberPeriods,
    "N", "����� �������� ���������",
    "1", "10", "100", "1K", "10K", "100K",
    ModesChannel::numberPeriods, NumberPeriods::LoadToFPGA
);

#endif


Switch *ModesChannel::switchNumberPeriods = &sNumberPeriods;


// ����� ������� ����� �������
DEF_SWITCH_6(sTimeLabels,
    "�����", "������������ ��������� �����",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    ModesChannel::timeLabels, PeriodTimeLabels::LoadToFPGA
);


Switch *ModesChannel::switchTimeLabels = &sTimeLabels;
