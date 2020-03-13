#pragma once
#include "Menu/MenuItems.h"

namespace Menu
{
    void Init();

    void Draw();

    bool Update();

    char *ChannelSettings();
    
    Page* UsedPage();
    
    uint8 UsedChannel();
};
