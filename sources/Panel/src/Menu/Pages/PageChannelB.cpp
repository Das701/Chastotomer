#include "defines.h"
#include "PageChannelB.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"
#include "FreqMeter/FreqMeter.h"
#include "Settings.h"
#include <cstring>


using namespace Display::Primitives;
using namespace Display;

extern Item *items[7];


InputCouple      PageChannelB::couple(InputCouple::DC);
InputImpedance   PageChannelB::impedance(InputImpedance::_1MOmh);
ModeFilter       PageChannelB::modeFilter(ModeFilter::Off);
ModeFront        PageChannelB::modeFront(ModeFront::Front);
Divider          PageChannelB::divider(Divider::_1);
TypeSynch        PageChannelB::typeSynch(TypeSynch::TTL);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Couple()
{
    FreqMeter::LoadInputCouple();
}

/// ����� ���������(���������) ����� �������� ������
DEF_SWITCH_UGO_2(sCouple,
    "\x7C \x7D", "����",
    "�������� ����", "�������� ����",
    "\x7C", "\x7D",
    PageChannelB::couple, OnPress_Couple
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Impedance()
{
    FreqMeter::LoadImpedance();
}

/// ��������� �������� ������������� �������� ������
DEF_SWITCH_UGO_2(sImpedance,
    "R��", "������� ������������� ������",
    "1 ���", "50 ��",
    "1���", "50��",
    PageChannelB::impedance, OnPress_Impedance
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Filter()
{
    FreqMeter::LoadModeFilter();
}

/// ���������(����������) ���
DEF_SWITCH_UGO_2(sLowpassFilter,
    "���", "���������/���������� ������� ��",
    "����.", "���.",
    "���", "",
    PageChannelB::modeFilter, OnPress_Filter
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Front()
{
    FreqMeter::LoadModeFront();
}

/// ����� ������ ������������� �������� ������
DEF_SWITCH_UGO_2(sFront,
    "\x8D \x8E", "����� ���� �������������",
    "�����", "����",
    "\x8D", "\x8E",
    PageChannelB::modeFront, OnPress_Front
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Divider()
{
    FreqMeter::LoadDivider();
}

/// ���������(����������) �������� �������� ����������
DEF_SWITCH_UGO_2(sDivider,
    "\x7E\x7F\x7E \x7E\x7F\x8F", "���/���� �������� ��������",
    "1:1", "1:10",
    "", "1:10",
    PageChannelB::divider, OnPress_Divider
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Sync()
{
    TYPE_SYNCH_B = (TypeSynch::E)PageChannelB::typeSynch.value;
    FreqMeter::LoadTypeSynch();
}

/// ����� ������ ������������� ���, ���
DEF_SWITCH_UGO_2(sSync,
    "�����", "����� ������ ������������",
    "���", "���",
    "���", "���",
    PageChannelB::typeSynch, OnPress_Sync
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

static Page pageChannelB(items);

Page *PageChannelB::self = &pageChannelB;

Switch *PageChannelB::switchTypeSynch = &sSync;
