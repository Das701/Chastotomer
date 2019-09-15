#include "defines.h"
#include "PageChannelA.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"
#include <cstring>

using namespace Display::Primitives;
using namespace Display;

extern Item *items[7];

SyncPress       PageChannelA::syncPress(SyncPress::SyncNotPressed);
ChannelInput    PageChannelA::channelInput(ChannelInput::Closed);
InputImpedance  PageChannelA::inputImpedance(InputImpedance::_1MOmh);
ModeFilter      PageChannelA::modeFilter(ModeFilter::Off);
ModeFront       PageChannelA::modeFront(ModeFront::Front);
Divider         PageChannelA::divider(Divider::_1);
LevelSynch      PageChannelA::levelSynch(LevelSynch::TTL);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_OCI()
{
    PageChannelA::syncPress.value = SyncPress::SyncNotPressed;
}

/// ����� ���������(���������) ����� �������� ������
DEF_SWITCH_UGO_2(sInputChoice,
    "\x7C \x7D", "����",
    "�������� ����", "�������� ����",
    "\x7C", "\x7D",
    PageChannelA::channelInput, OnPress_OCI
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Impedance()
{
    PageChannelA::syncPress.value = SyncPress::SyncNotPressed;
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
    PageChannelA::syncPress.value = SyncPress::SyncNotPressed;
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
    PageChannelA::syncPress.value = SyncPress::SyncNotPressed;
}

/// ����� ������ ������������� �������� ������
DEF_SWITCH_UGO_2(sFront,
    "\x8D \x8E", "����� ���� �������������",
    "�����", "����",
    "\x8D", "\x8E",
    PageChannelA::modeFront, OnPress_Front
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_VD()
{
    PageChannelA::syncPress.value = SyncPress::SyncNotPressed;
}

/// ���������(����������) �������� �������� ����������
DEF_SWITCH_UGO_2(sDivider,
    "\x7E\x7F\x7E \x7E\x7F\x8F", "���/���� �������� ��������",
    "1:1", "1:10",
    "", "1:10",
    PageChannelA::divider, OnPress_VD
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Sync()
{
    PageChannelA::syncPress.value = SyncPress::SyncPressed;
}

/// ����� ������ ������������� ���, ���
DEF_SWITCH_UGO_2(sSync,
    "�����", "����� ������ ������������",
    "���", "���",
    "1200mV", "-1240mV",
    PageChannelA::levelSynch, OnPress_Sync
);

static Item *items[7] =
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
