#include "defines.h"
#include "PageChannelC.h"
#include "Menu/MenuItems.h"

static Item *items[] =
{
    nullptr
};

static Page pageChannelC(items);

Page *PageChannelC::self = &pageChannelC;
