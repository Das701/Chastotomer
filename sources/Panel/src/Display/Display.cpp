#include "defines.h"
#include "Log.h"
#include "FreqMeter.h"
#include "Settings.h"
#include "Display/Console.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Display/Font/FontBig.h"
#include "Display/Font/FontMid.h"
#include "Hardware/FPGA.h"
#include "Hardware/MathFPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Hint.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Menu/Pages/Settings/PagesSettings.h"
#include "Menu/Pages/PageIndication.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>


using namespace Primitives;


// Нарисовать подсказку
static void DrawHint(int x, int y);

// Нарисовать строку настроек текущего канала
static void DrawChannelSettings();

static void DrawScreen();
static void DrawInfo();
static void DrawData();

static void SetTopRow(int i);


static bool needRedraw = true;      // Если true, требуется перерисовка дисплея
static int autoHint = 0;
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


bool Display::sendToSCPI = false;


bool Display::DrawWelcomeScreen()
{
    if (TIME_MS < 1000)
    {
        for (int i = 0; i < NUM_PARTS; i++)
        {
            SetTopRow(i);
            BeginScene();
            Text("OAO МНИПИ, 43-96/2, Cherm V 1.2").Write(40, 100, Color::WHITE);
            EndScene();
        }

        return true;
    }

    return false;
}


static void SetTopRow(int i)
{
    topRow = i * (Display::HEIGHT / Display::NUM_PARTS);

    topDraw = topRow;
    bottomhDraw = topDraw + Display::HEIGHT / Display::NUM_PARTS;
}


void Display::Refresh()
{
    needRedraw = true;
}


void Display::Update()
{
    if (beginSecond == 0)
    {
        beginSecond = TIME_MS;
    }

    static uint lastUpdate = 0;
    static uint currentFramesInSec = 0;         // Столько кадров отрисовано за текущую секунду
    static uint currentTimePaintInSec = 0;      // Столько времени потрачено на отрисовку за текущую секунду

    if (TimeMeasure::ProgressBar::IsDrawable())
    {
        needRedraw = true;
    }

    if (needRedraw ||
        ((TIME_MS - lastUpdate) > 1000))
    {
        timeStart = TIME_MS;

        for (int i = 0; i < NUM_PARTS; i++)
        {
            DrawPartScreen(i, true);
        }

        timeFrame = TIME_MS - timeStart;

        lastUpdate = TIME_MS;

        currentFramesInSec++;
        currentTimePaintInSec += timeFrame;
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
}


void Display::DrawPartScreen(int num, bool debugInfo)
{
    debugInfo = false;
    
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

    if (num == 0 && debugInfo)
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


static void DrawScreen()
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
        TypeMeasure::Current().Draw(10, 57);
        
        CurrentModeMeasure::Draw(120, 57);

        PageModes::DrawParameters(270, 57);
        
        DrawHint(10, Display::HEIGHT - Item::HEIGHT - 30);
        
        DrawChannelSettings();
        
        DrawInfo();
        
        Menu::Draw();
        
        DrawData();

        //Indicator::Test(10, 150, Color::WHITE);

        TimeMeasure::ProgressBar::Draw(0, 0);
    }
}


static void DrawChannelSettings()
{
    if (Display::InDrawingPart(17, 20))
    {
        Rectangle(460, 30).FillRounded(10, 15, 2, Color::GREEN_20, Color::GREEN_20);

        Text(Menu::ChannelSettings()).Write(20, 21, Color::WHITE);
    }
}


static void DrawHint(int x, int y)
{
    int dX = 4;
    int dY = 6;

    if((TIME_MS < (uint)(autoHint + 10000)) && autoFlag == true)
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
                autoHint = (int)TIME_MS;
                autoFlag = true;
            }
            else
            {
                Rectangle(360, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::GREEN_20);
                Text("Установка уровня синхр.").Write(x + dX, y + dY, Color::WHITE);
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
        if(CURRENT_CHANNEL_IS_A && (PageModesA::modeMeasureFrequency.IsTachometer() && (PageModesA::typeMeasure.IsFrequency())))
        {
        }
        else if(CURRENT_CHANNEL_IS_B && (PageModesB::modeMeasureFrequency.IsTachometer() && PageModesB::typeMeasure.IsFrequency()))
        {
        }
        else if(CURRENT_CHANNEL_IS_A && (PageModesA::typeMeasure.IsCountPulse()) && (PageModesA::modeMeasureCountPulse == ModeMeasureCountPulse::StartStop))
        {
        }
        else if(CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsCountPulse() && (PageModesB::modeMeasureCountPulse == ModeMeasureCountPulse::StartStop))
        {
        }
        else
        {
            Text("M").Write(430, 100);
        } 
    }

    if(FreqMeter::modeTest.IsEnabled())
    {
        Text("Тест").Write(430, 120);
    }

    if(PageIndication::refGenerator == RefGenerator::External)
    {
        Text("Внеш Г").Write(420, 160);
    }

    if((CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsCountPulse() && PageModesA::modeMeasureCountPulse == ModeMeasureCountPulse::StartStop) ||
       (CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsCountPulse() && PageModesB::modeMeasureCountPulse == ModeMeasureCountPulse::StartStop))
    {
        Text(PageModesA::StartStop() ? "Старт" : "Стоп").Write(430, 60);
    }

    if(PageIndication::launchSource == LaunchSource::OneTime)
    {
        if(PageIndication::OnceLaunch() == true)
        {
            if(second == 0)
            {
                second = (int)TIME_MS;
            }
            Text("ПУСК").Write(430, 80);
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

static void DrawData()
{
    if (Display::InDrawingPart(150, 50))
    {
        String data = MathFPGA::Data::GiveDigits();

        if (data[0] != 0)
        {
            if (std::isdigit(data[0]) != 0 || data[0] == ' ' || data[0] == '-')         // Значит, есть данные
            {
                FontBig::Write(data.c_str(), 10, 150, Color::WHITE);
            }
            else
            {
                Font::Set(TypeFont::GOSTB28B);
                Text(data.c_str()).Write(50, 165, Color::WHITE);
                Font::Set(TypeFont::GOSTAU16BOLD);
            }
        }
    }

    if (Display::InDrawingPart(170, 50))
    {
        FontMid::Write(MathFPGA::Data::GiveUnits().c_str(), 360, 170, Color::WHITE);
    }

    if ((CURRENT_CHANNEL_IS_A && PageModesA::modeMeasureFrequency.IsTachometer()) ||
        (CURRENT_CHANNEL_IS_B && PageModesB::modeMeasureFrequency.IsTachometer()))
    {
        Text("60s").Write(2, 87, 60, Color::WHITE);
    }
}


int Display::TopRow()
{
    return topRow;
}


void Display::SendToSCPI()
{
    sendToSCPI = true;
}


#ifdef WIN32

bool Display::InDrawingPart(int, int)
{
    return true;

#else

bool Display::InDrawingPart(int y, int height)
{
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

