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
#include "Menu/Pages/Pages.h"
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

// Отрисовать верхнюю часть экрана
static void DrawTopPart();

// Отрисовать нижнюю часть экрана
static void DrawBottomPart();


static bool needRedraw = true;      // Если true, требуется перерисовка дисплея
static int autoHint = 0;
static bool autoFlag = false;
static int second = 0;
static int topRow = 0;

void Display::DrawWelcomeScreen()
{   
    BeginScene();
    Text("OAO МНИПИ, 43-96/2, Cherm V 1.2").Write(40, 100);
    EndScene();
}


void Display::Refresh()
{
    needRedraw = true;
}


void Display::Update()
{
    if (needRedraw)
    {
        DrawTopPart();

        DrawBottomPart();
    }

    needRedraw = false;
}


static void DrawTopPart()
{
    topRow = 0;

    Display::BeginScene();

    DrawScreen();

    Display::EndScene();
}


static void DrawBottomPart()
{
    topRow = Display::HEIGHT / 2;

    Display::BeginScene();

    DrawScreen();

    Display::EndScene();
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


void DrawChannelSettings()
{
    Text(Menu::ChannelSettings()).Write(102, 17);
}


static void DrawTypeMeasure()
{
    int x = 0;
    int width = 100; 
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
    Rectangle(width, 30).Draw(x, 15, Color::WHITE);
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

static void DrawHint()
{
    if((TIME_MS < (uint)(autoHint + 10000)) && autoFlag == true)
    {
        Text(MathFPGA::Auto::Give()).Write(102, 37);
        FreqMeter::UnloadAuto();
    }
    else
    {
        if(FPGA::AutoMode())
        {
            if((MathFPGA::Auto::Mid() != 0) || (MathFPGA::Auto::Max() != 0) || (MathFPGA::Auto::Min() != 0))
            {
                Text(MathFPGA::Auto::Give()).Write(102, 37);
                FPGA::SwitchAuto();
                autoHint = (int)TIME_MS;
                autoFlag = true;
            }
            else
            {
                Text("Установка уровня синхр.").Write(102, 37);
            }
        }
        else
        {
            Text(Hint::Text()).Write(102, 37);
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
    FontBig::BigStringProp_print(MathFPGA::Measure::GiveData(), 10, 150, Color::WHITE);
    FontMid::MidStringProp_print(MathFPGA::GiveSpec(), 344, 170, Color::WHITE);
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

    if (toText)
    {
        Text(toText->ToText()).Write(2, y + 7, width, Color::WHITE);
    }

    Rectangle(width, 30).Draw(0, y, Color::WHITE);
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

    if (toText)
    {
        Text(toText->ToText()).Write(2, y + 7, width, Color::WHITE);
    }

    Rectangle(width, 30).Draw(0, y, Color::WHITE);
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

    if (toText)
    {
        Text(toText->ToText()).Write(2, y + 7, width, Color::WHITE);
    }

    Rectangle(width, 30).Draw(0, y, Color::WHITE);
}

static void DrawStatusBarD()
{
    int y = 80;
    int width = 60;
    Text(PageModesD::timeMeasure.ToText()).Write(2, y + 7, width, Color::WHITE);
    Rectangle(width, 30).Draw(0, y, Color::WHITE);
}


int Display::TopRow()
{
    return topRow;
}
