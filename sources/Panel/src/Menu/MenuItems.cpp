#include "defines.h"
#include "MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include "Menu/Pages/PageModes.h"
#include "stm32f4xx_hal.h"
#include "Menu/Pages/PageChannelA.h"
#include "Utils/String.h"
#include "Settings.h"
#include <cstring>
#include <string>


using namespace Display::Primitives;
using namespace Display;

char Item::hint[100];
uint Item::timeStopHint = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Enumeration::NumStates() const
{
    int result = 0;

    for (int i = 0; names[i] != nullptr; i++)
    {
        result++;
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Button::OnControl(const Control &control)
{
    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Button::Draw(int x, int y, bool selected)
{
    if (selected)
    {
        Color::BLACK.SetAsCurrent();
    }
    Text(text).Write(x, WIDTH, y + 2);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Page::Draw(int x, int y, bool)
{
    if (items)
    {
        for (int i = 0; items[i] != 0; i++)
        {
            if (i == selectedItem)
            {
                Rectangle(WIDTH - 2, HEIGHT - 1).Fill(x + 1, y + 1, Color::_14);
            }
        
            Rectangle(WIDTH, HEIGHT).Draw(x, y, Color::WHITE);
            items[i]->Draw(x, y, i == selectedItem);
            x += Item::WIDTH + 2;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::OnControl(const Control &control)
{
    switch (control.value)
    {
    case Control::Right:
        if( PageChannelA::syncPress.value == SyncPress::SyncPressed && Item::Hint() == hint)
        {
            set.syncValue += 20;
        }
        else
        {
            SelectNextItem();
        }
        return true;

    case Control::Left:
        if( PageChannelA::syncPress.value == SyncPress::SyncPressed && Item::Hint() == hint)
        {
            set.syncValue -= 20;
        }
        else
        {
            SelectPrevItem();
        }
        return true;

    case Control::GovButton: 
        timeStopHint = HAL_GetTick() + 5000;
        return SelectedItem()->OnControl(control);
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Page::NumItems()
{
    int i = 0;
    while(items[i] != 0)
    {
        i++;
    }
    return i;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SelectNextItem()
{
    Math::CircleIncrease<int>(&selectedItem, 0, NumItems() - 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SelectPrevItem()
{
    Math::CircleDecrease<int>(&selectedItem, 0, NumItems() - 1);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Switch::OnControl(const Control &control)
{
    if (control.action.IsPress() && control.value == Control::GovButton)
    {
        Math::CircleIncrease<uint8>(&state->value, 0, (uint8)(state->NumStates() - 1));

        if (funcOnPress)
        {
            funcOnPress();
        }

        CreateHint();
        
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Switch::Draw(int x, int y, bool selected)
{
    if(selected)
    {
        Color::BLACK.SetAsCurrent();
    }
    Text(text).Write(x, WIDTH, y + 2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Switch::CreateHint()
{
    std::strcpy(hint, commonHint);
    std::strcat(hint, " : ");
    std::strcat(hint, state->ToText());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char* Item::Hint()
{
    return (HAL_GetTick() < timeStopHint) ? hint : "";
}

