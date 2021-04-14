#include "defines.h"
#include "Settings.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Channels/Channels.h"
#include <cstring>


using namespace Primitives;


extern Switch sSync;
extern Item *items[7];


static void OnPress_Couple()
{
    InputCouple::LoadToFPGA();
}

// ����� ���������(���������) ����� �������� ������
DEF_SWITCH_UGO_2(sCouple,
    "����", "����",
    "�������� ����", "�������� ����",
    SYMBOL_DC_IN_QUOTES, SYMBOL_AC_IN_QUOTES,
    Channel::A->set.couple, OnPress_Couple
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
    Channel::A->set.impedance, OnPress_Impedance
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
    Channel::A->set.modeFilter, OnPress_Filter
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
    Channel::A->set.modeFront, OnPress_Front
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
    Channel::A->set.divider, OnPress_Divider
);


static void OnPress_Sync()
{
    if (Channel::A->set.typeSynch.IsHoldoff())
    {
        items[0] = &sSync;
        items[1] = ModesChannel::switchTimeMeasue;
        items[2] = ModesChannel::switchTimeLabels;
        items[3] = nullptr;

        Channel::A->pageSettings->selectedItem = 0;
    }
    else if(Channel::A->set.typeSynch.IsManual()) //-V2516
    {
        items[0] = &sCouple;
        items[1] = &sImpedance;
        items[2] = &sLowpassFilter;
        items[3] = &sFront;
        items[4] = &sDivider;
        items[5] = &sSync;
        items[6] = nullptr;
        
        Channel::A->pageSettings->selectedItem = 5;
    }

    TYPE_SYNCH_A = (TypeSynch::E)Channel::A->set.typeSynch.value;

    TypeSynch::LoadToFPGA();
}

// ����� ������ ������������� ���, ���
DEF_SWITCH_UGO_2(sSync,
    "�����", "����� ������ ������������",
    "����", "Holdoff",
    "����", "Holdoff",
    Channel::A->set.typeSynch, OnPress_Sync
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

Page pageChannelA(items, nullptr);
