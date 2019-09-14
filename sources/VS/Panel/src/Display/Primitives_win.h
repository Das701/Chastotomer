#pragma once
#include "Display/Primitives.h"


using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Rectangle::Rectangle(int _width, int _height) : width(_width), height(_height)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Rectangle::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    SDL_Rect rect = { x, y, width + 1, height + 1 };
    SDL_RenderDrawRect(renderer, &rect);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Rectangle::Fill(int x, int y, Color color)
{
    color.SetAsCurrent();

    SDL_Rect rect = { x, y, width + 1, height + 1 };
    SDL_RenderFillRect(renderer, &rect);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    SDL_RenderDrawLine(renderer, x, y, x + length, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Point::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    SDL_RenderDrawPoint(renderer, x, y);
}
