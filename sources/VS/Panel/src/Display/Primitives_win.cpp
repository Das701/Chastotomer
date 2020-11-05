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

    memDC.DrawRectangle({ x, y, width + 1, height + 1 });
}


void Rectangle::Fill(int x, int y, Color color)
{
    color.SetAsCurrent();

    wxBrush brush = memDC.GetBrush();
    wxPen pen = memDC.GetPen();
    memDC.SetBrush(wxBrush(pen.GetColour()));
    memDC.DrawRectangle({ x, y, width + 1, height + 1 });
    memDC.SetBrush(brush);
}


void HLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

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
