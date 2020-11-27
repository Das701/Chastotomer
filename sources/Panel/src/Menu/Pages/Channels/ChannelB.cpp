#include "defines.h"
#include "Settings.h"
#include "Display/Symbols.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"
#include <cstring>


using namespace Primitives;


extern Switch sSync;
extern Item *items[7];


InputCouple      PageSettingsB::couple(InputCouple::AC);
InputImpedance   PageSettingsB::impedance(InputImpedance::_1MOmh);
ModeFilter       PageSettingsB::modeFilter(ModeFilter::Off);
ModeFront        PageSettingsB::modeFront(ModeFront::Front);
Divider          PageSettingsB::divider(Divider::_1);
TypeSynch        PageSettingsB::typeSynch(TypeSynch::Manual);


static void OnPress_Couple()
{
    InputCouple::LoadToFPGA();
}

// ����� ���������(���������) ����� �������� ������
DEF_SWITCH_UGO_2(sCouple,
    "����", "����",
    "�������� ����", "�������� ����",
    SYMBOL_DC_IN_QUOTES, SYMBOL_AC_IN_QUOTES,
    PageSettingsB::couple, OnPress_Couple
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
    PageSettingsB::impedance, OnPress_Impedance
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
    PageSettingsB::modeFilter, OnPress_Filter
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
    PageSettingsB::modeFront, OnPress_Front
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
    PageSettingsB::divider, OnPress_Divider
);


// ����� ������� ����� �������
DEF_SWITCH_6(sPeriodTimeLabels,
    "�����", "������������ ��������� �����",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    Channel::timeLabels,  PeriodTimeLabels::LoadToFPGA
);


static void OnPress_Sync()
{
    if (PageSettingsB::typeSynch.IsHoldoff())
    {
        items[0] = &sSync;
        items[1] = Channel::switchTimeMeasue;
        items[2] = &sPeriodTimeLabels;
        items[3] = nullptr;
    }
    else if(PageSettingsB::typeSynch.IsManual())
    {
        items[0] = &sCouple;
        items[1] = &sImpedance;
        items[2] = &sLowpassFilter;
        items[3] = &sFront;
        items[4] = &sDivider;
        items[5] = &sSync;
        items[6] = nullptr;
    }
    TYPE_SYNCH_B = (TypeSynch::E)PageSettingsB::typeSynch.value;

    TypeSynch::LoadToFPGA();
}

// ����� ������ ������������� ���, ���
DEF_SWITCH_UGO_2(sSync,
    "�����", "����� ������ ������������",
    "����", "Holdoff",
    "����", "Holdoff",
    PageSettingsB::typeSynch, OnPress_Sync
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

static Page pageChannelB(items, nullptr);

Page *PageSettingsB::self = &pageChannelB;

Switch *PageSettingsB::switchTypeSynch = &sSync;

void PageSettingsB::FixPress()
{
    OnPress_Sync();
}