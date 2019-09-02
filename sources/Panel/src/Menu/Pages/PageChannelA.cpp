#include "defines.h"
#include "PageChannelA.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"


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

static char *namesInput[] = { "����. ����", "����. ����", nullptr };

/// ����� ���������(���������) ����� �������� ������
static Switch sInputChoice(
    "���", "����", namesInput,
    &PageChannelA::channelInput,
    &OnPress_OCI
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Impedance()
{
}

static char *namesImpedance[] = { "1 ���", "50 ��", nullptr };

/// ��������� �������� ������������� �������� ������
static Switch sImpedance(
    "R��", "������� ������������� �������� ������", namesImpedance,
    &PageChannelA::inputImpedance,
    &OnPress_Impedance
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Filter()
{
}

static char *namesLowpassFilter[] = { "����.", "���.", nullptr };

/// ���������(����������) ���
static Switch sLowpassFilter(
    "���", "���������/���������� ������� ��", namesLowpassFilter,
    &PageChannelA::modeFilter,
    &OnPress_Filter
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Front()
{
}

static char *namesFront[] = { "�����", "����", nullptr };

/// ����� ������ ������������� �������� ������
static Switch sFront(
    "�����", "����� ���� �������������", namesFront,
    &PageChannelA::modeFront,
    &OnPress_Front
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_VD()
{
}

static char *namesDivider[] = { "1:1", "1:10", nullptr };

/// ���������(����������) �������� �������� ����������
static Switch sDivider(
    "1/1 1/10", "���������/���������� �������� �������� ����������", namesDivider,
    &PageChannelA::divider,
    &OnPress_VD
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Sync()
{
}

static char *namesSync[] = { "���", "���", nullptr };

/// ����� ������ ������������� ���, ���
static Switch sSync(
    "�����", "����� ������ ������������", namesSync,
    &PageChannelA::levelSynch,
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

static Page pageChannelA(items);

Page *PageChannelA::self = &pageChannelA;
