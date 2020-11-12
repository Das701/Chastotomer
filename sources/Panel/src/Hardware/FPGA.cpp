#include "defines.h"
#include "Settings.h"
#include "Display/Display.h"
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
#define PinCS       GPIOB, GPIO_PIN_12

#define Set_CS      SetPin(PinCS)
#define Reset_CS    ResetPin(PinCS)

#define Read_FLAG   ReadPin(PinFLAG)


#define READ_PIN_B14(x, bit)                                                            \
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);                                \
    HAL_TIM::DelayUS(2);                                                                \
    x |= (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) << bit);                                 \
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);                              \
    HAL_TIM::DelayUS(2);

#define  CYCLE_READ_PIN_B14(num, x, verifyOverload)                                     \
    x = 0;                                                                              \
    for (int i = num - 1; i >= 0; i--)                                                  \
    {                                                                                   \
        READ_PIN_B14(x, i);                                                             \
    }                                                                                   \
    if(verifyOverload) { isOverloaded = (x & 1U) != 0; };                               \
    Display::Refresh();


#define WRITE_COMMAND(x)                                                                \
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, (x == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);    \
    HAL_TIM::DelayUS(2);                                                                \
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);                                \
    HAL_TIM::DelayUS(2);                                                                \
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);

#define CYCLE_WRITE_COMMAND(num, x)                                                     \
    for (int i = 0; i < num; i++)                                                       \
    {                                                                                   \
        WRITE_COMMAND(x[i]);                                                            \
    }

uint FPGA::fpgaTimer = 0;
uint FPGA::fpgaIdent = 0;
uint FPGA::fpgaCAL1 = 0;
uint FPGA::fpgaCAL2 = 0;

static char encData[10];
static bool autoMode = false;
static uint fpgaCalib = 0;
static int NAC = 0;

static bool isOverloaded = false;


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
}


void FPGA::Update()
{
    if(autoMode)
    {
        if(Read_FLAG != 0)
        {
            Set_CS;

            CYCLE_READ_PIN_B14(3, fpgaIdent, false);

            CYCLE_READ_PIN_B14(10, MathFPGA::Auto::fpgaMin, false);

            CYCLE_READ_PIN_B14(10, MathFPGA::Auto::fpgaMid, false);

            CYCLE_READ_PIN_B14(10, MathFPGA::Auto::fpgaMax, false);

            Reset_CS;

            HAL_TIM::DelayUS(8);
        }
    }
    else
    {
        if (ModeMeasureDuration::Current().Is_Ndt_1ns() && MathFPGA::Interpolation::IsEnabled())
        {
            if (Read_FLAG != 0)
            {
                Set_CS;

                CYCLE_READ_PIN_B14(3, fpgaIdent, false);

                CYCLE_READ_PIN_B14(24, fpgaTimer, false);

                CYCLE_READ_PIN_B14(24, fpgaCAL1, false);

                CYCLE_READ_PIN_B14(24, fpgaCAL2, false);

                Reset_CS;

                HAL_TIM::DelayUS(8);
            }
        }
        else if((ModeMeasureDuration::Current().Is_DutyCycle() || ModeMeasureDuration::Current().Is_Phase()) && MathFPGA::DutyCycle::IsEnabled())
        {
            if (Read_FLAG != 0)
            {
                Set_CS;

                CYCLE_READ_PIN_B14(32, MathFPGA::DutyCycle::fpgaPeriod, true);

                CYCLE_READ_PIN_B14(32, MathFPGA::DutyCycle::fpgaDuration, true);

                Reset_CS;

                HAL_TIM::DelayUS(8);
            }
        }
        else if (CURRENT_CHANNEL_IS_A && (PageModesA::modeMeasureFrequency.IsComparator() && PageModesA::typeMeasure.IsFrequency())) 
        {
            if (Read_FLAG != 0)
            {
                uint decFx = 0;

                uint fpgaTizm = 0;

                uint fpgaNkal = 0;

                Set_CS;

                CYCLE_READ_PIN_B14(3, fpgaIdent, false);

                CYCLE_READ_PIN_B14(32, decFx, false);

                CYCLE_READ_PIN_B14(16, fpgaTizm, false);

                CYCLE_READ_PIN_B14(16, fpgaNkal, false);

                Reset_CS;

                HAL_TIM::DelayUS(8);

                int decNkal = (int)fpgaNkal;

                if (decNkal != 0)
                {
                    int decTizm = (int)fpgaTizm;

                    if ((fpgaTizm & (1U << 15)) != 0)
                    {
                        decTizm -= 65536;
                    }

                    ValuePICO dx(decTizm);
                    dx.Div((uint)decNkal);
                    dx.Div(2 * 5000000);

                    ValuePICO k(5000000);
                    k.Sub(ValuePICO((int)decFx));
                    k.Div(5000000);
                    k.Sub(dx);
                    k.Mul(1000000);

                    k.SetSign(1);

                    MathFPGA::Measure::valueComparator = k;

                    PageStatistics::AppendValue(k.ToDouble());
                }
            }
        }
        else
        {
            if (Read_FLAG != 0)
            {
                Set_CS;

                CYCLE_READ_PIN_B14(32, MathFPGA::Measure::fpgaFrequencyA, true);
              
                if((ModeMeasureFrequency::Current().IsRatioAC() || ModeMeasureFrequency::Current().IsRatioBC()) &&
                    PageModesA::RelationCheck())
                {
                    CYCLE_READ_PIN_B14(32, MathFPGA::Measure::fpgaFrequencyB, true);
                }

                Reset_CS;

                HAL_TIM::DelayUS(8);
            }
        }
    }
}


void FPGA::WriteCommand(const char command[4], const char argument[6])
{
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == 0)
    {
        Reset_CS;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
        HAL_TIM::DelayUS(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET); //-V525
        HAL_TIM::DelayUS(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
        HAL_TIM::DelayUS(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);

        CYCLE_WRITE_COMMAND(4, command);

        CYCLE_WRITE_COMMAND(6, argument);

        HAL_TIM::DelayUS(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); //-V525
        HAL_TIM::DelayUS(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        HAL_TIM::DelayUS(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
        HAL_TIM::DelayUS(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
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

    Set_CS;

    CYCLE_READ_PIN_B14(3, fpgaIdent, false);

    CYCLE_READ_PIN_B14(10, fpgaCalib, false);

    Reset_CS;

    HAL_TIM::DelayUS(8);
}


void FPGA::WriteData()
{
    int negative = 1024;

    if(PageIndication::calibration.Is(Calibration::Pressed))
    {
        if((int)fpgaCalib + NAC < 0)
        {
            fpgaCalib = 0;
            NAC = 0;
        }

        fpgaCalib = (uint)((int)fpgaCalib + NAC);
        MathFPGA::DecToBin((int)fpgaCalib, encData);
        NAC = 0;
    }
    else
    {
        if(CURRENT_CHANNEL_IS_A)
        {
                if(MathFPGA::NA < 0)
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

    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == 0)
    {
        Reset_CS;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
        HAL_TIM::DelayUS(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); //-V525
        HAL_TIM::DelayUS(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
        HAL_TIM::DelayUS(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);

        for(int i = 9; i > -1; i--)
        {
            WRITE_COMMAND(encData[i]);
        }

        HAL_TIM::DelayUS(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); //-V525
        HAL_TIM::DelayUS(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        HAL_TIM::DelayUS(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
        HAL_TIM::DelayUS(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
    }
}


int FPGA::CalibNumber()
{
    return (int)fpgaCalib;
}


bool FPGA::IsOverloaded()
{
    return isOverloaded;
}
