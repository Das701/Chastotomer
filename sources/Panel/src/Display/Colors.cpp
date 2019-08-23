#include "defines.h"
#include "Colors.h"
#include "Display/Display.h"
#include "Display/Primitives.h"


using namespace Display::Primitives;

Color Color::BLACK(0);
Color Color::GRAY(1);
Color Color::WHITE(2);
Color Color::Number(3);


Color Color::current = Color::BLACK;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Color::SetAsCurrent()
{
    if (value != Color::Number.value)
    {
        current = Color(value);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color Color::GetCurrent()
{
    return current;
}
