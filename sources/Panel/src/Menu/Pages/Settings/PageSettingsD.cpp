#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Settings/PagesSettings.h"


static Item *items[7] =
{
    nullptr
};

static Page pageChannelD(items, nullptr);

Page *PageSettingsD::self = &pageChannelD;
