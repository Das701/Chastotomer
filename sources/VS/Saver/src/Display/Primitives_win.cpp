#pragma once
#include "Display/Primitives.h"


#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


using namespace Primitives;


extern wxMemoryDC memDC;


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


void Line::Draw(int x1, int y1, int x2, int y2)
{
    memDC.DrawLine({ x1, y1 }, { x2, y2 });
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
