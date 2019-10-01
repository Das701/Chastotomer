#include "defines.h"
#include "PageChannelC.h"
#include "Menu/MenuItems.h"

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
