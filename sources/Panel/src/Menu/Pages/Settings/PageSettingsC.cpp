#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Settings/PagesSettings.h"


static Item *items[7] =
{
    nullptr
};

static Page pageChannelC(items, nullptr);

Page *PageSettingsC::self = &pageChannelC;
