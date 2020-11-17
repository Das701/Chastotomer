#include "defines.h"
#include "FreqMeter.h"
#include "Settings.h"
#include "Menu/Pages/Settings/PagesSettings.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include <cstring>


using namespace Primitives;


extern Switch sSync;
extern Item *items[7];

InputCouple     PageSettingsA::couple(InputCouple::AC);
InputImpedance  PageSettingsA::impedance(InputImpedance::_1MOmh);
ModeFilter      PageSettingsA::modeFilter(ModeFilter::Off);
ModeFront       PageSettingsA::modeFront(ModeFront::Front);
Divider         PageSettingsA::divider(Divider::_1);
TypeSynch       PageSettingsA::typeSynch(TypeSynch::Manual);


static void OnPress_Couple()
{
    InputCouple::Load();
}

// ����� ���������(���������) ����� �������� ������
DEF_SWITCH_UGO_2(sCouple,
    "����", "����",
    "�������� ����", "�������� ����",
    SYMBOL_DC_IN_QUOTES, SYMBOL_AC_IN_QUOTES,
    PageSettingsA::couple, OnPress_Couple
);


static void OnPress_Impedance()
{
    InputImpedance::LoadToFPGA();
}

// ��������� �������� ������������� �������� ������
DEF_SWITCH_UGO_2(sImpedance,
    "R��", "������� ������������� ������",
    "1 ���", "50 ��",
    "1���", "50��",
    PageSettingsA::impedance, OnPress_Impedance
);


static void OnPress_Filter()
{
    ModeFilter::LoadToFPGA();
}

// ���������(����������) ���
DEF_SWITCH_UGO_2(sLowpassFilter,
    "���", "���������/���������� ������� ��",
    "����.", "���.",
    "���", "",
    PageSettingsA::modeFilter, OnPress_Filter
);


static void OnPress_Front()
{
    ModeFront::LoadToFPGA();
}


// ����� ������ ������������� �������� ������
DEF_SWITCH_UGO_2(sFront,
    "\x82/\x83", "����� ���� �������������",
    "�����", "����",
    SYMBOL_FRONT_IN_QUOTES, SYMBOL_RISE_IN_QUOTES,
    PageSettingsA::modeFront, OnPress_Front
);


static void OnPress_Divider()
{
    Divider::LoadToFPGA();
}

// ���������(����������) �������� �������� ����������
DEF_SWITCH_UGO_2(sDivider,
    "��������", "���/���� �������� ��������",
    "1:1", "1:10",
    "1:1", "1:10",
    PageSettingsA::divider, OnPress_Divider
);


// ����� ������� ����� �������
DEF_SWITCH_6(sPeriodTimeLabels,
    "�����", "������������ ��������� �����",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModes::timeLabels,  PeriodTimeLabels::LoadToFPGA
);


// ����� ������� ���������
#ifdef USE_1000s

DEF_SWITCH_7(sTimeMeasure,
    "�����", "����� �����",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModes::timeMeasure, TimeMeasure::LoadToFPGA
);

#else

DEF_SWITCH_6(sTimeMeasure,
    "�����", "����� �����",
    "1ms", "10ms", "100ms", "1s", "10s", "100s",
    PageModes::timeMeasure, TimeMeasure::LoadToFPGA
);

#endif


static void OnPress_Sync()
{
    if (PageSettingsA::typeSynch.IsHoldoff())
    {
        items[0] = &sSync;
        items[1] = &sTimeMeasure;
        items[2] = &sPeriodTimeLabels;
        items[3] = nullptr;

        PageSettingsA::self->selectedItem = 0;
    }
    else if(PageSettingsA::typeSynch.IsManual())
    {
        items[0] = &sCouple;
        items[1] = &sImpedance;
        items[2] = &sLowpassFilter;
        items[3] = &sFront;
        items[4] = &sDivider;
        items[5] = &sSync;
        items[6] = nullptr;
        
        PageSettingsA::self->selectedItem = 5;
    }

    TYPE_SYNCH_A = (TypeSynch::E)PageSettingsA::typeSynch.value;

    TypeSynch::LoadToFPGA();
}

// ����� ������ ������������� ���, ���
DEF_SWITCH_UGO_2(sSync,
    "�����", "����� ������ ������������",
    "����", "Holdoff",
    "����", "Holdoff",
    PageSettingsA::typeSynch, OnPress_Sync
);

static Item *items[7] =
{
    &sCouple,
    &sImpedance,
    &sLowpassFilter,
    &sFront,
    &sDivider,
    &sSync,
    nullptr
};

static Page pageChannelA(items, nullptr);

Page *PageSettingsA::self = &pageChannelA;

Switch *PageSettingsA::switchTypeSynch = &sSync;

void PageSettingsA::FixPress()
{
    OnPress_Sync();
}


InputImpedance &InputImpedance::Current()
{
    return CURRENT_CHANNEL_IS_A ? PageSettingsA::impedance : PageSettingsB::impedance;
}


ModeFront &ModeFront::Current()
{
    return CURRENT_CHANNEL_IS_A ? PageSettingsA::modeFront : PageSettingsB::modeFront;
}


Divider &Divider::Current()
{
    return CURRENT_CHANNEL_IS_A ? PageSettingsA::divider : PageSettingsB::divider;
}
