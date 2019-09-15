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

/// Выбор закрытого(открытого) входа текущего канала
DEF_SWITCH_UGO_2(sInputChoice,
    "\x7C \x7D", "Вход",
    "Открытый вход", "Закрытый вход",
    "\x7C", "\x7D",
    PageChannelA::channelInput, OnPress_OCI
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Impedance()
{
}

/// Установка входного сопротивления текущего канала
DEF_SWITCH_UGO_2(sImpedance,
    "Rвх", "Входное сопротивление канала",
    "1 МОм", "50 Ом",
    "1МОм", "50Ом",
    PageChannelA::inputImpedance, OnPress_Impedance
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Filter()
{
}

/// Включение(отключение) ФНЧ
DEF_SWITCH_UGO_2(sLowpassFilter,
    "ФНЧ", "Включение/отключение фильтра НЧ",
    "Откл.", "Вкл.",
    "ФНЧ", "",
    PageChannelA::modeFilter, OnPress_Filter
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Front()
{
}

/// Выбор фронта синхронизации текущего канала
DEF_SWITCH_UGO_2(sFront,
    "\x8D \x8E", "Выбор типа синхронизации",
    "Фронт", "Срез",
    "\x8D", "\x8E",
    PageChannelA::modeFront, OnPress_Front
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_VD()
{
}

/// Включение(отключение) входного делителя напряжения
DEF_SWITCH_UGO_2(sDivider,
    "\x7E\x7F\x7E \x7E\x7F\x8F", "Вкл/откл входного делителя",
    "1:1", "1:10",
    "", "1:10",
    PageChannelA::divider, OnPress_VD
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Sync()
{
}

/// Выбор уровня синхронизации ТТЛ, ЭСЛ
DEF_SWITCH_UGO_2(sSync,
    "Синхр", "Выбор уровня сихронизации",
    "ТТЛ", "ЭСЛ",
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
