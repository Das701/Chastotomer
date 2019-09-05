#include "defines.h"
#include "PageChannelB.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"
#include <cstring>

using namespace Display::Primitives;
using namespace Display;

extern Item *items[];


ChannelInputB    PageChannelB::channelInput(ChannelInputB::Closed);
InputImpedanceB  PageChannelB::inputImpedance(InputImpedanceB::_1MOmh);
ModeFilterB      PageChannelB::modeFilter(ModeFilterB::Off);
ModeFrontB       PageChannelB::modeFront(ModeFrontB::Front);
DividerB         PageChannelB::divider(DividerB::_1);
LevelSynchB      PageChannelB::levelSynch(LevelSynchB::TTL);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_OCI()
{
}

/// ����� ���������(���������) ����� �������� ������
DEF_SWITCH_UGO_2(sInputChoice,
    "���", "����",
    "����. ����", "����. ����",
    "��", "��",
    PageChannelB::channelInput, OnPress_OCI
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Impedance()
{
}

/// ��������� �������� ������������� �������� ������
DEF_SWITCH_UGO_2(sImpedance,
    "R��", "������� ������������� �������� ������",
    "1 ���", "50 ��",
    "1���", "50��",
    PageChannelB::inputImpedance, OnPress_Impedance
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Filter()
{
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
}

/// ����� ������ ������������� �������� ������
DEF_SWITCH_UGO_2(sFront,
    "�����", "����� ���� �������������",
    "�����", "����",
    "�����", "����",
    PageChannelB::modeFront, OnPress_Front
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_VD()
{
}

/// ���������(����������) �������� �������� ����������
DEF_SWITCH_UGO_2(sDivider,
    "1/1 1/10", "���������/���������� �������� �������� ����������",
    "1:1", "1:10",
    "", "1:10",
    PageChannelB::divider, OnPress_VD
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Sync()
{
}

/// ����� ������ ������������� ���, ���
DEF_SWITCH_UGO_2(sSync,
    "�����", "����� ������ ������������",
    "���", "���",
    "1200mV", "-1240mV",
    PageChannelB::levelSynch, OnPress_Sync
);

static Item *items[] =
{
    &sInputChoice,
    &sImpedance,
    &sLowpassFilter,
    &sFront,
    &sDivider,
    &sSync,
    nullptr
};

static Page pageChannelB(items);

Page *PageChannelB::self = &pageChannelB;
