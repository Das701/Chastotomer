#include "defines.h"
#include "Colors.h"
#include "Display/Display.h"


Color Color::BLACK(0);
Color Color::WHITE(1);
Color Color::Number(2);


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
