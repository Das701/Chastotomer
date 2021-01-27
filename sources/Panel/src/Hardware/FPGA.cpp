#include "defines.h"
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
#include "Menu/Pages/Channels/Channels.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstring>
#include <cstdio>
#include <stm32f4xx_hal.h>


#define ReadPin(x)  HAL_GPIO_ReadPin(x)
#define SetPin(x)   HAL_GPIO_WritePin(x, GPIO_PIN_SET)
#define ResetPin(x) HAL_GPIO_WritePin(x, GPIO_PIN_RESET)

#define PinRD       GPIOC, GPIO_PIN_8                   // Флаг готовности чтения
#define PinWR       GPIOC, GPIO_PIN_9                   // Флга готовности к записи
#define PinCS       GPIOB, GPIO_PIN_12
#define PinDATA     GPIOB, GPIO_PIN_15
#define PinCLOCK    GPIOB, GPIO_PIN_13

#define Set_CS      SetPin(PinCS);
#define Reset_CS    ResetPin(PinCS);

#define Set_CLOCK   SetPin(PinCLOCK);   DELAY
#define Reset_CLOCK ResetPin(PinCLOCK); DELAY

#define Set_DATA    SetPin(PinDATA);    DELAY
#define Reset_DATA  ResetPin(PinDATA);  DELAY

#define Flag_RD     ReadPin(PinRD)

#define Read_WR     ReadPin(PinWR)

#define DELAY  HAL_TIM::DelayUS(2)


static uint ident = 0;      // Это значение считывается непосредственно из FPGA
static char encData[10];
static bool autoMode = false;
static bool isOverloaded = false;

uint FPGA::GovernorData::kCalib = 0;     // Это значение считывается непосредственно из FPGA
int FPGA::GovernorData::NAC = 0;         // Поправка для калибровочного коэффициента


MathFPGA::Comparator::Stack MathFPGA::Comparator::values(400);


void FPGA::CycleRead(int numBits, uint &value, bool verifyOnOverload)
{
    value = 0;

    for (int i = numBits - 1; i >= 0; i--)
    {
        Set_CLOCK; //-V2571
        value |= (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) << i); //-V2571
        Reset_CLOCK; //-V2571
    }

    if(verifyOnOverload)
    {
        isOverloaded = (value & 1U) != 0;
    };
}


void FPGA::CycleWrite(uint value, int numBits)
{
    for (int i = 0; i < numBits; i++)
    {
        WriteBit(((value) >> i) & 0x1);
    }
}


void FPGA::WriteBit(uint bit)
{
    HAL_GPIO_WritePin(PinDATA, (bit == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET); //-V2571
    DELAY;
    Set_CLOCK; //-V2571
    Reset_CLOCK; //-V2571
}


void FPGA::Init()
{
    __HAL_RCC_GPIOB_CLK_ENABLE(); //-V2571
    __HAL_RCC_GPIOC_CLK_ENABLE(); //-V2571
    
    
    GPIO_InitTypeDef is =
    {
        GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP
    };
    HAL_GPIO_Init(GPIOB, &is); //-V2571

    is.Pin = GPIO_PIN_14;
    is.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOB, &is); //-V2571

    is.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    HAL_GPIO_Init(GPIOC, &is); //-V2571

    Reset_CS; //-V2571
}


void FPGA::Update() //-V2008
{
    if(autoMode)
    {
        ReadAutoMode();
    }
    else
    {
        if (MathFPGA::Validator::VerySmallTime())
        {
            return;
        }

        if (Channel::Current()->mod.typeMeasure.IsDuration() && ModeDuration::Current().IsNdt_1ns())
        {
            ReadInterpolator();
        }
        else if(Channel::Current()->mod.typeMeasure.IsDuration() && (ModeDuration::Current().IsFillFactor() || ModeDuration::Current().IsPhase()))
        {
            ReadFillFactorPhase();
        }
        else if (CURRENT_CHANNEL_IS_A && (Channel::A->mod.modeFrequency.IsComparator() && Channel::A->mod.typeMeasure.IsFrequency())) 
        {
            ReadComparator();
        }
        else
        {
            if (Flag_RD != 0) //-V2571
            {
                uint counterA = 0;
                uint counterB = 0;

                Set_CS; //-V2571

                CycleRead(32, counterA, true);
              
                if((ModeFrequency::Current().IsRatioAC() || ModeFrequency::Current().IsRatioBC()) && Relation::IsEnabled())
                {
                    CycleRead(32, counterB, true);
                }

                Reset_CS; //-V2571

                MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::MainCounters, counterA, counterB);

                HAL_TIM::DelayUS(8);
            }
        }
    }
} 


void FPGA::ReadFillFactorPhase()
{
    if (Flag_RD != 0) //-V2571
    {
        uint period = 0;
        uint duration = 0;

        Set_CS; //-V2571
        CycleRead(32, period, true);
        CycleRead(32, duration, true);
        Reset_CS; //-V2571

//        LOG_WRITE("%d %d", period, duration);

        MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::FillFactorPhase, period, duration);

        HAL_TIM::DelayUS(8);
    }
}


void FPGA::ReadInterpolator()
{
    if (Flag_RD != 0) //-V2571
    {
        uint timer = 0;
        uint cal1 = 0;
        uint cal2 = 0;

        Set_CS; //-V2571
        CycleRead(3, ident, false);
        CycleRead(24, timer, false);
        CycleRead(24, cal1, false);
        CycleRead(24, cal2, false);
        Reset_CS; //-V2571

        MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::Interpolator, timer, cal1, cal2);

        HAL_TIM::DelayUS(8);
    }
}


void FPGA::ReadAutoMode()
{
    if (Flag_RD != 0) //-V2571
    {
        Set_CS; //-V2571
        CycleRead(3, ident, false);
        CycleRead(10, MathFPGA::Auto::fpgaMin, false);
        CycleRead(10, MathFPGA::Auto::fpgaMid, false);
        CycleRead(10, MathFPGA::Auto::fpgaMax, false);
        Reset_CS; //-V2571

        Display::Refresh();

        HAL_TIM::DelayUS(8);
    }
}


void FPGA::ReadComparator()
{
    if (Flag_RD != 0) //-V2571
    {
        uint counter = 0;
        uint interpol1 = 0;
        uint cal1 = 0;
        uint interpol2 = 0;
        uint cal2 = 0;

        Set_CS; //-V2571
        CycleRead(3, ident, false);
        CycleRead(32, counter, false);
        CycleRead(16, interpol1, false);
        CycleRead(16, cal1, false);
        CycleRead(16, interpol2, false);
        CycleRead(16, cal2, false);
        Reset_CS; //-V2571

        MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::Comparator, counter, interpol1, cal1, interpol2, cal2);

        HAL_TIM::DelayUS(8);
    }
}


void FPGA::GovernorData::IncreaseN()
{
    if(PageIndication::calibrationMode.IsEnabled())
    {
        NAC++;
    }
    else
    {
        if(CURRENT_CHANNEL_IS_A)
        {
            MathFPGA::NA++;
        }
        else if(CURRENT_CHANNEL_IS_B) //-V2516
        {
            MathFPGA::NB++;
        }
    }
}

void FPGA::GovernorData::DecreaseN()
{       
    if(PageIndication::calibrationMode.IsEnabled())
    {
        NAC--;
    }
    else
    {
        if(CURRENT_CHANNEL_IS_A)
        {
            MathFPGA::NA--;
        }
        else if(CURRENT_CHANNEL_IS_B) //-V2516
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


void FPGA::ReadValueCalibrator()
{
    uint calib = 0;

    while (Flag_RD == 0) //-V2571
    {
    }

    Set_CS; //-V2571

    CycleRead(3, ident, false);

    CycleRead(10, calib, false);

    Reset_CS; //-V2571

    HAL_TIM::DelayUS(8);

    GovernorData::SetValueCalibrator(calib);
}


void FPGA::GovernorData::SetValueCalibrator(uint value)
{
    kCalib = value;
    NAC = 0;
}


void FPGA::GovernorData::Write()
{
    Calculate();

    if (Read_WR != 0)           // \todo К сожалению, флаг готовности не работает так, как надо и если ожидать его установки в ноль, то происходит сбой передачи данных //-V2571
    {                           // Если флаг не готов, выходим. Передавать нужно только если флаг уже установлен в 0
        return;
    }
    
    Reset_CLOCK; //-V2571
    Set_DATA; //-V2571
    Set_CLOCK; //-V2571
    Reset_CLOCK; //-V2571

    for (int i = 9; i > -1; i--)
    {
        WriteBit((uint)encData[i]);
    }

    Reset_DATA; //-V525 //-V2571
    Set_CLOCK; //-V2571
    Set_DATA; //-V2571

    Reset_CLOCK; //-V2571
    Reset_DATA; //-V2571

//    while (Read_READY == 0)
//    {
//    }
}


void FPGA::WriteCommand(const Command &command)
{
    while (Read_WR != 0)             // \todo Провеерить. Возможно, по аналогии с передачей данных нельзя ожидать флага готовности //-V2571
    {
    }

    Reset_CLOCK; //-V525 //-V2571
    Reset_DATA; //-V2571
    Set_CLOCK; //-V2571
    Reset_CLOCK; //-V2571

    CycleWrite(command.value, 10); //-V2571

    Reset_DATA; //-V525 //-V2571
    Set_CLOCK; //-V2571
    Set_DATA; //-V2571

    Reset_CLOCK; //-V2571
    Reset_DATA; //-V2571
}

void FPGA::GovernorData::Reset()
{
    NAC = 0;
}


int FPGA::GovernorData::ValueCalibrator()
{
    int value = (int)kCalib + NAC;

    LIMITATION_BELOW(value, 0);

    return value;
}


void FPGA::GovernorData::Calculate()
{
    if (PageIndication::calibrationMode.IsEnabled())
    {
        MathFPGA::DecToBin(ValueCalibrator(), encData);
    }
    else
    {
        int negative = 1024;

        if (CURRENT_CHANNEL_IS_A)
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
        else if (CURRENT_CHANNEL_IS_B) //-V2516
        {
            if (MathFPGA::NB < 0)
            {
                MathFPGA::DecToBin(negative + MathFPGA::NB, encData);
            }
            else
            {
                MathFPGA::DecToBin(MathFPGA::NB, encData);
            }
        }
    }
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
