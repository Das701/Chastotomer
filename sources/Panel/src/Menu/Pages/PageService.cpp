#include "defines.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageService.h"


static void OnPress_Information()
{

}

DEF_BUTTON(bInformation, "Инфо", "Вывод системной информации", OnPress_Information);


static void OnPress_ResetSettings()
{

}

DEF_BUTTON(bResetSettings, "Сброс настроек", "Сброс настроек в состояние по умолчанию", OnPress_ResetSettings);


static void OnPress_Language()
{

}

DEF_BUTTON(bLanguage, "Язык", "Выбор языка интерфейса", OnPress_Language);


static void OnPress_ResetMode()
{

}

DEF_BUTTON(bResetMode, "Сброс режима", "", OnPress_ResetMode);


static Item *items[7] =
{
    &bInformation,
    &bResetSettings,
    &bLanguage,
    &bResetMode,
    nullptr,
    nullptr,
    nullptr
};


static Page pageService(items, nullptr);

Page *PageService::self = &pageService;
