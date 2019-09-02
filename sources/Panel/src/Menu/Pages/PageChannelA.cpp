#include "defines.h"
#include "PageChannelA.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"


using namespace Display::Primitives;
using namespace Display;

extern Item *items[];

ModeOCI          PageChannelA::modeOCI(ModeOCI::Closed);
HintA                 PageChannelA::hintA(HintA::OCI);
ModeImpedance PageChannelA::modeImpedance(ModeImpedance::MOhm1);
ModeFilter    PageChannelA::modeFilter(ModeFilter::Off);
ModeFront  PageChannelA::modeFront(ModeFront::Front);
ModeVD       PageChannelA::modeVD(ModeVD::A1_1);
ModeSync       PageChannelA::modeSync(ModeSync::TTL);


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_OCI()
{
    PageChannelA::hintA.value = HintA::OCI;
}
/// ����� ���������(���������) ����� �������� ������
static Switch sInputChoice("���", (uint8 *)&PageChannelA::modeOCI, 2, &OnPress_OCI);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Impedance()
{
    PageChannelA::hintA.value = HintA::Impedance;
}
/// ��������� �������� ������������� �������� ������
static Switch sImpedance("R��", (uint8 *)&PageChannelA::modeImpedance, 2, &OnPress_Impedance);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Filter()
{
    PageChannelA::hintA.value = HintA::Filter;
}
/// ���������(����������) ���
static Switch sLowpassFilter("���", (uint8 *)&PageChannelA::modeFilter, 2, &OnPress_Filter);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Front()
{
    PageChannelA::hintA.value = HintA::Front;
}
/// ����� ������ ������������� �������� ������
static Switch sFront("�����", (uint8 *)&PageChannelA::modeFront, 2, &OnPress_Front);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_VD()
{
    PageChannelA::hintA.value = HintA::VD;
}
/// ���������(����������) �������� �������� ����������
static Switch sVoltageDivider("1/1 1/10", (uint8 *)&PageChannelA::modeVD, 2, &OnPress_VD);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Sync()
{
    PageChannelA::hintA.value = HintA::Sync;
}
/// ����� ������ ������������� ���, ���
static Switch sSync("�����", (uint8 *)&PageChannelA::modeSync, 2, &OnPress_Sync);

static Item *items[] =
{
    &sInputChoice,
    &sImpedance,
    &sLowpassFilter,
    &sFront,
    &sVoltageDivider,
    &sSync,
    nullptr
};

static Page pageChannelA(items);

Page *PageChannelA::self = &pageChannelA;

pString ModeOCI::ToText()
{
    static const pString names[2] = { "����. ����", "����. ����" };
    return names[value];
}

pString ModeImpedance::ToText()
{
    static const pString names[2] = { "1 ���", "50 ��" };
    return names[value];
}

pString ModeFilter::ToText()
{
    static const pString names[2] = { "����.", "���." };
    return names[value];
}

pString ModeFront::ToText()
{
    static const pString names[2] = { "�����", "����" };
    return names[value];
}

pString ModeVD::ToText()
{
    static const pString names[2] = { "1:1", "1:10" };
    return names[value];
}

pString ModeSync::ToText()
{
    static const pString names[2] = { "���", "���" };
    return names[value];
}
