#include "Display.h"
#include "Primitives.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageModes.h"
#include "Text.h"


using namespace Display::Primitives;
using Display::Text;


static void DrawModeMeasureFrequency();
static void DrawModeMeasurePeriod();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Update()
{
    BeginScene(Color::BLACK);

    Rectangle(256, 64).Draw(0, 0, Color::WHITE);

    DrawModeMeasurePeriod();

    Menu::Draw();

    EndScene();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawModeMeasureFrequency()
{
    static const char * const modes[5] = { "Частота", "f(A)/f(C)", "f(A)/f(B)", "f=1/T", "Тахометр" };

    Text(modes[PageModes::ModeMeasureFrequency()]).Write(10, 30);
    Text("Измерение частоты").Write(38, 15);
    Text("f").Write(18, 10);
    Rectangle(25, 15).Draw(10, 8, Color::WHITE);
}

static void DrawModeMeasurePeriod()
{
    static const char * const modes[2] = { "Период", "T=1/f" };

    Text(modes[PageModes::ModeMeasurePeriod()]).Write(10, 30);
    Text("Измерение периода").Write(38, 15);
    Text("T").Write(18, 10);
    Rectangle(25, 15).Draw(10, 8, Color::WHITE);
}

static void DrawMode()
{

}
