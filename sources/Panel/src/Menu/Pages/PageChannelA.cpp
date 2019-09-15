#include "defines.h"
#include "PageChannelA.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"
#include "Settings.h"
#include "FreqMeter/FreqMeter.h"
#include <cstring>

using namespace Display::Primitives;
using namespace Display;

extern Item *items[7];

InputCouple     PageChannelA::inputCouple(InputCouple::DC);
InputImpedance  PageChannelA::inputImpedance(InputImpedance::_1MOmh);
ModeFilter      PageChannelA::modeFilter(ModeFilter::Off);
ModeFront       PageChannelA::modeFront(ModeFront::Front);
Divider         PageChannelA::divider(Divider::_1);
TypeSynch       PageChannelA::typeSynch(TypeSynch::TTL);

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
    PageChannelA::inputCouple, OnPress_Couple
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
    PageChannelA::inputImpedance, OnPress_Impedance
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
    PageChannelA::modeFilter, OnPress_Filter
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
    PageChannelA::modeFront, OnPress_Front
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
    PageChannelA::divider, OnPress_Divider
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Sync()
{
    TYPE_SYNCH_A = (TypeSynch::E)PageChannelA::typeSynch.value;
    FreqMeter::LoadTypeSynch();
}

/// ����� ������ ������������� ���, ���
DEF_SWITCH_UGO_2(sSync,
    "�����", "����� ������ ������������",
    "���", "���",
    "���", "���",
    PageChannelA::typeSynch, OnPress_Sync
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

static Page pageChannelA(items);

Page *PageChannelA::self = &pageChannelA;

Switch *PageChannelA::switchTypeSynch = &sSync;
