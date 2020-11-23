#include "defines.h"
//#include "Log.h"
#include "Settings.h"
#include "Display/Display.h"
#include "Display/Objects.h"
#include "Hardware/FPGA.h"
#include "Hardware/MathFPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Hint.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Menu/Pages/Settings/PagesSettings.h"
#include "Utils/StringUtils.h"
#include <cstring>
#include <cstdio>
#include <stm32f4xx_hal.h>


#define ReadPin(x)  HAL_GPIO_ReadPin(x)
#define SetPin(x)   HAL_GPIO_WritePin(x, GPIO_PIN_SET)
#define ResetPin(x) HAL_GPIO_WritePin(x, GPIO_PIN_RESET)

#define PinFLAG     GPIOC, GPIO_PIN_8
#define PinWR       GPIOB, GPIO_PIN_12
#define PinDATA     GPIOB, GPIO_PIN_15
#define PinCLOCK    GPIOB, GPIO_PIN_13

#define Set_WR      SetPin(PinWR);
#define Reset_WR    ResetPin(PinWR);

#define Set_CLOCK   SetPin(PinCLOCK);   DELAY
#define Reset_CLOCK ResetPin(PinCLOCK); DELAY

#define Set_DATA    SetPin(PinDATA);    DELAY
#define Reset_DATA  ResetPin(PinDATA);  DELAY

#define Read_FLAG   ReadPin(PinFLAG)

#define DELAY  HAL_TIM::DelayUS(10)


#define READ_PIN_B14(x, bit)                                                            \
    Set_CLOCK;                                                                          \
    x |= (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) << bit);                                 \
    Reset_CLOCK;

#define  CYCLE_READ_PIN_B14(num, x, verifyOverload)                                     \
    x = 0;                                                                              \
    for (int i = num - 1; i >= 0; i--) { READ_PIN_B14(x, i);}                           \
    if(verifyOverload) { isOverloaded = (x & 1U) != 0; };                               \

#define  CYCLE_READ_PIN_B14_NO_REFRESH(num, x, verifyOverload)                          \
    x = 0;                                                                              \
    for (int i = num - 1; i >= 0; i--) { READ_PIN_B14(x, i); }                          \
    if(verifyOverload) { isOverloaded = (x & 1U) != 0; };


#define WRITE_BIT(x)                                                                    \
    HAL_GPIO_WritePin(PinDATA, (x == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);               \
    DELAY;                                                                              \
    Set_CLOCK;                                                                          \
    Reset_CLOCK;

#define WRITE(num, x) for (int i = 0; i < num; i++) { WRITE_BIT(x[i]); }

static uint ident = 0;      // Это значение считывается непосредственно из FPGA
static uint kCalib = 0;     // Это значение считывается непосредственно из FPGA

static char encData[10];
static bool autoMode = false;
static int NAC = 0;

static bool isOverloaded = false;


MathFPGA::Comparator::Stack MathFPGA::Comparator::values(400);


void FPGA::Init()
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    
    GPIO_InitTypeDef is =
    {
        GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP
    };
    HAL_GPIO_Init(GPIOB, &is);

    is.Pin = GPIO_PIN_14;
    is.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOB, &is);

    is.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    HAL_GPIO_Init(GPIOC, &is);

    Reset_WR;

//    TimeMeasure::LoadToFPGA();
//    NumberPeriods::LoadToFPGA();
//    PeriodTimeLabels::LoadToFPGA();
//
//    PageModes::OnChanged_TypeMeasure();
//
//    Channel::LoadCurrentToFPGA();
}


void FPGA::Update() //-V2008
{
    if(autoMode)
    {
        ReadAutoMode();
    }
    else
    {
        if (TypeMeasure::Current().IsDuration() && ModeMeasureDuration::Current().IsNdt_1ns())
        {
            ReadInterpolator();
        }
        else if(TypeMeasure::Current().IsDuration() && (ModeMeasureDuration::Current().IsFillFactor() || ModeMeasureDuration::Current().IsPhase()))
        {
            ReadFillFactorPhase();
        }
        else if (CURRENT_CHANNEL_IS_A && (PageModesA::modeMeasureFrequency.IsComparator() && PageModesA::typeMeasure.IsFrequency())) 
        {
            ReadComparator();
        }
        else
        {
            if (Read_FLAG != 0)
            {
                uint counterA = 0;
                uint counterB = 0;

                Set_WR;

                CYCLE_READ_PIN_B14(32, counterA, true);
              
                if((ModeMeasureFrequency::Current().IsRatioAC() || ModeMeasureFrequency::Current().IsRatioBC()) && PageModesA::RelationCheck())
                {
                    CYCLE_READ_PIN_B14(32, counterB, true);
                }

                Reset_WR;

//                LOG_WRITE("%d %d", counterA, counterB);

                MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::MainCounters, counterA, counterB);

                HAL_TIM::DelayUS(8);
            }
        }
    }
} 


void FPGA::ReadFillFactorPhase()
{
    if (Read_FLAG != 0)
    {
        uint period = 0;
        uint duration = 0;

        Set_WR;
        CYCLE_READ_PIN_B14(32, period, true);
        CYCLE_READ_PIN_B14(32, duration, true);
        Reset_WR;

        MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::FillFactorPhase, period, duration);

        HAL_TIM::DelayUS(8);
    }
}


void FPGA::ReadInterpolator()
{
    if (Read_FLAG != 0)
    {
        uint timer = 0;
        uint cal1 = 0;
        uint cal2 = 0;

        Set_WR;
        CYCLE_READ_PIN_B14(3, ident, false); //-V525
        CYCLE_READ_PIN_B14(24, timer, false);
        CYCLE_READ_PIN_B14(24, cal1, false);
        CYCLE_READ_PIN_B14(24, cal2, false);
        Reset_WR;

        MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::Interpolator, timer, cal1, cal2);

        HAL_TIM::DelayUS(8);
    }
}


void FPGA::ReadAutoMode()
{
    if (Read_FLAG != 0)
    {
        Set_WR;
        CYCLE_READ_PIN_B14(3, ident, false);
        CYCLE_READ_PIN_B14(10, MathFPGA::Auto::fpgaMin, false);
        CYCLE_READ_PIN_B14(10, MathFPGA::Auto::fpgaMid, false);
        CYCLE_READ_PIN_B14(10, MathFPGA::Auto::fpgaMax, false);
        Reset_WR;

        Display::Refresh();

        HAL_TIM::DelayUS(8);
    }
}


void FPGA::ReadComparator()
{
    if (Read_FLAG != 0)
    {
        uint fx = 0;
        uint tizm = 0;
        uint nkal = 0;

        Set_WR;
        CYCLE_READ_PIN_B14_NO_REFRESH(3, ident, false);
        CYCLE_READ_PIN_B14_NO_REFRESH(32, fx, false);
        CYCLE_READ_PIN_B14_NO_REFRESH(16, tizm, false);
        CYCLE_READ_PIN_B14_NO_REFRESH(16, nkal, false);
        Reset_WR;

        MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::Comparator, fx, tizm, nkal);

        HAL_TIM::DelayUS(8);
    }
}


void FPGA::IncreaseN()
{
    if(PageIndication::calibration.Is(Calibration::Pressed))
    {
        NAC++;
    }
    else
    {
        if(CURRENT_CHANNEL_IS_A)
        {
            MathFPGA::NA++;
        }
        else if(CURRENT_CHANNEL_IS_B)
        {
            MathFPGA::NB++;
        }
    }
}

void FPGA::DecreaseN()
{       
    if(PageIndication::calibration.Is(Calibration::Pressed))
    {
        NAC--;
    }
    else
    {
        if(CURRENT_CHANNEL_IS_A)
        {
            MathFPGA::NA--;
        }
        else if(CURRENT_CHANNEL_IS_B)
        {
            MathFPGA::NB--;
        }
    }
}


void FPGA::SwitchAuto()
{
    if(!autoMode)
    {
        autoMode = true;
    }
    else
    {
        autoMode = false;
    }
}


bool FPGA::AutoMode()
{
    return autoMode;
}


void FPGA::ReadCalibNumber()
{
    while (Read_FLAG == 0)
    {
    }

    Set_WR;

    CYCLE_READ_PIN_B14(3, ident, false);

    CYCLE_READ_PIN_B14(10, kCalib, false);

    Reset_WR;

    HAL_TIM::DelayUS(8);
}


void FPGA::WriteData()
{
    int negative = 1024;

    if(PageIndication::calibration.Is(Calibration::Pressed))
    {
        if((int)kCalib + NAC < 0)
        {
            kCalib = 0;
            NAC = 0;
        }

        kCalib = (uint)((int)kCalib + NAC);
        MathFPGA::DecToBin((int)kCalib, encData);
        NAC = 0;
    }
    else
    {
        if(CURRENT_CHANNEL_IS_A)
        {
            if (MathFPGA::NA < 0)
            {
                MathFPGA::DecToBin(negative + MathFPGA::NA, encData);
            }
            else
            {
                MathFPGA::DecToBin(MathFPGA::NA, encData);
            }
        }
        else if(CURRENT_CHANNEL_IS_B)
        {
            if(MathFPGA::NB < 0)
            {
                MathFPGA::DecToBin(negative + MathFPGA::NB, encData);
            }
            else
            {
                MathFPGA::DecToBin(MathFPGA::NB, encData);
            }
        }
    }

    while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) != 0)
    {
    }

    Reset_CLOCK;
    Set_DATA;
    Set_CLOCK;
    Reset_CLOCK;

    //LOG_WRITE("%s", MathFPGA::BinToString(encData, 10).c_str());

    for (int i = 9; i > -1; i--)
    {
        WRITE_BIT(encData[i]);
    }

    Reset_DATA;
    Set_CLOCK;
    Set_DATA;

    Reset_CLOCK;
    Reset_DATA;
}


void FPGA::WriteCommand(const char command[4], const char argument[6])
{
    //    LOG_WRITE("%s %s", MathFPGA::BinToString(command, 4).c_str(), MathFPGA::BinToString(argument, 6).c_str());

    while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) != 0)
    {
    }

    Reset_CLOCK;
    Reset_DATA;
    Set_CLOCK;
    Reset_CLOCK;

    WRITE(4, command);
    WRITE(6, argument);

    Reset_DATA;
    Set_CLOCK;
    Set_DATA;

    Reset_CLOCK;
    Reset_DATA;
}


int FPGA::CalibNumber()
{
    return (int)kCalib;
}


bool FPGA::IsOverloaded()
{
    return isOverloaded;
}


bool MathFPGA::Comparator::Stack::AppendValue(double val)
{
//    if (val > 1e5)
//    {
//        return false;
//    }
//
//    if (Size() < 5)
//    {
//        Push(val);
//        return true;
//    }
//
//    if(val != 0.0) //-V2550 //-V550
//    {
//        for (int i = 0; i < 5; i++)
//        {
//            double fromEnd = GetFromEnd(i);
//            
//            if(fromEnd == 0.0) { continue; }                //-V2550 //-V550
//    
//            if (val / 10 > fromEnd) { return false; }     // Избавляемся от слишком больших значений
//    
//            if (val * 10 < fromEnd) { return false; }     // Выкидываем слишком маленькие значения
//        }
//    }

    Push(val);

    return true;
}


double MathFPGA::Comparator::Stack::GetFromEnd(int fromEnd)
{
    if (fromEnd < 0 || fromEnd > (Size() - 1))
    {
        return -1.0;
    }

    return (*this)[Size() - 1 - fromEnd];
}


String FPGA::GiveIdent()
{
    return String("%ud", ident);
}
