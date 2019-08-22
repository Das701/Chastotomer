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
        for (int i = 0; items[i]; i++)
        {
            Rectangle(WIDTH, HEIGHT).Draw(x, y);
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
