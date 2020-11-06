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

static Page pageChannelD(items);

Page *PageSettingsD::self = &pageChannelD;
