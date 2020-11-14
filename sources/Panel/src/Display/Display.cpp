#include "defines.h"
#include "Log.h"
#include "FreqMeter.h"
#include "Settings.h"
#include "Display/Console.h"
#include "Display/Display.h"
#include "Display/Indicator.h"
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


// ���������� ���������
static void DrawHint(int x, int y);

// ���������� ������ �������� �������� ������
static void DrawChannelSettings();

static void DrawScreen();
static void DrawInfo();
static void DrawData();

// ���������� ��������� ����� ������
static void DrawPartScreen(int num);

static void SetTopRow(int i);


static bool needRedraw = true;      // ���� true, ��������� ����������� �������
static int autoHint = 0;
static bool autoFlag = false;
static int second = 0;
static int topRow = 0;


static uint timeStart = 0;
static uint timeFrame = 0;
static uint fps = 0;                // ������� ������ ���������� �� ��������� �������
static uint beginSecond = 0;        // � ��� ����� �������� ��������� �������
static uint timePaint = 0;          // ����� ��������� �� �������


static int topDraw = 0;             // ������� � �������������� ����� ������
static int bottomhDraw = 0;         // ������ � �������������� ����� ������


bool Display::DrawWelcomeScreen()
{
    if (TIME_MS < 1000)
    {
        for (int i = 0; i < NUM_PARTS; i++)
        {
            SetTopRow(i);
            BeginScene();
            Text("OAO �����, 43-96/2, Cherm V 1.2").Write(40, 100, Color::WHITE);
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
    static uint currentFramesInSec = 0;         // ������� ������ ���������� �� ������� �������
    static uint currentTimePaintInSec = 0;      // ������� ������� ��������� �� ��������� �� ������� �������

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
            DrawPartScreen(i);
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


static void DrawPartScreen(int num)
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
        LOG_WRITE("Update");
        Console::Draw();
    }

    if (num == 0)
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
        Text("---����� ����������---").Write(40, 20, Color::WHITE);
        Text("������� ���. ��� ����������").Write(5, 50);
        Text("������� ����� ������� ��� ������").Write(5, 90);
        Text(SU::Int2String(FPGA::CalibNumber()).c_str()).Write(5, 120);
    }
    else
    {
        TypeMeasure::Current().Draw(10, 57);
        
        CurrentModeMeasure::Draw(120, 57);

        CurrentModeMeasure::DrawParameters(270, 57);
        
        DrawHint(10, Display::HEIGHT - Item::HEIGHT - 30);
        
        DrawChannelSettings();
        
        DrawInfo();
        
        Menu::Draw();
        
        DrawData();

        //Indicator::Test(10, 150, Color::WHITE);

        TimeMeasure::ProgressBar::Draw(0, 0);

        if (ModeMeasureFrequency::Current().IsComparator())
        {
            int y = 93;
            int dY = 16;
            Text(String("%u", MathFPGA::curFX)).Write(10, y, Color::BLACK);
            Text(String("%u", MathFPGA::curTIZM)).Write(10, y + dY);
            Text(String("%u", MathFPGA::curNKAL)).Write(10, y + 2 * dY);

            int width = 90;

            if (MathFPGA::sFX.Size() > 0)
            {
                int counter = 0;

                for (int i = MathFPGA::sFX.Size() - 1; i > MathFPGA::sFX.Size() - 5 && i >= 0; i--)
                {
                    Text(String("%u", MathFPGA::sFX[i])).Write(10 + width * (counter + 1), y);
                    Text(String("%u", MathFPGA::sTIZM[i])).Write(10 + width * (counter + 1), y + dY);
                    Text(String("%u", MathFPGA::sNKAL[i])).Write(10 + width * (counter + 1), y + 2 * dY);
                    Text(String("%.3e", MathFPGA::values[i])).Write(10 + width * (counter++ + 1), y + 3 * dY);
                }
            }
        }
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
                Text("��������� ������ �����.").Write(x + dX, y + dY, Color::WHITE);
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
        Text("����").Write(430, 120);
    }

    if(PageIndication::refGenerator == RefGenerator::External)
    {
        Text("���� �").Write(420, 160);
    }

    if(PageModesA::typeMeasure.IsPeriod() || (PageModesA::typeMeasure.IsDuration() && !PageModesA::modeMeasureDuration.Is_Ndt_1ns()) 
      || PageModesB::typeMeasure.IsPeriod() || (PageModesB::typeMeasure.IsDuration() && !PageModesA::modeMeasureDuration.Is_Ndt_1ns()))
    {
        Text("�����").Write(430, 140);
        Text(PageModesA::timeLabels.ToString()).Write(430, 160);
        
    }

    if((CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsCountPulse() && PageModesA::modeMeasureCountPulse == ModeMeasureCountPulse::StartStop) ||
       (CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsCountPulse() && PageModesB::modeMeasureCountPulse == ModeMeasureCountPulse::StartStop))
    {
        Text(PageModesA::StartStop() ? "�����" : "����").Write(430, 60);
    }

    if(PageIndication::launchSource == LaunchSource::OneTime)
    {
        if(PageIndication::OnceLaunch() == true)
        {
            if(second == 0)
            {
                second = (int)TIME_MS;
            }
            Text("����").Write(430, 80);
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
        String data = MathFPGA::Measure::GiveData();

        if (data[0] != 0)
        {
            if (std::isdigit(data[0]) != 0 || data[0] == ' ' || data[0] == '-')         // ������, ���� ������
            {
                //FontBig::Write(data.c_str(), 10, 150, Color::WHITE);
                Indicator::DrawDataAboutRight(data.c_str(), 360, 150, Color::WHITE, Color::GREEN_10);
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
        FontMid::Write(MathFPGA::Measure::GiveUnits().c_str(), 360, 170, Color::WHITE);
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
