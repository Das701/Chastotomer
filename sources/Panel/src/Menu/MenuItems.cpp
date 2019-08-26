#include "defines.h"
#include "MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"


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
    DrawOnClick();
    if (items)
    {
        for (int i = 0; items[i] != 0; i++)
        {
            Color color = Color::WHITE;

            if (i == currentItem)
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
    clickCount++;
    if (clickCount == 4)
    {
        clickCount = 0;
    }

        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Page::NumItems()
{
    int i = 0;
    while(items[i++] != 0)
    {
    }
    return i - 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SelectNextItem()
{
    currentItem++;
    if (currentItem == NumItems())
    {
        currentItem = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SelectPrevItem()
{
    currentItem--;
    if (currentItem < 0)
    {
        currentItem = NumItems() - 1;
    }
}

void Page::DrawOnClick()
{
    if (clickCount == 1)
    {
        items[currentItem]->Draw(10, 10);
    }
    else if (clickCount == 2)
    {
        items[currentItem]->Draw(10, 10);
        Rectangle(WIDTH, HEIGHT).Draw(10, 10, Color::WHITE);
    }
    else if (clickCount == 3)
    {
        items[currentItem]->Draw(10, 10);
        Rectangle(WIDTH, HEIGHT).Draw(10, 10, Color::WHITE);
        Rectangle(WIDTH - 2, HEIGHT - 2).Draw(11, 11, Color::WHITE);
    }
}
