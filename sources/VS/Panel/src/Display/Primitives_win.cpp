#pragma once
#include "Display/Primitives.h"


#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


using namespace Display::Primitives;


extern wxMemoryDC memDC;


void Rectangle::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    Draw(x, y);
}


void Rectangle::Draw(int x, int y)
{
    if (width == 1 && height == 1)
    {
        Point().Draw(x, y);
    }
    else
    {
        memDC.DrawRectangle({ x, y, width, height });
    }
}


void Rectangle::Fill(int x, int y)
{
    if (width == 1 && height == 1)
    {
        Point().Draw(x, y);
    }
    else
    {
        wxBrush brush = memDC.GetBrush();
        wxPen pen = memDC.GetPen();
        memDC.SetBrush(wxBrush(pen.GetColour()));
        memDC.DrawRectangle({ x, y, width, height });
        memDC.SetBrush(brush);
    }
}


void HLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    Draw(x, y);
}


void HLine::Draw(int x, int y)
{
    memDC.DrawLine({ x, y }, { x + length, y });
}


void VLine::Draw(int x, int y)
{
    memDC.DrawLine({ x, y }, { x, y + length });
}


void Point::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    memDC.DrawPoint({ x, y });
}


void Point::Draw(int x, int y)
{
    memDC.DrawPoint({ x, y });
}
