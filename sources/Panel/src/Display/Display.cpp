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
#include "Hardware/HAL.h"
#include "FreqMeter/FreqMeter.h"
#include "Display/Font/BigFont1.h"
#include "Display/Font/FontMid.h"
#include "Utils/StringUtils.h"
#include <cstdlib>
#include <cstring>

int second = 0;
using namespace Display::Primitives;
using Display::Text;

/// ���������� ��� ���������
static void DrawTypeMeasure();
/// ���������� ����� ���������
static void DrawModeMeasure();
/// ���������� ���������
static void DrawHint();
/// ���������� ������-���
static void DrawStatusBar();
static void DrawStatusBarA();
static void DrawStatusBarB();
static void DrawStatusBarC();
static void DrawStatusBarD();
/// ���������� ������ �������� �������� ������
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

void Display::StartScreen()
{   
    BeginScene(Color::BLACK);
    Text("OAO �����, 43-96/2, Cherm V 1.2").Write(40, 100);
    EndScene();
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
        Text("---����� ����������---").Write(40, 20);
        Text("������� ���. ��� ����������").Write(5, 50);
        Text("������� ����� ������� ��� ������").Write(5, 90);
        SU::Int2String(PLIS::CalibNumber(), buffer);
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

//        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        //delay_us(8);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawChannelSettings()
{
    Text(Menu::ChannelSettings()).Write(102, 17);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawTypeMeasure()
{
    int x = 0;
    int width = 100; 
    if (CURRENT_CHANNEL_IS_A)
    {
        Text(PageModes::typeMeasure.ToText()).Write(x, 25, width);
    }
    else if (CURRENT_CHANNEL_IS_B)
    {
        Text(PageModesB::typeMeasureB.ToText()).Write(x, 25, width);
    }
    else if (CURRENT_CHANNEL_IS_C)
    {
        Text(PageModesC::typeMeasureC.ToText()).Write(x, 25, width);
    }
    else if (CURRENT_CHANNEL_IS_D)
    {
        Text("�������").Write(x, 25, width);
    }
    Rectangle(width, 30).Draw(x, 15, Color::WHITE);
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
        Text(modes[PageModes::typeMeasure.value]->ToText()).Write(0, 55);
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
        Text(modesB[PageModesB::typeMeasureB.value]->ToText()).Write(0, 55);
    }
    else if (CURRENT_CHANNEL_IS_C)
    {
        static const Enumeration *modesC[2] =
        {
            &PageModesC::modeMeasureFrequencyC,
            &PageModesC::modeMeasureCountPulseC
        };
        Text(modesC[PageModesC::typeMeasureC.value]->ToText()).Write(0, 55);
    }   
    if (CURRENT_CHANNEL_IS_D)
    {  
        Text("�������").Write(0, 55);
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
        Text(PLIS::GiveAuto()).Write(102, 37);
        FreqMeter::UnloadAuto();
    }
    else
    {
        if(PLIS::AutoMode())
        {
            
            if((PLIS::MidAuto() != 0) || (PLIS::MaxAuto() != 0) || (PLIS::MinAuto() != 0))
            {
                Text(PLIS::GiveAuto()).Write(102, 37);
                PLIS::SwitchAuto();
                autoHint = HAL_GetTick();
                autoFlag = true;
            }
            else
            {
                Text("��������� ������ �����.").Write(102, 37);
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
        if(CURRENT_CHANNEL_IS_A && ((PageModes::modeMeasureFrequency == ModeMeasureFrequency::Tachometer) && (PageModes::typeMeasure == TypeMeasure::Frequency)))
        {
        }
        else if(CURRENT_CHANNEL_IS_B && ((PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::Tachometer) && (PageModesB::typeMeasureB == TypeMeasureB::Frequency)))
        {
        }
        else if(CURRENT_CHANNEL_IS_A && (PageModes::typeMeasure == TypeMeasure::CountPulse) && (PageModes::modeMeasureCountPulse == ModeMeasureCountPulse::StartStop))
        {
        }
        else if(CURRENT_CHANNEL_IS_B && (PageModesB::typeMeasureB == TypeMeasureB::CountPulse) && (PageModesB::modeMeasureCountPulseB == ModeMeasureCountPulseB::StartStop))
        {
        }
//        else if(CURRENT_CHANNEL_IS_C && (PageModesC::typeMeasureC == TypeMeasureC::CountPulse) && (PageModesC::modeMeasureCountPulseC == ModeMeasureCountPulseC::StartStop))
//        {
//        }
        else
        {
            Text("M").Write(430, 100);
        } 
    }
//    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) != 0)
//    {
//        if(xMark == false)
//        {
//            xMark = true;
//        }
//        else
//        {
//            xMark = false;
//        }
//    }
//    if(xMark == true)
//       {
//           Text("X").Write(430, 180);
//       }
    if(FreqMeter::TestModeStatus() == true)
    {
        Text("����").Write(430, 120);
    }
    if(PageIndication::refGenerator == RefGenerator::External)
    {
        Text("���� �").Write(420, 160);
    }
    if(PageModes::typeMeasure == TypeMeasure::Period || (PageModes::typeMeasure == TypeMeasure::Duration && PageModes::modeMeasureDuration != ModeMeasureDuration::Ndt_1ns) 
      || PageModesB::typeMeasureB == TypeMeasureB::Period || (PageModesB::typeMeasureB == TypeMeasureB::Duration && PageModes::modeMeasureDuration != ModeMeasureDuration::Ndt_1ns))
    {
        Text("�����").Write(430, 140);
        Text(PageModes::periodTimeLabels.ToText()).Write(430, 160);
        
    }
    if((CURRENT_CHANNEL_IS_A && (PageModes::typeMeasure == TypeMeasure::CountPulse) && PageModes::modeMeasureCountPulse == ModeMeasureCountPulse::StartStop) ||
       (CURRENT_CHANNEL_IS_B && (PageModesB::typeMeasureB == TypeMeasureB::CountPulse) && PageModesB::modeMeasureCountPulseB == ModeMeasureCountPulseB::StartStop))
    {
        if(PageModes::StartStop() == true)
        {
            Text("Start").Write(430, 60);
        }
        else
        {
            Text("Stop").Write(430, 60);
        }
    }
    if(PageIndication::launchSource == LaunchSource::OneTime)
    {
        if(PageIndication::OnceLaunch() == true)
        {
            if(second == 0)
            {
                second = HAL_GetTick();
            }
            Text("����").Write(430, 80);
            if((second + 1000) < HAL_GetTick())
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
//    Text(PLIS::GiveData()).Write(120, 120);
    FontBig::BigStringProp_print(PLIS::GiveData(), 10, 150, Color::WHITE);
    FontMid::MidStringProp_print(PLIS::GiveSpec(), 344, 170, Color::WHITE);
//    Text(PLIS::GiveSpec()).Write(344, 170);
//    Text(PLIS::GiveIdent()).Write(344, 80);
    if(((PageModes::modeMeasureFrequency == ModeMeasureFrequency::Tachometer && CURRENT_CHANNEL_IS_A) || 
            (PageModesB::modeMeasureFrequencyB == ModeMeasureFrequency::Tachometer && CURRENT_CHANNEL_IS_B)))
    {
        Text("60s").Write(2, 87, 60, Color::WHITE);
    }
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
            &PageModes::numberPeriods,      /// AB
            &PageModes::timeMeasure,        /// AC
            &PageModes::numberPeriods,      /// T_1
            nullptr,                        /// Tachometer
            nullptr                         /// Comparator
        },
        {   /// ModeMeasurePeriod::
            &PageModes::numberPeriods,      /// Period
            &PageModes::timeMeasure         /// F_1
        },
        {   /// ModeMeasureDuration::
            nullptr,                        /// Ndt
            nullptr,      /// Ndt_1
            nullptr,                        /// Ndt_1ns
            nullptr,                        /// Interval
            nullptr,                        /// S_1
            nullptr                         /// Phase
        },
        {   /// ModeMeasureCountPulse::
            nullptr,                        /// ATC
            &PageModes::numberPeriods,      /// ATC_1
            nullptr
        }
    };

    const Enumeration *mode = modes[PageModes::typeMeasure];

    const Enumeration *toText = enums[PageModes::typeMeasure][mode->value];
    
    int y = 80;
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
            &PageModes::numberPeriods,      /// BA
            &PageModes::timeMeasure,        /// BC
            &PageModes::numberPeriods,      /// T_1
            nullptr                         /// Tachometer
        },
        {   /// ModeMeasurePeriod::
            &PageModes::numberPeriods,      /// Period
            &PageModes::timeMeasure         /// F_1
        },
        {   /// ModeMeasureDuration::
            nullptr,                          /// Ndt
            nullptr,      /// Ndt_1
            nullptr,                          /// Ndt_1ns
            nullptr,                          /// Interval
            nullptr,                          /// S_1
            nullptr                           /// Phase
        },
        {   /// ModeMeasureCountPulse::
            nullptr,                          /// ATC
            &PageModes::numberPeriods,       /// ATC_1
            nullptr
        }
    };

    const Enumeration *mode = modesB[PageModesB::typeMeasureB];

    const Enumeration *toText = enumsB[PageModesB::typeMeasureB][mode->value];

    int y = 80;
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

    static const Enumeration * const enumsC[TypeMeasureC::Count][5] =
    {
        {   /// ModeMeasureFrequency::
            &PageModesC::timeMeasureC,        /// Freq
            &PageModesC::numberPeriodsC,      /// CA
            &PageModesC::numberPeriodsC,      /// CB
        },
        {   /// ModeMeasureCountPulse::
            nullptr,                          /// Manual
            nullptr,                          /// ATC
            &PageModesC::numberPeriodsC,  
            &PageModesC::numberPeriodsC       /// ATC_1
        }
    };

    const Enumeration *mode = modesC[PageModesC::typeMeasureC];

    const Enumeration *toText = enumsC[PageModesC::typeMeasureC][mode->value];

    int y = 80;
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
    int y = 80;
    int x = 0;
    int width = 60;
    Text(PageModesD::timeMeasureD.ToText()).Write(x + 2, y + 7, width, Color::WHITE);
    Rectangle(width, 30).Draw(x, y, Color::WHITE);
}
