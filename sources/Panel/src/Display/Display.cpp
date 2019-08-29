#include "Display.h"
#include "Primitives.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageModes.h"
#include "Text.h"


using namespace Display::Primitives;
using Display::Text;

extern int drawMode;
static int lastDrawMode = 1;

/// Нарисовать режим измерения
static void DrawMode();
/// Нарисовать режим измерения частоты
static void DrawModeMeasureFrequency();
/// Нарисовать режим измерения периода
static void DrawModeMeasurePeriod();
/// Нарисовать режим измерения длительности
static void DrawModeMeasureDuration();
/// Нарисовать режим счёта импульсов
static void DrawModeCountPulse();

/// Нарисовать период меток времени
static void DrawPeriodTimeLabels();
/// Нарисовать время счёта
static void DrawTimeMeasure();
/// Нарисовать число периодов
static void DrawNumberPeriods();
/// Нарисовать статус-бар
static void DrawStatusBar();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Update()
{
    BeginScene(Color::BLACK);

    Rectangle(256, 64).Draw(0, 0, Color::WHITE);

    DrawStatusBar();

    DrawMode();

    Menu::Draw();

    EndScene();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawModeMeasureFrequency()
{
    static const pString modes[5] = { "Частота", "f(A)/f(C)", "f(A)/f(B)", "f=1/T", "Тахометр" };

    Text(modes[PageModes::modeMeasureFrequency]).Write(10, 30);
    Text("Измерение частоты").Write(38, 15);
    Text("f").Write(18, 10);
    Rectangle(25, 15).Draw(10, 8, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawModeMeasurePeriod()
{
    static const pString modes[2] = { "Период", "T=1/f" };

    Text(modes[PageModes::modeMeasurePeriod]).Write(10, 30);
    Text("Измерение периода").Write(38, 15);
    Text("T").Write(18, 10);
    Rectangle(25, 15).Draw(10, 8, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawModeMeasureDuration()
{
    static const pString modes[6] = { "ndt", "<ndt>", "ndt/1нс", "Интервал", "1/S", "Фаза" };

    Text(modes[PageModes::modeMeasureDuration]).Write(10, 30);
    Text("Измерение длительности").Write(38, 15);
    Text("t").Write(18, 10);
    Rectangle(25, 15).Draw(10, 8, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawModeCountPulse()
{
    static const pString modes[3] = { "Ручн.", "А(tC)", "А(TC)" };

    Text(modes[PageModes::modeCountPulse]).Write(10, 30);
    Text("Счет числа импульсов").Write(38, 15);
    Text("Счет").Write(12, 10);
    Rectangle(25, 15).Draw(10, 8, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawPeriodTimeLabels()
{
    static const pString modes[6] = { "10-3", "10-4", "10-5", "10-6", "10-7", "10-8" };

    Text(modes[PageModes::periodTimeLabels]).Write(10, 30);
    Text("Длительность временных меток:").Write(38, 15);
    switch (lastDrawMode)
    {
    case 1:
        Text("f").Write(18, 10);
        break;
    case 2:
        Text("T").Write(18, 10);
        break;
    case 3:
        Text("t").Write(18, 10);
        break;
    }
    Rectangle(25, 15).Draw(10, 8, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawTimeMeasure()
{
    static const pString modes[6] = { "1ms", "10ms", "100ms", "1s", "10s", "100s" };

    Text(modes[PageModes::timeMeasure]).Write(10, 30);
    Text("Время счета:").Write(38, 15);
    switch (lastDrawMode)
    {
    case 1:
        Text("f").Write(18, 10);
        break;
    case 2:
        Text("T").Write(18, 10);
        break;
    case 3:
        Text("t").Write(18, 10);
        break;
    }
    Rectangle(25, 15).Draw(10, 8, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawNumberPeriods()
{
    static const pString modes[6] = { "1", "10", "100", "1K", "10K", "100K" };

    Text(modes[PageModes::numberPeriods]).Write(10, 30);
    Text("Число периодов измерения:").Write(38, 15);
    switch (lastDrawMode)
    {
    case 1:
        Text("f").Write(18, 10);
        break;
    case 2:
        Text("T").Write(18, 10);
        break;
    case 3:
        Text("t").Write(18, 10);
        break;
    }
    Rectangle(25, 15).Draw(10, 8, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawStatusBar()
{
//    if(statusBar == 1)
    {
        static const pString modes[6] = { "1ms", "10ms", "100ms", "1s", "10s", "100s" };
        Text(modes[PageModes::timeMeasure]).Write(8, 49);
    }
//    if(statusBar == 2)
    {
        static const pString modes[6] = { "1", "10", "100", "1K", "10K", "100K" };
        Text(modes[PageModes::numberPeriods]).Write(8, 49);
    }
    Rectangle(25, 15).Draw(5, 45, Color::WHITE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawMode()
{
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
        DrawPeriodTimeLabels();
    }
    else if (drawMode == 6)
    {
        DrawTimeMeasure();
    }
    else if (drawMode == 7)
    {
        DrawNumberPeriods();
    }
    else
    {
        DrawModeMeasureFrequency();
    }
}
