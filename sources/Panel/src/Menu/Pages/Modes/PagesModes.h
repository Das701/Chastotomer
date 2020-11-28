#pragma once
#include "Settings.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Menu/Pages/Channels/Channels.h"


class PageModesA
{
public:
    static void RelationOn();
    static void RelationOff();
    static bool RelationCheck();

    static bool StartStop();
    static void ToggleStartStop();
};
