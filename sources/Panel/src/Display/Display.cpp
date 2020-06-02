#include "Display.h"
#include "Primitives.h"
#include "Menu/Menu.h"
#include "Settings.h"
#include "Utils/String.h"
#include "stdio.h"
#include "Menu/Pages/PageModes.h"
#include "Menu/Pages/PageModesB.h"
#include "Menu/Pages/PageModesC.h"
#include "Menu/Pages/PageModesD.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageIndication.h"
#include "Text.h"
#include "Menu/MenuItems.h"
#include "Menu/Hint.h"
#include <cstdlib>
#include <cstring>
#include "Hardware/HAL.h"
#include "FreqMeter/FreqMeter.h"

using namespace Display::Primitives;
using Display::Text;

/// Нарисовать тип измерения
static void DrawTypeMeasure();
/// Нарисовать режим измерения
static void DrawModeMeasure();
/// Нарисовать подсказку
static void DrawHint();
/// Нарисовать статус-бар
static void DrawStatusBar();
static void DrawStatusBarA();
static void DrawStatusBarB();
static void DrawStatusBarC();
static void DrawStatusBarD();
/// Нарисовать строку настроек текущего канала
static void DrawChannelSettings();

static void DrawScreen();

static void DrawInfo();

static void DrawData();

extern int info;

static int autoHint = 0;
static bool autoFlag = false;

static void CalculateCoord(int &x, int &y, int sizeX, int sizeY)
{
    static int deltaX = 1;
    static int deltaY = 0;

    x += deltaX;
    y += deltaY;

    if (x < 0)
    {
        x = 0;
        deltaX = -deltaX;
    }
    if (y < 0)
    {
        y = 0;
        deltaY = 1;
    }
    if (x + sizeX > 250)
    {
        deltaX = -deltaX;
    }
    if (y + sizeY > 60)
    {
        deltaY = -1;
    }
}

static void Func3()
{
    static int x = 0;
    static int y = 0;

    int sizeX = 8;
    int sizeY = 3;

    static HLine line(sizeX);

    CalculateCoord(x, y, sizeX, sizeY);

    line.Draw(x, y, Color::WHITE);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Update()
{
    BeginScene(Color::BLACK);

    DrawScreen();
    
    Func3();

    EndScene();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawScreen()
{
    if(PageIndication::calibration.Is(Calibration::Pressed))
    {
        char buffer[20];
        Text("---Режим Калибровка---").Write(40, 20);
        Text("Нажмите ЭНК. для сохранения").Write(5, 50);
        Text("Нажмите любую клавишу для выхода").Write(5, 90);
        Int2String(PLIS::CalibNumber(), buffer);
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawChannelSettings()
{
    Text(Menu::ChannelSettings()).Write(102, 2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawTypeMeasure()
{
    int x = 0;
    int width = 100; 
    if (CURRENT_CHANNEL_IS_A)
    {
        Text(PageModes::typeMeasure.ToText()).Write(x, 10, width);
    }
    else if (CURRENT_CHANNEL_IS_B)
    {
        Text(PageModesB::typeMeasureB.ToText()).Write(x, 10, width);
    }
    else if (CURRENT_CHANNEL_IS_C)
    {
        Text(PageModesC::typeMeasureC.ToText()).Write(x, 10, width);
    }
    else if (CURRENT_CHANNEL_IS_D)
    {
        Text("Частота").Write(x, 10, width);
    }
    Rectangle(width, 30).Draw(x, 0, Color::WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawModeMeasure()
{
    if (CURRENT_CHANNEL_IS_A)
    {
        static const Enumeration *modes[4] =
        {
            &PageModes::modeMeasureFrequency,
            &PageModes::modeMeasurePeriod,
            &PageModes::modeMeasureDuration,
            &PageModes::modeMeasureCountPulse
        };
        Text(modes[PageModes::typeMeasure.value]->ToText()).Write(0, 40);
    }
    else if (CURRENT_CHANNEL_IS_B)
    {
        static const Enumeration *modesB[4] =
        {
            &PageModesB::modeMeasureFrequencyB,
            &PageModesB::modeMeasurePeriodB,
            &PageModesB::modeMeasureDurationB,
            &PageModesB::modeMeasureCountPulseB
        };
        Text(modesB[PageModesB::typeMeasureB.value]->ToText()).Write(0, 40);
    }
    else if (CURRENT_CHANNEL_IS_C)
    {
        static const Enumeration *modesC[2] =
        {
            &PageModesC::modeMeasureFrequencyC,
            &PageModesC::modeMeasureCountPulseC
        };
        Text(modesC[PageModesC::typeMeasureC.value]->ToText()).Write(0, 40);
    }   
    if (CURRENT_CHANNEL_IS_D)
    {  
        Text("Частота").Write(0, 40);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawHint()
{
    if((HAL_GetTick() < (autoHint + 10000)) && autoFlag == true)
    {
        Text(PLIS::GiveAuto()).Write(102, 22);
        FreqMeter::UnloadAuto();
    }
    else
    {
        if(PLIS::AutoMode())
        {
            
            if((PLIS::MidAuto() != 0) || (PLIS::MaxAuto() != 0) || (PLIS::MinAuto() != 0))
            {
                Text(PLIS::GiveAuto()).Write(102, 22);
                PLIS::SwitchAuto();
                autoHint = HAL_GetTick();
                autoFlag = true;
            }
            else
            {
                Text("Установка уровня синхр.").Write(102, 22);
            }
        }
        else
        {
            Text(Hint::Text()).Write(102, 22);
            autoFlag = false;
        }
    }
}

static void DrawInfo()
{
    if(PageIndication::memoryMode == MemoryMode::On)
    {
        Text("M").Write(430, 100);
    }
    if(FreqMeter::TestModeStatus() == true)
    {
        Text("Тест").Write(430, 120);
    }
    if(PageModes::typeMeasure == TypeMeasure::Period || PageModes::typeMeasure == TypeMeasure::Duration || PageModesB::typeMeasureB == TypeMeasureB::Period || PageModesB::typeMeasureB == TypeMeasureB::Duration)
    {
        Text("Метки").Write(430, 140);
        Text(PageModes::periodTimeLabels.ToText()).Write(430, 160);
        
    }
    switch (info)
    {
        case 1:
            Text("Right").Write(100, 100);
            break;
        case 2:
            Text("Left").Write(100, 100);
            break;
        case 3:
            Text("GovLeft").Write(100, 100);
            break;
        case 4:
            Text("GovRight").Write(100, 100);
            break;
        case 5:
            Text("Enter").Write(100, 100);
            break;
        case 6:
            Text("Count").Write(100, 100);
            break;
        case 7:
            Text("None").Write(100, 100);
            break;
        case 8:
            Text("Mode").Write(100, 100);
            break;
        case 9:
            Text("Indication").Write(100, 100);
            break;
        case 10:
            Text("Channels").Write(100, 100);
            break;
        case 11:
            Text("GovButton").Write(100, 100);
            break;
        case 12:
            Text("Esc").Write(100, 100);
            break;
        case 13:
            Text("Test").Write(100, 100);
            break;
        case 14:
            Text("Auto").Write(100, 100);
            break;
    }
}

static void DrawData()
{
    Text(PLIS::GiveData()).Write(120, 120);
    Rectangle(320, 90).Draw(70, 100, Color::WHITE);
}

static void DrawStatusBarA()
{
    static const Enumeration * const modes[TypeMeasure::Count] =
    {///            Frequency                         Period                         Duration                         CountPulse
        &PageModes::modeMeasureFrequency, &PageModes::modeMeasurePeriod, &PageModes::modeMeasureDuration, &PageModes::modeMeasureCountPulse
    };

    static const Enumeration * const enums[TypeMeasure::Count][6] =
    {
        {   /// ModeMeasureFrequency::
            &PageModes::timeMeasure,        /// Freq
            &PageModes::timeMeasure,        /// AC
            &PageModes::timeMeasure,      /// AB
            &PageModes::numberPeriods,        /// T_1
            &PageModes::timeMeasure         /// Tachometer
        },
        {   /// ModeMeasurePeriod::
            &PageModes::numberPeriods,      /// Period
            &PageModes::timeMeasure         /// F_1
        },
        {   /// ModeMeasureDuration::
            nullptr,                        /// Ndt
            &PageModes::numberPeriods,      /// Ndt_1
            nullptr,                        /// Ndt_1ns
            nullptr,                        /// Interval
            nullptr,                        /// S_1
            nullptr                         /// Phase
        },
        {   /// ModeMeasureCountPulse::
            nullptr,                        /// Manual
            nullptr,                        /// ATC
            &PageModes::numberPeriods       /// ATC_1
        }
    };

    const Enumeration *mode = modes[PageModes::typeMeasure];

    const Enumeration *toText = enums[PageModes::typeMeasure][mode->value];
    
    int y = 65;
    int x = 0;
    int width = 60;

    if (toText)
    {
        Text(toText->ToText()).Write(x + 2, y + 7, width, Color::WHITE);
    }

    Rectangle(width, 30).Draw(x, y, Color::WHITE);
}

static void DrawStatusBarB()
{
    static const Enumeration * const modesB[TypeMeasureB::Count] =
    {///            Frequency                         Period                         Duration                         CountPulse
        &PageModesB::modeMeasureFrequencyB, &PageModesB::modeMeasurePeriodB, &PageModesB::modeMeasureDurationB, &PageModesB::modeMeasureCountPulseB
    };

    static const Enumeration * const enumsB[TypeMeasureB::Count][6] =
    {
        {   /// ModeMeasureFrequency::
            &PageModes::timeMeasure,        /// Freq
            &PageModes::timeMeasure,      /// AC
            &PageModes::timeMeasure,      /// AB
            &PageModes::numberPeriods,        /// T_1
            &PageModes::timeMeasure         /// Tachometer
        },
        {   /// ModeMeasurePeriod::
            &PageModes::numberPeriods,      /// Period
            &PageModes::timeMeasure         /// F_1
        },
        {   /// ModeMeasureDuration::
            nullptr,                          /// Ndt
            &PageModes::numberPeriods,      /// Ndt_1
            nullptr,                          /// Ndt_1ns
            nullptr,                          /// Interval
            nullptr,                          /// S_1
            nullptr                           /// Phase
        },
        {   /// ModeMeasureCountPulse::
            nullptr,                          /// Manual
            nullptr,                          /// ATC
            &PageModes::numberPeriods       /// ATC_1
        }
    };

    const Enumeration *mode = modesB[PageModesB::typeMeasureB];

    const Enumeration *toText = enumsB[PageModesB::typeMeasureB][mode->value];

    int y = 65;
    int x = 0;
    int width = 60;

    if (toText)
    {
        Text(toText->ToText()).Write(x + 2, y + 7, width, Color::WHITE);
    }

    Rectangle(width, 30).Draw(x, y, Color::WHITE);
}

static void DrawStatusBarC()
{
    static const Enumeration * const modesC[TypeMeasureC::Count] =
    {///            Frequency                         CountPulse
        &PageModesC::modeMeasureFrequencyC, &PageModesC::modeMeasureCountPulseC
    };

    static const Enumeration * const enumsC[TypeMeasureC::Count][4] =
    {
        {   /// ModeMeasureFrequency::
            &PageModesC::timeMeasureC,        /// Freq
            &PageModesC::numberPeriodsC,      /// CA
            &PageModesC::numberPeriodsC,      /// CB
        },
        {   /// ModeMeasureCountPulse::
            nullptr,                          /// Manual
            nullptr,                          /// ATC
            nullptr,  
            &PageModesC::numberPeriodsC       /// ATC_1
        }
    };

    const Enumeration *mode = modesC[PageModesC::typeMeasureC];

    const Enumeration *toText = enumsC[PageModesC::typeMeasureC][mode->value];

    int y = 65;
    int x = 0;
    int width = 60;

    if (toText)
    {
        Text(toText->ToText()).Write(x + 2, y + 7, width, Color::WHITE);
    }

    Rectangle(width, 30).Draw(x, y, Color::WHITE);
}

static void DrawStatusBarD()
{
    int y = 65;
    int x = 0;
    int width = 60;
    Text(PageModesD::timeMeasureD.ToText()).Write(x + 2, y + 7, width, Color::WHITE);
    Rectangle(width, 30).Draw(x, y, Color::WHITE);
}

