#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Channels/Channels.h"


static Item *items[7] =
{
    nullptr
};

static Page pageChannelC(items, nullptr);

Page *PageSettingsC::self = &pageChannelC;
