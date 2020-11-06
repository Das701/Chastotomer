#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/PagesSettings.h"


static Item *items[7] =
{
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

static Page pageChannelC(items);

Page *PageSettingsC::self = &pageChannelC;
