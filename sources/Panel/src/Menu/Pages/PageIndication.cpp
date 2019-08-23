#include "defines.h"
#include "PageIndication.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"


using namespace Display::Primitives;
using namespace Display;


/// Выбор времени отображения результата измерения
static Button bDisplayTime("Т-ИНД");

/// Выбор опорного генератора
static Button bRefGenerator("ОГ");

/// Выбор источника запуска
static Button bLaunchSource("ЗАП");

/// Вход в режим калибровки 
static Button bCalibration("Калиб");

/// Включение(отключение) режима памяти
static Button bMemoryMode("Пам");

static Item *items[] =
{
    &bDisplayTime,
    &bRefGenerator,
    &bLaunchSource,
    &bCalibration,
    &bMemoryMode,
    nullptr
};

static Page pageIndication(items);

Page *PageIndication::self = &pageIndication;
