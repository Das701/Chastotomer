#include "defines.h"
#include "MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include "Menu/Pages/PageModes.h"

using namespace Display::Primitives;
using namespace Display;

Page Page::empty;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Button::OnControl(Control control)
{
    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Button::Draw(int x, int y)
{
    Text(text).Write(x, WIDTH, y + 2);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Page::Draw(int x, int y)
{
    if (items)
    {
        for (int i = 0; items[i] != 0; i++)
        {
            Color color = Color::WHITE;

            if (i == selectedItem)
            {
                color = Color::GRAY;
                Rectangle(WIDTH - 2, HEIGHT - 2).Draw(x + 1, y + 1, color);
            }

            Rectangle(WIDTH, HEIGHT).Draw(x, y, color);
            items[i]->Draw(x, y);
            x += Item::WIDTH + 2;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::OnControl(Control control)
{
    switch (control.value)
    {
    case Control::Right:
        SelectNextItem();
        return true;

    case Control::Left:
        SelectPrevItem();
        return true;

    case Control::GovButton: 
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
bool Switch::OnControl(Control control)
{
    if (control.action.IsPress() && control.value == Control::GovButton)
    {
        Math::CircleIncrease<uint8>(state, 0, (uint8)(num - 1));

        if (funcOnPress)
        {
            funcOnPress();
        }

        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Switch::Draw(int x, int y)
{
    Text(text).Write(x, WIDTH, y + 2);
}

