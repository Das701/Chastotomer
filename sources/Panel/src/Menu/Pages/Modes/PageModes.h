#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/PageModesA.h"
#include "Menu/Pages/Modes/PageModesB.h"
#include "Menu/Pages/Modes/PageModesC.h"
#include "Menu/Pages/Modes/PageModesD.h"


struct CurrentTypeMeasure
{
    static int Value();
    static void Draw(int x, int y);
};
