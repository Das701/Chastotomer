#include "defines.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageService.h"


static void OnPress_Information()
{

}

DEF_BUTTON(bInformation, "»нфо", OnPress_Information);


static void OnPress_ResetSettings()
{

}

DEF_BUTTON(bResetSettings, "—брос настроек", OnPress_ResetSettings);


static void OnPress_Language()
{

}

DEF_BUTTON(bLanguage, "язык", OnPress_Language);


static void OnPress_ResetMode()
{

}

DEF_BUTTON(bResetMode, "—брос режима", OnPress_ResetMode);


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
