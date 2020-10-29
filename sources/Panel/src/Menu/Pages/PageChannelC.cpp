#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Pages.h"


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

Page *PageChannelC::self = &pageChannelC;
