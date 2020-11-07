#pragma once
#include "Display/Primitives.h"


#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


using namespace Display::Primitives;


extern wxMemoryDC memDC;


Rectangle::Rectangle(int _width, int _height) : width(_width), height(_height)
{

}


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


void Rectangle::DrawRounded(int x, int y, int round, Color color)
{
    color.SetAsCurrent();

    HLine(width - 2 * round + 1).Draw(x + round, y);
    HLine(width - 2 * round + 1).Draw(x + round, y + height);
    VLine(height - 2 * round + 1).Draw(x, y + round);
    VLine(height - 2 * round + 1).Draw(x + width, y + round);

    if (round == 2)
    {
        Point().Draw(x + 1, y + 1);
        Point().Draw(x + width - 1, y + 1);
        Point().Draw(x + 1, y + height - 1);
        Point().Draw(x + width - 1, y + height - 1);
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
