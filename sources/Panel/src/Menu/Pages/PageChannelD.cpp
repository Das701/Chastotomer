#include "defines.h"
#include "PageChannelD.h"
#include "Menu/MenuItems.h"

static Item *items[] =
{
    nullptr
};

static Page pageChannelD(items);

Page *PageChannelD::self = &pageChannelD;
