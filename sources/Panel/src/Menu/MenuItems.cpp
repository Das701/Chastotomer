#include "defines.h"
#include "MenuItems.h"
#include "Display/Primitives.h"


using Display::Primitives::Rectangle;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static Control emptyControl;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Page::Draw(int x, int y)
{
    if (closed)
    {
        DrawClosed(x, y);
    }
    else
    {
        DrawOpened(x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::DrawClosed(int x, int y)
{
    //Painter::DrawRectangle(x, y, Control::WIDTH, Control::HEIGHT, Color::WHITE);

    Rectangle(Control::WIDTH, Control::HEIGHT).Draw(x, y, Color::WHITE);
    drawClosed(x, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::DrawOpened(int x, int y)
{
    Control *control = FirstOnDisplay();

    while (control)
    {
        control->Draw(x, y);

        x += Control::WIDTH + Control::D_X;

        control = NextOnDisplay();

        if (control == LastOnDisplay())
        {
            break;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Control *Page::FirstOnDisplay()
{
    lastIndexed = currentSubPage * CONTROLS_ON_PAGE;

    return controls[lastIndexed];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Control *Page::NextOnDisplay()
{
    lastIndexed++;

    if (lastIndexed < NumControls())
    {
        return controls[lastIndexed];
    }

    return &emptyControl;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Control *Page::LastOnDisplay()
{
    return &emptyControl;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Page::NumControls()
{
    int result = 0;

    for (; controls[result] != nullptr; result++)
    {
    }

    return result;
}
