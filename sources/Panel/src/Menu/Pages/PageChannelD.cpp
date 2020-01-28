#include "defines.h"
#include "PageChannelD.h"
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

static Page pageChannelD(items);

Page *PageChannelD::self = &pageChannelD;
