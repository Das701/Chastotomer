#include "defines.h"
#include "PageChannelA.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"
#include <cstring>

using namespace Display::Primitives;
using namespace Display;

extern Item *items[];


ChannelInput    PageChannelA::channelInput(ChannelInput::Closed);
InputImpedance  PageChannelA::inputImpedance(InputImpedance::_1MOmh);
ModeFilter      PageChannelA::modeFilter(ModeFilter::Off);
ModeFront       PageChannelA::modeFront(ModeFront::Front);
Divider         PageChannelA::divider(Divider::_1);
LevelSynch      PageChannelA::levelSynch(LevelSynch::TTL);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_OCI()
{
}

/// ����� ���������(���������) ����� �������� ������
DEF_SWITCH_UGO_2(sInputChoice,
    "���", "����",
    "����. ����", "����. ����",
    "�� ", "�� ",
    PageChannelA::channelInput, OnPress_OCI
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Impedance()
{
}

/// ��������� �������� ������������� �������� ������
DEF_SWITCH_UGO_2(sImpedance,
    "R��", "������� ������������� �������� ������",
    "1 ���", "50 ��",
    "1��� ", "50�� ",
    PageChannelA::inputImpedance, OnPress_Impedance
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Filter()
{
}

/// ���������(����������) ���
DEF_SWITCH_UGO_2(sLowpassFilter,
    "���", "���������/���������� ������� ��",
    "����.", "���.",
    "��� ", " ",
    PageChannelA::modeFilter, OnPress_Filter
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Front()
{
}

/// ����� ������ ������������� �������� ������
DEF_SWITCH_UGO_2(sFront,
    "�����", "����� ���� �������������",
    "�����", "����",
    "����� ", "���� ",
    PageChannelA::modeFront, OnPress_Front
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_VD()
{
}

/// ���������(����������) �������� �������� ����������
DEF_SWITCH_UGO_2(sDivider,
    "1/1 1/10", "���������/���������� �������� �������� ����������",
    "1:1", "1:10",
    " ", "1:10 ",
    PageChannelA::divider, OnPress_VD
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Sync()
{
}

/// ����� ������ ������������� ���, ���
DEF_SWITCH_UGO_2(sSync,
    "�����", "����� ������ ������������",
    "���", "���",
    "1200mV ", "-1240mV ",
    PageChannelA::levelSynch, OnPress_Sync
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

static Page pageChannelA(items);

Page *PageChannelA::self = &pageChannelA;
