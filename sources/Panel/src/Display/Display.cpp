#include "Display.h"
#include "Primitives.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageModes.h"
#include "Text.h"


using namespace Display::Primitives;
using Display::Text;


static void DrawModeMeasureFrequency();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Update()
{
    BeginScene(Color::BLACK);

    Rectangle(256, 64).Draw(0, 0, Color::WHITE);

    DrawModeMeasureFrequency();

    Menu::Draw();

    EndScene();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawModeMeasureFrequency()
{
    static const char * const modes[4] = { "Режим 1", "Режим 2", "Режим 3", "Режим 4" };

    Text(modes[PageModes::ModeMeasureFrequency()]).Write(10, 10);
}
