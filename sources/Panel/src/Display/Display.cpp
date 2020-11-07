#include "defines.h"
#include "FreqMeter.h"
#include "Settings.h"
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
#include "Menu/Pages/PageModesA.h"
#include "Menu/Pages/PageModesB.h"
#include "Menu/Pages/PageModesC.h"
#include "Menu/Pages/PageModesD.h"
#include "Menu/Pages/PagesSettings.h"
#include "Menu/Pages/PageIndication.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


using namespace Display::Primitives;
using Display::Text;


// Нарисовать тип измерения
static void DrawTypeMeasure();

// Нарисовать режим измерения
static void DrawModeMeasure();

// Нарисовать подсказку
static void DrawHint();

// Нарисовать статус-бар
static void DrawStatusBar();

// Нарисовать строку настроек текущего канала
static void DrawChannelSettings();

static void DrawStatusBarA();
static void DrawStatusBarB();
static void DrawStatusBarC();
static void DrawStatusBarD();
static void DrawScreen();
static void DrawInfo();
static void DrawData();

// Отрисовать очередную часть экрана
static void DrawPartScreen(int num);

static void SetTopRow(int i);


static bool needRedraw = true;      // Если true, требуется перерисовка дисплея
static int autoHint = 0;
static bool autoFlag = false;
static int second = 0;
static int topRow = 0;


static uint timeStart = 0;
static uint timeFull = 0;


static int topDraw = 0;             // Верхний у отрисовываемой части экрана
static int bottomhDraw = 0;         // Нижний у отрисовываемой части экрана


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
    if (needRedraw)
    {
        timeStart = TIME_MS;

        for (int i = 0; i < NUM_PARTS; i++)
        {
            DrawPartScreen(i);
        }

        timeFull = TIME_MS - timeStart;
    }

    needRedraw = false;
}


static void DrawPartScreen(int num)
{
    SetTopRow(num);

    if (num == 0)
    {
        timeStart = TIME_MS;
    }

    Display::BeginScene();

    if (num == 0)
    {
        Text(String("Пол %d", timeFull).c_str()).Write(400, 0, Color::WHITE);
    }

    DrawScreen();

    Display::EndScene();

    if (num == Display::NUM_PARTS)
    {
        timeFull = TIME_MS - timeStart;
    }
}


static void DrawScreen()
{
    if(PageIndication::calibration.Is(Calibration::Pressed))
    {
        char buffer[20];
        Text("---Режим Калибровка---").Write(40, 20, Color::WHITE);
        Text("Нажмите ЭНК. для сохранения").Write(5, 50);
        Text("Нажмите любую клавишу для выхода").Write(5, 90);
        SU::Int2String(FPGA::CalibNumber(), buffer);
        Text(buffer).Write(5, 120);
    }
    else
    {
        DrawStatusBar();
            
        DrawTypeMeasure();
        
        DrawModeMeasure();
        
        DrawHint();
        
        DrawChannelSettings();
        
        DrawInfo();
        
        Menu::Draw();
        
        DrawData();
    }
}


static void DrawTypeMeasure()
{
    int x = 0;
    int width = 100; 

    Rectangle(width, 30).FillRounded(x, 15, 2, Color::GREEN_20, Color::WHITE);

    Color::WHITE.SetAsCurrent();

    if (CURRENT_CHANNEL_IS_A)
    {
        Text(PageModesA::typeMeasure.ToText()).Write(x, 25, width);
    }
    else if (CURRENT_CHANNEL_IS_B)
    {
        Text(PageModesB::typeMeasure.ToText()).Write(x, 25, width);
    }
    else if (CURRENT_CHANNEL_IS_C)
    {
        Text(PageModesC::typeMeasure.ToText()).Write(x, 25, width);
    }
    else if (CURRENT_CHANNEL_IS_D)
    {
        Text("Частота").Write(x, 25, width);
    }
}


static void DrawModeMeasure()
{
    if (CURRENT_CHANNEL_IS_A)
    {
        static const Enumeration *modes[4] =
        {
            &PageModesA::modeMeasureFrequency,
            &PageModesA::modeMeasurePeriod,
            &PageModesA::modeMeasureDuration,
            &PageModesA::modeMeasureCountPulse
        };
        Text(modes[PageModesA::typeMeasure.value]->ToText()).Write(0, 55);
    }
    else if (CURRENT_CHANNEL_IS_B)
    {
        static const Enumeration *modesB[4] =
        {
            &PageModesB::modeMeasureFrequency,
            &PageModesB::modeMeasurePeriod,
            &PageModesB::modeMeasureDuration,
            &PageModesB::modeMeasureCountPulse
        };
        Text(modesB[PageModesB::typeMeasure.value]->ToText()).Write(0, 55);
    }
    else if (CURRENT_CHANNEL_IS_C)
    {
        static const Enumeration *modesC[2] =
        {
            &PageModesC::modeMeasureFrequency,
            &PageModesC::modeMeasureCountPulse
        };
        Text(modesC[PageModesC::typeMeasure.value]->ToText()).Write(0, 55);
    }   
    if (CURRENT_CHANNEL_IS_D)
    {  
        Text("Частота").Write(0, 55);
    }
}


static void DrawStatusBar()
{
    if (CURRENT_CHANNEL_IS_A)
    {
        DrawStatusBarA();
    }
    else if (CURRENT_CHANNEL_IS_B)
    {
        DrawStatusBarB();
    }
    else if (CURRENT_CHANNEL_IS_C)
    {
        DrawStatusBarC();
    }
    else if (CURRENT_CHANNEL_IS_D)
    {
        DrawStatusBarD();
    }
}

static void DrawChannelSettings()
{
    if (Display::InDrawingPart(17, 20))
    {
        Rectangle(360, 30).FillRounded(105, 15, 2, Color::GREEN_20, Color::GREEN_20);

        Text(Menu::ChannelSettings()).Write(108, 21, Color::WHITE);
    }
}


static void DrawHint()
{
    int x = 105;
    int y = 60;
    int dX = 4;
    int dY = 6;

    if((TIME_MS < (uint)(autoHint + 10000)) && autoFlag == true)
    {
        Rectangle(360, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::GREEN_20);

        Text(MathFPGA::Auto::Give()).Write(x + dX, y + dY, Color::WHITE);

        FreqMeter::UnloadAuto();
    }
    else
    {
        if(FPGA::AutoMode())
        {
            if((MathFPGA::Auto::Mid() != 0) || (MathFPGA::Auto::Max() != 0) || (MathFPGA::Auto::Min() != 0))
            {
                Rectangle(360, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::GREEN_20);

                Text(MathFPGA::Auto::Give()).Write(x + dX, y + dY, Color::WHITE);
                FPGA::SwitchAuto();
                autoHint = (int)TIME_MS;
                autoFlag = true;
            }
            else
            {
                Rectangle(360, 30).FillRounded(105, 60, 2, Color::GREEN_20, Color::GREEN_20);
                Text("Установка уровня синхр.").Write(x + dX, y + dY, Color::WHITE);
            }
        }
        else
        {
            if (Hint::Text()[0] != 0)
            {
                Rectangle(360, 30).FillRounded(105, 60, 2, Color::GREEN_20, Color::GREEN_20);
                Text(Hint::Text()).Write(x + dX, y + dY, Color::WHITE);
            }
            autoFlag = false;
        }
    }
}

//static bool xMark = false;

static void DrawInfo()
{
    if(PageIndication::memoryMode == MemoryMode::On)
    {
        if(CURRENT_CHANNEL_IS_A && (PageModesA::modeMeasureFrequency.IsTachometer() && (PageModesA::typeMeasure.IsFrequency())))
        {
        }
        else if(CURRENT_CHANNEL_IS_B && ((PageModesB::modeMeasureFrequency == ModeMeasureFrequencyB::Tachometer) && PageModesB::typeMeasure.IsFrequency()))
        {
        }
        else if(CURRENT_CHANNEL_IS_A && (PageModesA::typeMeasure.IsCountPulse()) && (PageModesA::modeMeasureCountPulse == ModeMeasureCountPulseA::StartStop))
        {
        }
        else if(CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsCountPulse() && (PageModesB::modeMeasureCountPulse == ModeMeasureCountPulseB::StartStop))
        {
        }
        else
        {
            Text("M").Write(430, 100);
        } 
    }

    if(FreqMeter::TestModeStatus() == true)
    {
        Text("Тест").Write(430, 120);
    }

    if(PageIndication::refGenerator == RefGenerator::External)
    {
        Text("Внеш Г").Write(420, 160);
    }

    if(PageModesA::typeMeasure.IsPeriod() || (PageModesA::typeMeasure.IsDuration() && !PageModesA::modeMeasureDuration.Is_Ndt_1ns()) 
      || PageModesB::typeMeasure.IsPeriod() || (PageModesB::typeMeasure.IsDuration() && !PageModesA::modeMeasureDuration.Is_Ndt_1ns()))
    {
        Text("Метки").Write(430, 140);
        Text(PageModesA::periodTimeLabels.ToText()).Write(430, 160);
        
    }

    if((CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsCountPulse() && PageModesA::modeMeasureCountPulse == ModeMeasureCountPulseA::StartStop) ||
       (CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsCountPulse() && PageModesB::modeMeasureCountPulse == ModeMeasureCountPulseB::StartStop))
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
        FontBig::Write(MathFPGA::Measure::GiveData(), 10, 150, Color::WHITE);
    }

    if (Display::InDrawingPart(170, 50))
    {
        FontMid::Write(MathFPGA::GiveSpec(), 344, 170, Color::WHITE);
    }

    if ((CURRENT_CHANNEL_IS_A && PageModesA::modeMeasureFrequency.IsTachometer()) ||
        (CURRENT_CHANNEL_IS_B && PageModesB::modeMeasureFrequency.IsTachometer()))
    {
        Text("60s").Write(2, 87, 60, Color::WHITE);
    }
}

static void DrawStatusBarA()
{
    static const Enumeration * const modes[TypeMeasureAB::Count] =
    {//            Frequency                         Period                         Duration                         CountPulse
        &PageModesA::modeMeasureFrequency, &PageModesA::modeMeasurePeriod, &PageModesA::modeMeasureDuration, &PageModesA::modeMeasureCountPulse
    };

    static const Enumeration * const enums[TypeMeasureAB::Count][6] =
    {
        {   // ModeMeasureFrequency::
            &PageModesA::timeMeasure,        // Freq
            &PageModesA::numberPeriods,      // AB
            &PageModesA::timeMeasure,        // AC
            &PageModesA::numberPeriods,      // T_1
            nullptr,                        // Tachometer
            nullptr                         // Comparator
        },
        {   // ModeMeasurePeriod::
            &PageModesA::numberPeriods,      // Period
            &PageModesA::timeMeasure         // F_1
        },
        {   // ModeMeasureDuration::
            nullptr,                        // Ndt
            nullptr,      // Ndt_1
            nullptr,                        // Ndt_1ns
            nullptr,                        // Interval
            nullptr,                        // S_1
            nullptr                         // Phase
        },
        {   // ModeMeasureCountPulseA::
            nullptr,                        // ATC
            &PageModesA::numberPeriods,      // ATC_1
            nullptr
        }
    };

    const Enumeration *mode = modes[PageModesA::typeMeasure];

    const Enumeration *toText = enums[PageModesA::typeMeasure][mode->value];
   
    int y = 80;
    int width = 60;

    Rectangle(width, 30).FillRounded(0, y, 2, Color::GREEN_20, Color::WHITE);

    if (toText)
    {
        Text(toText->ToText()).Write(2, y + 7, width, Color::WHITE);
    }
}

static void DrawStatusBarB()
{
    static const Enumeration * const modesB[TypeMeasureAB::Count] =
    {//            Frequency                         Period                         Duration                         CountPulse
        &PageModesB::modeMeasureFrequency, &PageModesB::modeMeasurePeriod, &PageModesB::modeMeasureDuration, &PageModesB::modeMeasureCountPulse
    };

    static const Enumeration * const enumsB[TypeMeasureAB::Count][6] =
    {
        {   // ModeMeasureFrequency::
            &PageModesA::timeMeasure,        // Freq
            &PageModesA::numberPeriods,      // BA
            &PageModesA::timeMeasure,        // BC
            &PageModesA::numberPeriods,      // T_1
            nullptr                         // Tachometer
        },
        {   // ModeMeasurePeriod::
            &PageModesA::numberPeriods,      // Period
            &PageModesA::timeMeasure         // F_1
        },
        {   // ModeMeasureDuration::
            nullptr,                          // Ndt
            nullptr,      // Ndt_1
            nullptr,                          // Ndt_1ns
            nullptr,                          // Interval
            nullptr,                          // S_1
            nullptr                           // Phase
        },
        {   // ModeMeasureCountPulseA::
            nullptr,                          // ATC
            &PageModesA::numberPeriods,       // ATC_1
            nullptr
        }
    };

    const Enumeration *mode = modesB[PageModesB::typeMeasure];

    const Enumeration *toText = enumsB[PageModesB::typeMeasure][mode->value];

    int y = 80;
    int width = 60;

    Rectangle(width, 30).FillRounded(0, y, 2, Color::GREEN_20, Color::WHITE);

    if (toText)
    {
        Text(toText->ToText()).Write(2, y + 7, width, Color::WHITE);
    }
}

static void DrawStatusBarC()
{
    static const Enumeration * const modesC[TypeMeasureC::Count] =
    {//            Frequency                         CountPulse
        &PageModesC::modeMeasureFrequency, &PageModesC::modeMeasureCountPulse
    };

    static const Enumeration * const enumsC[TypeMeasureC::Count][5] =
    {
        {   // ModeMeasureFrequency::
            &PageModesC::timeMeasure,        // Freq
            &PageModesC::numberPeriods,       // CA
            &PageModesC::numberPeriods,       // CB
        },
        {   // ModeMeasureCountPulseA::
            nullptr,                          // Manual
            nullptr,                          // ATC
            &PageModesC::numberPeriods,  
            &PageModesC::numberPeriods       // ATC_1
        }
    };

    const Enumeration *mode = modesC[PageModesC::typeMeasure];

    const Enumeration *toText = enumsC[PageModesC::typeMeasure][mode->value];

    int y = 80;
    int width = 60;

    Rectangle(width, 30).FillRounded(0, y, 2, Color::GREEN_20, Color::WHITE);

    if (toText)
    {
        Text(toText->ToText()).Write(2, y + 7, width, Color::WHITE);
    }
}

static void DrawStatusBarD()
{
    int y = 80;
    int width = 60;
    Rectangle(width, 30).FillRounded(0, y, 2, Color::GREEN_20, Color::WHITE);
    Text(PageModesD::timeMeasure.ToText()).Write(2, y + 7, width, Color::WHITE);
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
