#include "Display.h"
#include "Primitives.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageModes.h"
#include "Text.h"


using namespace Display::Primitives;
using Display::Text;
extern int drawMode;
static int lastDrawMode = 1;
extern int statusBar;
static void DrawMode();
static void DrawModeMeasureFrequency();
static void DrawModeMeasurePeriod();
static void DrawModeMeasureDuration();
static void DrawModeCountPulse();
static void DrawModeTimeLabelLenght();
static void DrawModeTimeMeasure();
static void DrawModeNumberPeriods();
static void StatusBar();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Update()
{
    BeginScene(Color::BLACK);

    Rectangle(256, 64).Draw(0, 0, Color::WHITE);

    DrawMode();

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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawModeMeasurePeriod()
{
    static const char * const modes[2] = { "Период", "T=1/f" };

    Text(modes[PageModes::ModeMeasurePeriod()]).Write(10, 30);
    Text("Измерение периода").Write(38, 15);
    Text("T").Write(18, 10);
    Rectangle(25, 15).Draw(10, 8, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawModeMeasureDuration()
{
    static const char * const modes[6] = { "ndt", "<ndt>", "ndt/1нс", "Интервал", "1/S", "Фаза" };

    Text(modes[PageModes::ModeMeasureDuration()]).Write(10, 30);
    Text("Измерение длительности").Write(38, 15);
    Text("t").Write(18, 10);
    Rectangle(25, 15).Draw(10, 8, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawModeCountPulse()
{
    static const char * const modes[3] = { "Ручн.", "А(tC)", "А(TC)" };

    Text(modes[PageModes::ModeCountPulse()]).Write(10, 30);
    Text("Счет числа импульсов").Write(38, 15);
    Text("Счет").Write(12, 10);
    Rectangle(25, 15).Draw(10, 8, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawModeTimeLabelLenght()
{
    static const char * const modes[6] = { "10-3", "10-4", "10-5", "10-6", "10-7", "10-8" };

    Text(modes[PageModes::ModeTimeLabelLenght()]).Write(10, 30);
    Text("Длительность временных меток:").Write(38, 15);
    switch(lastDrawMode)
    {
        case 1:
        Text("f").Write(18, 10);
        case 2:
        Text("T").Write(18, 10);
        case 3:
        Text("t").Write(18, 10);
    }    
    Rectangle(25, 15).Draw(10, 8, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawModeTimeMeasure()
{
    static const char * const modes[6] = { "1ms", "10ms", "100ms", "1s", "10s", "100s" };

    Text(modes[PageModes::ModeTimeMeasure()]).Write(10, 30);
    Text("Время счета:").Write(38, 15);
    switch(lastDrawMode)
    {
        case 1:
        Text("f").Write(18, 10);
        case 2:
        Text("T").Write(18, 10);
        case 3:
        Text("t").Write(18, 10);
    }    
    Rectangle(25, 15).Draw(10, 8, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawModeNumberPeriods()
{
    static const char * const modes[6] = { "1", "10", "100", "1K", "10K", "100K" };

    Text(modes[PageModes::ModeNumberPeriods()]).Write(10, 30);
    Text("Число периодов измерения:").Write(38, 15);
    switch(lastDrawMode)
    {
        case 1:
        Text("f").Write(18, 10);
        case 2:
        Text("T").Write(18, 10);
        case 3:
        Text("t").Write(18, 10);
    }    
    Rectangle(25, 15).Draw(10, 8, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void StatusBar()
{
    
    if(statusBar == 1)
    {
        static const char * const modes[6] = { "1ms", "10ms", "100ms", "1s", "10s", "100s" };
        Text(modes[PageModes::ModeTimeMeasure()]).Write(8, 49);
    }
    if(statusBar == 2)
    {
        static const char * const modes[6] = { "1", "10", "100", "1K", "10K", "100K" };
        Text(modes[PageModes::ModeNumberPeriods()]).Write(8, 49);
    }
    Rectangle(25, 15).Draw(5, 45, Color::WHITE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawMode()
{
    StatusBar();
    if(drawMode == 1)
    {
        DrawModeMeasureFrequency();
        lastDrawMode = 1;
    }
    else if (drawMode == 2)
    {
        DrawModeMeasurePeriod();
        lastDrawMode = 2;
    }
    else if (drawMode == 3)
    {
        DrawModeMeasureDuration();
        lastDrawMode = 3;
    }
    else if (drawMode == 4)
    {
        DrawModeCountPulse();
    }
    else if (drawMode == 5)
    {
        DrawModeTimeLabelLenght();
    }
    else if (drawMode == 6)
    {
        DrawModeTimeMeasure();
    }
    else if (drawMode == 7)
    {
        DrawModeNumberPeriods();
    }
    else
    {
        DrawModeMeasureFrequency();
    }
}
