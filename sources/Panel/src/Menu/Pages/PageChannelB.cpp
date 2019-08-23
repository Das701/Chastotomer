#include "defines.h"
#include "PageChannelB.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"


using namespace Display::Primitives;
using namespace Display;


/// Выбор закрытого(открытого) входа текущего канала
static Button bInputChoice("~");

/// Установка входного сопротивления текущего канала
static Button bImpedance("Rвх");

/// Включение(отключение) ФНЧ
static Button bLowpassFilter("ФНЧ");

/// Выбор фронта синхронизации текущего канала
static Button bFront(" ");

/// Включение(отключение) входного делителя напряжения
static Button bVoltageDivider("1/1 1/10");

/// Выбор уровня синхронизации ТТЛ, ЭСЛ
static Button bSync("Синхр");

static Item *items[] =
{
    &bInputChoice,
    &bImpedance,
    &bLowpassFilter,
    &bFront,
    &bVoltageDivider,
    &bSync,
    nullptr
};

static Page pageChannelB(items);

Page *PageChannelB::self = &pageChannelB;
