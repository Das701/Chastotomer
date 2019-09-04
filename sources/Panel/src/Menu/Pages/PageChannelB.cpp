#include "defines.h"
#include "PageChannelB.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"

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

static char *namesInput[] = { "����. ����", "����. ����", nullptr };
static char *ugoInput[] = { "�� ", "�� ", nullptr };

/// ����� ���������(���������) ����� �������� ������
static Switch sInputChoice(
    "���", "����", namesInput,
    ugoInput, 
    &PageChannelB::channelInput,
    &OnPress_OCI
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Impedance()
{
}

static char *namesImpedance[] = { "1 ���", "50 ��", nullptr };
static char *ugoImpedance[] = { "1��� ", "50�� ", nullptr };

/// ��������� �������� ������������� �������� ������
static Switch sImpedance(
    "R��", "������� ������������� �������� ������", namesImpedance,
    ugoImpedance, &PageChannelB::inputImpedance,
    &OnPress_Impedance
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Filter()
{
}

static char *namesLowpassFilter[] = { "����.", "���.", nullptr };
static char *ugoLowpassFilter[] = { "��� ", " ", nullptr };

/// ���������(����������) ���
static Switch sLowpassFilter(
    "���", "���������/���������� ������� ��", namesLowpassFilter,
    ugoLowpassFilter, &PageChannelB::modeFilter,
    &OnPress_Filter
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Front()
{
}

static char *namesFront[] = { "�����", "����", nullptr };
static char *ugoFront[] = { "����� ", "���� ", nullptr };

/// ����� ������ ������������� �������� ������
static Switch sFront(
    "�����", "����� ���� �������������", namesFront,
    ugoFront, &PageChannelB::modeFront,
    &OnPress_Front
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_VD()
{
}

static char *namesDivider[] = { "1:1", "1:10", nullptr };
static char *ugoDivider[] = { " ", "1:10 ", nullptr };

/// ���������(����������) �������� �������� ����������
static Switch sDivider(
    "1/1 1/10", "���������/���������� �������� �������� ����������", namesDivider,
    ugoDivider, &PageChannelB::divider,
    &OnPress_VD
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Sync()
{
}

static char *namesSync[] = { "���", "���", nullptr };
static char *ugoSync[] = { "1200mV ", "-1240mV ", nullptr };

/// ����� ������ ������������� ���, ���
static Switch sSync(
    "�����", "����� ������ ������������", namesSync,
    ugoSync, &PageChannelB::levelSynch,
    &OnPress_Sync
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
