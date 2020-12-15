#include "defines.h"
#include "Log.h"
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
#include "Utils/StringUtils.h"
#include <cstring>
#include <cstdio>
#include <stm32f4xx_hal.h>


#define ReadPin(x)  HAL_GPIO_ReadPin(x)
#define SetPin(x)   HAL_GPIO_WritePin(x, GPIO_PIN_SET)
#define ResetPin(x) HAL_GPIO_WritePin(x, GPIO_PIN_RESET)

#define PinRD       GPIOC, GPIO_PIN_8                   // ‘лаг готовности чтени€
#define PinWR       GPIOC, GPIO_PIN_9                   // ‘лга готовности к записи
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


#define READ_PIN_B14(x, bit)                                                            \
    Set_CLOCK;                                                                          \
    x |= (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) << bit);                                 \
    Reset_CLOCK;

#define  CYCLE_READ_PIN_B14(num, x, verifyOverload)                                     \
    x = 0;                                                                              \
    for (int i = num - 1; i >= 0; i--) { READ_PIN_B14(x, i);}                           \
    if(verifyOverload) { isOverloaded = (x & 1U) != 0; };                               \

#define WRITE_BIT(x)                                                                    \
    HAL_GPIO_WritePin(PinDATA, ((x) == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);             \
    DELAY;                                                                              \
    Set_CLOCK;                                                                          \
    Reset_CLOCK;

#define WRITE(value, num)   for (int i = 0; i < num; i++) { WRITE_BIT(((value) >> i) & 0x1)}


static uint ident = 0;      // Ёто значение считываетс€ непосредственно из FPGA
static uint kCalib = 0;     // Ёто значение считываетс€ непосредственно из FPGA

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

    Reset_CS;
}


void FPGA::Update() //-V2008
{
    if(autoMode)
    {
        ReadAutoMode();
    }
    else
    {
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
            if (Flag_RD != 0)
            {
                uint counterA = 0;
                uint counterB = 0;

                Set_CS;

                CYCLE_READ_PIN_B14(32, counterA, true);
              
                if((ModeFrequency::Current().IsRatioAC() || ModeFrequency::Current().IsRatioBC()) && Relation::IsEnabled())
                {
                    CYCLE_READ_PIN_B14(32, counterB, true);
                }

                Reset_CS;

                LOG_WRITE("%d %d", counterA, counterB);

                MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::MainCounters, counterA, counterB);

                HAL_TIM::DelayUS(8);
            }
        }
    }
} 


void FPGA::ReadFillFactorPhase()
{
    if (Flag_RD != 0)
    {
        uint period = 0;
        uint duration = 0;

        Set_CS;
        CYCLE_READ_PIN_B14(32, period, true);
        CYCLE_READ_PIN_B14(32, duration, true);
        Reset_CS;

//        LOG_WRITE("%d %d", period, duration);

        MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::FillFactorPhase, period, duration);

        HAL_TIM::DelayUS(8);
    }
}


void FPGA::ReadInterpolator()
{
    if (Flag_RD != 0)
    {
        uint timer = 0;
        uint cal1 = 0;
        uint cal2 = 0;

        Set_CS;
        CYCLE_READ_PIN_B14(3, ident, false); //-V525
        CYCLE_READ_PIN_B14(24, timer, false);
        CYCLE_READ_PIN_B14(24, cal1, false);
        CYCLE_READ_PIN_B14(24, cal2, false);
        Reset_CS;

        MathFPGA::Measure::SetNewData(MathFPGA::Measure::TypeData::Interpolator, timer, cal1, cal2);

        HAL_TIM::DelayUS(8);
    }
}


void FPGA::ReadAutoMode()
{
    if (Flag_RD != 0)
    {
        Set_CS;
        CYCLE_READ_PIN_B14(3, ident, false);
        CYCLE_READ_PIN_B14(10, MathFPGA::Auto::fpgaMin, false);
        CYCLE_READ_PIN_B14(10, MathFPGA::Auto::fpgaMid, false);
        CYCLE_READ_PIN_B14(10, MathFPGA::Auto::fpgaMax, false);
        Reset_CS;

        Display::Refresh();

        HAL_TIM::DelayUS(8);
    }
}


void FPGA::ReadComparator()
{
    if (Flag_RD != 0)
    {
        uint fx = 0;
        uint tizm = 0;
        uint nkal = 0;

        Set_CS;
        CYCLE_READ_PIN_B14(3, ident, false);
        CYCLE_READ_PIN_B14(32, fx, false);
        CYCLE_READ_PIN_B14(16, tizm, false);
        CYCLE_READ_PIN_B14(16, nkal, false);
        Reset_CS;

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
    while (Flag_RD == 0)
    {
    }

    Set_CS;

    CYCLE_READ_PIN_B14(3, ident, false);

    CYCLE_READ_PIN_B14(10, kCalib, false);

    Reset_CS;

    HAL_TIM::DelayUS(8);
}


void FPGA::WriteDataGovernor()
{
    CalculateData();

    if (Read_WR != 0)           // \todo   сожалению, флаг готовности не работает так, как надо и если ожидать его установки в ноль, то происходит сбой передачи данных
    {                           // ≈сли флаг не готов, выходим. ѕередавать нужно только если флаг уже установлен в 0
        return;
    }
    
    Reset_CLOCK;
    Set_DATA;
    Set_CLOCK;
    Reset_CLOCK;

    for (int i = 9; i > -1; i--)
    {
        WRITE_BIT(encData[i]);
    }

    Reset_DATA; //-V525
    Set_CLOCK;
    Set_DATA;

    Reset_CLOCK;
    Reset_DATA;

//    while (Read_READY == 0)
//    {
//    }
}


void FPGA::WriteCommand(const Command &command)
{
    while (Read_WR != 0)             // \todo ѕровеерить. ¬озможно, по аналогии с передачей данных нельз€ ожидать флага готовности
    {
    }

    Reset_CLOCK; //-V525
    Reset_DATA;
    Set_CLOCK;
    Reset_CLOCK;

    WRITE(command.value, 10);

    Reset_DATA; //-V525
    Set_CLOCK;
    Set_DATA;

    Reset_CLOCK;
    Reset_DATA;
}


void FPGA::CalculateData()
{
    int negative = 1024;

    if (PageIndication::calibration.Is(Calibration::Pressed))
    {
        if ((int)kCalib + NAC < 0)
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
        else if (CURRENT_CHANNEL_IS_B)
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
//            if (val / 10 > fromEnd) { return false; }     // »збавл€емс€ от слишком больших значений
//    
//            if (val * 10 < fromEnd) { return false; }     // ¬ыкидываем слишком маленькие значени€
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
