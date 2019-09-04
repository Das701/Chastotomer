#include "defines.h"
#include "PageChannelA.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
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

static char *namesInput[] = { "Откр. вход", "Закр. вход", nullptr };
static char *ugoInput[] = { "ОВ ", "ЗВ ", nullptr };

/// Выбор закрытого(открытого) входа текущего канала
static Switch sInputChoice(
    "ОЗВ", "Вход", namesInput,
    ugoInput, 
    &PageChannelA::channelInput,
    &OnPress_OCI
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Impedance()
{
}

static char *namesImpedance[] = { "1 МОм", "50 Ом", nullptr };
static char *ugoImpedance[] = { "1МОм ", "50Ом ", nullptr };

/// Установка входного сопротивления текущего канала
static Switch sImpedance(
    "Rвх", "Входное сопротивление текущего канала", namesImpedance,
    ugoImpedance, &PageChannelA::inputImpedance,
    &OnPress_Impedance
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Filter()
{
}

static char *namesLowpassFilter[] = { "Откл.", "Вкл.", nullptr };
static char *ugoLowpassFilter[] = { "ФНЧ ", " ", nullptr };

/// Включение(отключение) ФНЧ
static Switch sLowpassFilter(
    "ФНЧ", "Включение/отключение фильтра НЧ", namesLowpassFilter,
    ugoLowpassFilter, &PageChannelA::modeFilter,
    &OnPress_Filter
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Front()
{
}

static char *namesFront[] = { "Фронт", "Срез", nullptr };
static char *ugoFront[] = { "Фронт ", "Срез ", nullptr };

/// Выбор фронта синхронизации текущего канала
static Switch sFront(
    "Фронт", "Выбор типа синхронизации", namesFront,
    ugoFront, &PageChannelA::modeFront,
    &OnPress_Front
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_VD()
{
}

static char *namesDivider[] = { "1:1", "1:10", nullptr };
static char *ugoDivider[] = { " ", "1:10 ", nullptr };

/// Включение(отключение) входного делителя напряжения
static Switch sDivider(
    "1/1 1/10", "Включение/отключение входного делителя напряжения", namesDivider,
    ugoDivider, &PageChannelA::divider,
    &OnPress_VD
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Sync()
{
}

static char *namesSync[] = { "ТТЛ", "ЭСЛ", nullptr };
static char *ugoSync[] = { "1200mV ", "-1240mV ", nullptr };

/// Выбор уровня синхронизации ТТЛ, ЭСЛ
static Switch sSync(
    "Синхр", "Выбор уровня сихронизации", namesSync,
    ugoSync, &PageChannelA::levelSynch,
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
