#pragma once
#include "Menu/MenuItems.h"

namespace Menu
{
    void Draw();

    bool Update();

    char *ChannelSettings();
    
    Page* OpenedPage();
};
