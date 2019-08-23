#include "defines.h"
#include "MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"


using namespace Display::Primitives;
using namespace Display;

Page Page::empty;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Page::Draw(int x, int y)
{

    if (items)
    {
        for (int i = 0; items[i] != 0; i++)
        {
            if(i == currentItem) 
            {
                Rectangle(WIDTH, HEIGHT).Draw(x, y, Color::GRAY);
                Rectangle(WIDTH - 2, HEIGHT - 2).Draw(x + 1, y + 1, Color::GRAY);
            }
            else
            {
                Rectangle(WIDTH, HEIGHT).Draw(x, y, Color::WHITE);
            }
            items[i]->Draw(x, y);
            x += Item::WIDTH + 2;

        }
        
    }

}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Button::Draw(int x, int y)
{
    Text(text).Write(x, WIDTH, y + 2);
}

bool Page::PressButton(Control control)
{
    if(control.value == Control::GovButton)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Page::NumItems()
{
    int i;
    for (i = 0; items[i] != 0; i++)
    {
    }
    return i;
}

void Page::ControlRight()
{

    if(currentItem == NumItems() - 1)
    {
        currentItem = 0;
    }
    else
    {
        currentItem++;
    }
}
void Page::ControlLeft()
{
    if(currentItem == 0)
    {
        currentItem = NumItems() - 1;
    }
    else
    {
        currentItem--;
    }
}