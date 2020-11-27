#include "defines.h"
#include "Settings.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageService.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Menu/Pages/Modes/PagesModes.h"


static void OnPress_Information()
{

}

DEF_BUTTON(bInformation, "Инфо", "Вывод системной информации", OnPress_Information);


static void OnPress_ResetSettings()
{
    TimeMeasure::Set(TimeMeasure::_1ms);
    NumberPeriods::Set(NumberPeriods::_1);
    PeriodTimeLabels::Set(PeriodTimeLabels::T_8);

    PageModesA::typeMeasure.value = TypeMeasure::Frequency;
    PageModesA::modeMeasureFrequency.value = ModeMeasureFrequency::Frequency;

    CURRENT_CHANNEL = Channel::_A;

    Menu::SetOpenedPage(PageModesA::self);

    PageModes::OnChanged_TypeMeasure();

    Channel::LoadCurrentToFPGA();

    PageModes::OnChanged_TypeMeasure();

    Channel::LoadCurrentToFPGA();
}

DEF_BUTTON(bResetSettings, "Сброс", "Сброс настроек в состояние по умолчанию", OnPress_ResetSettings);


static void OnPress_Language()
{

}

DEF_BUTTON(bLanguage, "Язык", "Выбор языка интерфейса", OnPress_Language);


static void OnPress_ShowStatistics()
{
    set.showStatistics = !set.showStatistics;
}

DEF_BUTTON(bShowStatistics, "Статистика", "Показывать временную статистику", OnPress_ShowStatistics);


static Item *items[7] =
{
    &bInformation,
    &bResetSettings,
    &bLanguage,
    &bShowStatistics,
    nullptr,
    nullptr,
    nullptr
};


static Page pageService(items, nullptr);

Page *PageService::self = &pageService;
