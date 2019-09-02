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
/// Выбор закрытого(открытого) входа текущего канала
static Switch sInputChoice("ОЗВ", (uint8 *)&PageChannelA::modeOCI, 2, &OnPress_OCI);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Impedance()
{
    PageChannelA::hintA.value = HintA::Impedance;
}
/// Установка входного сопротивления текущего канала
static Switch sImpedance("Rвх", (uint8 *)&PageChannelA::modeImpedance, 2, &OnPress_Impedance);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Filter()
{
    PageChannelA::hintA.value = HintA::Filter;
}
/// Включение(отключение) ФНЧ
static Switch sLowpassFilter("ФНЧ", (uint8 *)&PageChannelA::modeFilter, 2, &OnPress_Filter);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Front()
{
    PageChannelA::hintA.value = HintA::Front;
}
/// Выбор фронта синхронизации текущего канала
static Switch sFront("Фронт", (uint8 *)&PageChannelA::modeFront, 2, &OnPress_Front);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_VD()
{
    PageChannelA::hintA.value = HintA::VD;
}
/// Включение(отключение) входного делителя напряжения
static Switch sVoltageDivider("1/1 1/10", (uint8 *)&PageChannelA::modeVD, 2, &OnPress_VD);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Sync()
{
    PageChannelA::hintA.value = HintA::Sync;
}
/// Выбор уровня синхронизации ТТЛ, ЭСЛ
static Switch sSync("Синхр", (uint8 *)&PageChannelA::modeSync, 2, &OnPress_Sync);

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
    static const pString names[2] = { "Откр. вход", "Закр. вход" };
    return names[value];
}

pString ModeImpedance::ToText()
{
    static const pString names[2] = { "1 МОм", "50 Ом" };
    return names[value];
}

pString ModeFilter::ToText()
{
    static const pString names[2] = { "Откл.", "Вкл." };
    return names[value];
}

pString ModeFront::ToText()
{
    static const pString names[2] = { "Фронт", "Срез" };
    return names[value];
}

pString ModeVD::ToText()
{
    static const pString names[2] = { "1:1", "1:10" };
    return names[value];
}

pString ModeSync::ToText()
{
    static const pString names[2] = { "ТТЛ", "ЭСЛ" };
    return names[value];
}
