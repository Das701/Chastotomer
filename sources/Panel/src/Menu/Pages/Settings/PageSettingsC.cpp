#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Settings/PagesSettings.h"


static Item *items[7] =
{
    nullptr
};

static MenuPage pageChannelC(items, nullptr);

MenuPage *PageSettingsC::self = &pageChannelC;
