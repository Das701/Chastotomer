#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Hint.h"
#include "Menu/MenuItems.h"
#include <cstring>


// ѕолный текст подсказки дл€ последнего использованного итема
static String hint;
// »тем, дл€ которого действует подсказка
static const Item *item = nullptr;
// ¬рем€ окончани€ отображени€ подсказки
static uint timeHideHint;


void Hint::Create(const Item *i)
{
    if (i != nullptr)
    {
        item = i;
        item->CreateHint(hint);
        timeHideHint = TIME_MS + 5000;
    }
}


String Hint::Text()
{
    return TIME_MS <  timeHideHint ? String(hint) : String("");
}


const Item *Hint::UnderItem()
{
    return item;
}


void Hint::Hide()
{
    hint[0] = 0;
    item = nullptr;
    timeHideHint = TIME_MS - 1;
}
