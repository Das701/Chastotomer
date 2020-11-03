#include "defines.h"
#include "Settings.h"
#include "Hardware/FPGA.h"
#include "Hardware/MathFPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Hint.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include "Menu/Pages/PageModesA.h"
#include "Menu/Pages/PageModesB.h"
#include "Menu/Pages/PageModesC.h"
#include "Menu/Pages/PageModesD.h"
#include "Menu/Pages/PageIndication.h"
#include "Utils/StringUtils.h"
#include <cstring>
#include <cstdio>
#include <stm32f4xx_hal.h>


#define READ_PIN_B14(x)  \
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);    \
    HAL_TIM::DelayUS(2);                                    \
    x = (char)HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);         \
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);  \
    HAL_TIM::DelayUS(2);

#define WRITE_COMMAND_1()   \
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);    \
    HAL_TIM::DelayUS(2);                                    \
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);    \
    HAL_TIM::DelayUS(2);                                    \
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);

#define WRITE_COMMAND_0() \
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);  \
    HAL_TIM::DelayUS(2);                                    \
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);    \
    HAL_TIM::DelayUS(2);                                    \
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);


char FPGA::dataA[32] = { 0 };
char FPGA::dataB[32] = { 0 };
char FPGA::timer1[27] = { 0 };
char FPGA::ident[4] = { 0 };
char FPGA::minAuto[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
char FPGA::midAuto[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
char FPGA::maxAuto[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


static char encData[10];
static bool autoMode = false;
static char calibBin[10];
static int calibNumber = 0;
static int NAC = 0;


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
        if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) != 0)
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

            for (int i = 0; i < 3; i++)
            {
                READ_PIN_B14(ident[i]);
            }

            for (int i = 0; i < 10; i++)
            {
                READ_PIN_B14(minAuto[i]);
            }

            for (int i = 0; i < 10; i++)
            {
                READ_PIN_B14(midAuto[i]);
            }

            for (int i = 0; i < 10; i++)
            {
                READ_PIN_B14(maxAuto[i]);
            }

            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
            HAL_TIM::DelayUS(8);
        }
    }
    else
    {
        if (ModeMeasureDuration::Current().Is_Ndt_1ns() && PageModesA::InterpolateCheck())
        {
            if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) != 0)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

                for (int i = 0; i < 3; i++)
                {
                    READ_PIN_B14(ident[i]);
                }

                for (int i = 0; i < 24; i++)
                {
                    READ_PIN_B14(timer1[i]);
                }

                for (int i = 0; i < 24; i++)
                {
                    READ_PIN_B14(MathFPGA::CAL1[i]);
                }

                for (int i = 0; i < 24; i++)
                {
                    READ_PIN_B14(MathFPGA::CAL2[i]);
                }

                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
                HAL_TIM::DelayUS(8);
            }
        }
        else if((ModeMeasureDuration::Current().Is_Dcycle() || ModeMeasureDuration::Current().Is_Phase()) && PageModesA::DCycleCheck())
        {
            if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) != 0)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

                for (int i = 0; i < 32; i++)
                {
                    READ_PIN_B14(MathFPGA::period[i]);
                }

                for (int i = 0; i < 32; i++)
                {
                    READ_PIN_B14(MathFPGA::duration[i]);
                }

                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
                HAL_TIM::DelayUS(8);
            }
        }
        else if (CURRENT_CHANNEL_IS_A && (PageModesA::modeMeasureFrequency.IsComparator() && PageModesA::typeMeasure.IsFrequency())) 
        {
            if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) != 0)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

                for (int i = 0; i < 3; i++)
                {
                    READ_PIN_B14(ident[i]);
                }

                for (int i = 0; i < 32; i++)
                {
                    READ_PIN_B14(MathFPGA::binFx[i]);
                }

                for (int i = 0; i < 16; i++)
                {
                    READ_PIN_B14(MathFPGA::binTizm[i]);
                }

                for (int i = 0; i < 16; i++)
                {
                    READ_PIN_B14(MathFPGA::binNkal[i]);
                }

                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
                HAL_TIM::DelayUS(8);
            }
        }
        else
        {
            if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) != 0)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

                for (int i = 0; i < 32; i++)
                {
                    READ_PIN_B14(dataA[i]);
                }

                if(CurrentModeMeasureFrequency::Is_AC_or_BC() && PageModesA::RelationCheck())
                {
                    for (int i = 0; i < 32; i++)
                    {
                        READ_PIN_B14(dataB[i]);
                    }
                }

                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
                HAL_TIM::DelayUS(8);
            }
        }
    }
}


void FPGA::WriteCommand(const char *command, const char *argument)
{
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == 0)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
        HAL_TIM::DelayUS(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET); //-V525
        HAL_TIM::DelayUS(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
        HAL_TIM::DelayUS(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);

        for (int i = 0; i < 4; i++)
        {
            if (command[i] == 1)
            {
                WRITE_COMMAND_1();
            }
            else
            {
                WRITE_COMMAND_0();
            }
        }

        for (int i = 0; i < 6; i++)
        {
            if (argument[i] == 1)
            {
                WRITE_COMMAND_1();
            }
            else
            {
                WRITE_COMMAND_0();
            }
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
    while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 0)
    {
    }

    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) != 0)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

        for (int i = 0; i < 3; i++)
        {
            READ_PIN_B14(ident[i]);
        }

        for (int i = 0; i < 10; i++)
        {
            READ_PIN_B14(calibBin[i]);
        }

        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        HAL_TIM::DelayUS(8);
    }

    int len = 10;
    calibNumber = 0;
    int base = 1;

    for (int i = len - 1; i >= 0; i--)
    {
        if (calibBin[i] == 1)
        {
            calibNumber += base;
        }
        base = base * 2;
    }
}


void FPGA::WriteData()
{
    int negative = 1024;

    if(PageIndication::calibration.Is(Calibration::Pressed))
    {
        if(calibNumber + NAC < 0)
        {
            calibNumber = 0;
            NAC = 0;
        }
        calibNumber = calibNumber + NAC;
        MathFPGA::DecToBin(calibNumber, encData);
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
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
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
            if (encData[i] == 1)
            {
                WRITE_COMMAND_1();
            }
            else
            {
                WRITE_COMMAND_0();
            }
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
    return calibNumber;
}


void FPGA::RefreshAuto()
{
    for (int i = 0; i < 10; i++)
    {
        minAuto[i] = 0;
        midAuto[i] = 0;
        maxAuto[i] = 0;
    }
}
