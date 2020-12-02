#include "defines.h"
#include "Log.h"
#include "Settings.h"
#include "Display/Console.h"
#include "Display/Display.h"
#include "Display/Objects.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/FPGA.h"
#include "Hardware/FreqMeter.h"
#include "Hardware/MathFPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Hint.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Menu/Pages/PageIndication.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


using namespace Primitives;


// Нарисовать подсказку
static void DrawHint(int x, int y);

static void DrawInfo();

static void SetTopRow(int i);


static bool needRedraw = true;      // Если true, требуется перерисовка дисплея
static uint timeAutoHint = 0;
static bool autoFlag = false;
static int second = 0;
static int topRow = 0;


static uint timeStart = 0;
static uint timeFrame = 0;
static uint fps = 0;                // Столько кадров отрисовано за последнюю секунду
static uint beginSecond = 0;        // В это время началась последняя секунда
static uint timePaint = 0;          // Время отрисовки за секунду


static int topDraw = 0;             // Верхний у отрисовываемой части экрана
static int bottomhDraw = 0;         // Нижний у отрисовываемой части экрана

static int width = Display::PHYSICAL_WIDTH;
static int height = Display::PHYSICAL_HEIGHT;


bool Display::sendToSCPI = false;
bool Display::drawingScene = false;


struct Coord
{
    int x;
    int y;
};


static int yString = 110;

static Coord coordMemory = { 15, yString };
static Coord coordTest = { 40, yString };
static Coord coordExtGenerator = { 95, yString };
static Coord coordLaunch = { 130, yString };


static const int MAX_OBJECTS = 10;
static Object *objects[MAX_OBJECTS];
static int numObjects = 0;

static DataZone sDataZone;
DataZone *Display::zoneData = &sDataZone;

static ProgressBarTimeMeasureZone sProgressBarTimeMeasureZone;
ProgressBarTimeMeasureZone *Display::zoneProgressBarTimeMeasure = &sProgressBarTimeMeasureZone;


static void AddObject(Object *object)
{
    objects[numObjects++] = object;
}


void Display::Init()
{
    InitHardware();

    Font::Set(TypeFont::GOSTAU16BOLD);

    Font::SetSpacing(2);

    AddObject(zoneData);
    AddObject(zoneProgressBarTimeMeasure);
}


static void DrawValue(char *string, int x, int y)
{
    Rectangle(Font::GetLengthText(string) + 5, 20).FillRounded(x - 3, y - 3, 2, Color::GREEN_20, Color::GRAY_20);

    Text(string).Write(x, y, Color::WHITE);
}


void Display::DrawWelcomeScreen()
{
    uint startTime = TIME_MS;
    
    while (TIME_MS - startTime < 3000)
    {
        for (int i = 0; i < NUM_PARTS; i++)
        {
            SetTopRow(i);
            BeginScene();
            Text("OAO МНИПИ, 43-96/2, Cherm V 1.2").Write(100, 110, Color::WHITE);
            EndScene();
        }
    }
}


static void SetTopRow(int i)
{
    topRow = i * (Display::PHYSICAL_HEIGHT / Display::NUM_PARTS);

    topDraw = topRow;
    bottomhDraw = topDraw + Display::PHYSICAL_HEIGHT / Display::NUM_PARTS;
}


void Display::Refresh()
{
    needRedraw = true;

    for (int i = 0; i < numObjects; i++)
    {
        objects[i]->Refresh();
    }
}


void Display::Update()
{
#ifdef WIN32

    BeginScene();
    DrawScreen();
    EndScene();

#else 

    static uint currentFramesInSec = 0;         // Столько кадров отрисовано за текущую секунду
    static uint currentTimePaintInSec = 0;      // Столько времени потрачено на отрисовку за текущую секунду
    static char prevHint = 0;

    zoneProgressBarTimeMeasure->Refresh();      // Прогресс-бар будем перерисовывать каждый кадр

    if (Hint::Text()[0] != prevHint)            // Если изменилась подсказка - перерисовываем экран
    {
        prevHint = Hint::Text()[0];

        Display::Refresh();
    }

    if (timeAutoHint != 0U && (TIME_MS - timeAutoHint) > 10000)
    {
        timeAutoHint = 0U;
        Display::Refresh();
    }

    if (needRedraw)
    {
        timeStart = TIME_MS;

        for (int i = 0; i < NUM_PARTS; i++)
        {
            DrawPartScreen(i, true);
        }

        timeFrame = TIME_MS - timeStart;
        currentFramesInSec++;
        currentTimePaintInSec += timeFrame;
    }

    for (int i = 0; i < numObjects; i++)
    {
        objects[i]->Update(Object::ModeDraw::ToHardware);
    }

    needRedraw = false;

    if (TIME_MS >= beginSecond + 1000)
    {
        fps = currentFramesInSec;
        currentFramesInSec = 0;
        beginSecond = TIME_MS;
        timePaint = currentTimePaintInSec;
        currentTimePaintInSec = 0;
    }

#endif
}


void Display::DrawPartScreen(int num, bool debugInfo)
{
    SetTopRow(num);

    if (num == 0)
    {
        timeStart = TIME_MS;
    }

    Display::BeginScene();

    DrawScreen();

    if (num == 0)
    {
        Console::Draw();
    }

    if (num == 0 && debugInfo && gset.showStatistics)
    {
        Text(String("%d", timeFrame)).Write(440, 0, Color::BLACK);
        Text(String("%d", fps)).Write(440, 15);
        Text(String("%d", timePaint)).Write(440, 30);
    }

    Display::EndScene();

    if (num == Display::NUM_PARTS)
    {
        timeFrame = TIME_MS - timeStart;
    }
}


void Display::DrawScreen()
{
    if(PageIndication::calibration.Is(Calibration::Pressed))
    {
        Text("---Режим Калибровка---").Write(40, 20, Color::WHITE);
        Text("Нажмите ЭНК. для сохранения").Write(5, 50);
        Text("Нажмите любую клавишу для выхода").Write(5, 90);
        Text(SU::Int2String(FPGA::CalibNumber()).c_str()).Write(5, 120);
    }
    else
    {
        Channel::Current()->DrawMode(10, 57);

        Channel::Current()->DrawSettings(10, 15);
        
        DrawHint(10, Display::PHYSICAL_HEIGHT - Item::HEIGHT - 30);
        
        DrawInfo();

        for (int i = 0; i < numObjects; i++)
        {
            objects[i]->Update(Object::ModeDraw::ToBuffer);
        }

        Menu::Draw();
    }
}


static void DrawHint(int x, int y)
{
    int dX = 4;
    int dY = 6;

    if((TIME_MS < timeAutoHint + 10000) && timeAutoHint != 0 && autoFlag)
    {
        Rectangle(360, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::GREEN_20);

        Text(MathFPGA::Auto::Give().c_str()).Write(x + dX, y + dY, Color::WHITE);

        FreqMeter::UnloadAuto();
    }
    else
    {
        if(FPGA::AutoMode())
        {
            if((MathFPGA::Auto::Mid() != 0) || (MathFPGA::Auto::Max() != 0) || (MathFPGA::Auto::Min() != 0))
            {
                Rectangle(360, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::GREEN_20);

                Text(MathFPGA::Auto::Give().c_str()).Write(x + dX, y + dY, Color::WHITE);
                FPGA::SwitchAuto();
                timeAutoHint = TIME_MS;
                autoFlag = true;
            }
            else
            {
                Rectangle(360, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::GREEN_20);
                Text("Установка уровня синхронизации").Write(x + dX, y + dY, Color::WHITE);
                timeAutoHint = 0;
            }
        }
        else
        {
            if (Hint::Text()[0] != 0)
            {
                Rectangle(360, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::GREEN_20);
                Text(Hint::Text()).Write(x + dX, y + dY, Color::WHITE);
            }
            autoFlag = false;
        }
    }
}


static void DrawInfo()
{
    if(PageIndication::memoryMode == MemoryMode::On)
    {
        if(CURRENT_CHANNEL_IS_A && Channel::A->mod.modeFrequency.IsTachometer() && (Channel::A->mod.typeMeasure.IsFrequency()))
        {
        }
        else if(CURRENT_CHANNEL_IS_B && Channel::B->mod.modeFrequency.IsTachometer() && Channel::B->mod.typeMeasure.IsFrequency())
        {
        }
        else if(CURRENT_CHANNEL_IS_A && Channel::A->mod.typeMeasure.IsCountPulse() && Channel::A->mod.modeCountPulse.Is_StartStop())
        {
        }
        else if(CURRENT_CHANNEL_IS_B && Channel::B->mod.typeMeasure.IsCountPulse() && Channel::B->mod.modeCountPulse.Is_StartStop())
        {
        }
        else
        {
            DrawValue("M", coordMemory.x, coordMemory.y);
        } 
    }

    if(FreqMeter::modeTest.IsEnabled())
    {
        DrawValue("Тест", coordTest.x, coordTest.y);
    }

    if(PageIndication::refGenerator == RefGenerator::External)
    {
        DrawValue("ОГ", coordExtGenerator.x, coordExtGenerator.y);
    }

    if (PageIndication::launchSource.IsExternal())
    {
        DrawValue("Зап:внешн", coordLaunch.x, coordLaunch.y);
    }
    else if (PageIndication::launchSource.IsOneTime())
    {
        DrawValue("Зап:однокр", coordLaunch.x, coordLaunch.y);
    }

    if((CURRENT_CHANNEL_IS_A && Channel::A->mod.typeMeasure.IsCountPulse() && Channel::A->mod.modeCountPulse.Is_StartStop()) ||
       (CURRENT_CHANNEL_IS_B && Channel::B->mod.typeMeasure.IsCountPulse() && Channel::B->mod.modeCountPulse.Is_StartStop()))
    {
        Text(ModeStartStop::IsEnabled() ? "Старт" : "Стоп").Write(430, 60);
    }

    if(PageIndication::launchSource == LaunchSource::OneTime)
    {
        if(PageIndication::OnceLaunch() == true)
        {
            if(second == 0)
            {
                second = (int)TIME_MS;
            }
            Text("ПУСК").Write(430, 110);
            if((second + 1000) < (int)TIME_MS)
            {
                second = 0;
                PageIndication::OnceLaunchSwitchFalse();
            }
        }
        else
        {
            Text(" ").Write(430, 80);
        }
    }
}


int Display::TopRow()
{
    return topRow;
}


void Display::SendToSCPI()
{
    sendToSCPI = true;
    
    Refresh();
}


static int oldTopRow = 0;


void Display::Prepare(int w, int h)
{
    width = w;
    height = h;
    oldTopRow = topRow;
    topRow = 0;
}


void Display::Restore()
{
    width = PHYSICAL_WIDTH;
    height = PHYSICAL_HEIGHT;
    topRow = oldTopRow;
}


int Display::Width()
{
    return width;
}


int Display::Height()
{
    return height;
}


#ifdef WIN32

bool Display::InDrawingPart(int, int)
{
    return true;

#else

bool Display::InDrawingPart(int y, int height)
{
    if (!drawingScene)
    {
        return true;
    }

    int yBottom = y + height;

    if (y >= topDraw && y <= bottomhDraw)
    {
        return true;
    }

    if (y <= topDraw && yBottom >= bottomhDraw)
    {
        return true;
    }

    if (yBottom >= topDraw && yBottom <= bottomhDraw)
    {
        return true;
    }

    return false;

#endif

}

