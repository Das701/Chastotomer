#include "defines.h"
#include "FreqMeter.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/FPGA.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/MenuItemsDef.h"

using namespace Primitives;

extern Item *items[7];
static bool onceLaunch = false;

DisplayTime  PageIndication::displayTime(DisplayTime::_100ms);
RefGenerator PageIndication::refGenerator(RefGenerator::Internal);
LaunchSource PageIndication::launchSource(LaunchSource::Internal);
Calibration  PageIndication::calibration(Calibration::Unpressed);
MemoryMode   PageIndication::memoryMode(MemoryMode::On);



static void OnPress_DisplayTime()
{
    FreqMeter::LoadDisplayTime();
}

// Выбор времени отображения результата измерения
DEF_SWITCH_3(sDisplayTime,
    "Т-ИНД", "Время индикации",
    "0.1s.", "1s.", "10s.",
    PageIndication::displayTime, OnPress_DisplayTime
);


static void OnPress_RefGenerator()
{
    RefGenerator::LoadToFPGA();
}

// Выбор опорного генератора
DEF_SWITCH_2(sRefGenerator,
    "ОГ", "Выбор опорного генератора",
    "Внутренний", "Внешний", 
    PageIndication::refGenerator, OnPress_RefGenerator
);


static void OnPress_LaunchSource()
{
    LaunchSource::LoadToFPGA();
}

// Выбор источника запуска
DEF_SWITCH_3(sLaunchSource,
    "ЗАП", "Выбор источника запуска",
    "Внутренний", "Внешний", "Однократный",
    PageIndication::launchSource, OnPress_LaunchSource);

static void OnPress_Calibration()
{
    FreqMeter::LoadCalibration();
    if(PageIndication::calibration == Calibration::Pressed)
    {
        FPGA::ReadCalibNumber();
    }
}

// Вход в режим калибровки
DEF_SWITCH_2(sCalibration,
    "Калиб", "",
    "Не нажата", "Нажата", 
    PageIndication::calibration, OnPress_Calibration
);


static void OnPress_MemoryMode()
{
    MemoryMode::Load();
}

bool PageIndication::OnceLaunch()
{
    return onceLaunch;
}

void PageIndication::OnceLaunchSwitchTrue()
{
    onceLaunch = true;
}

void PageIndication::OnceLaunchSwitchFalse()
{
    onceLaunch = false;
}

// Включение(отключение) режима памяти
DEF_SWITCH_2(sMemoryMode,
    "Память", "Режим память",
    "Выкл", "Вкл",
    PageIndication::memoryMode, OnPress_MemoryMode
);

static Item *items[7] =
{
    &sDisplayTime,
    &sRefGenerator,
    &sLaunchSource,
    &sCalibration,
    &sMemoryMode,
    nullptr,
};

static Page pageIndication(items, nullptr);

Page *PageIndication::self = &pageIndication;
