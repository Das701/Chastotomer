#include "defines.h"
#include "Log.h"
#include "Settings.h"
#include "Calculate/MathFPGA.h"
#include "Display/Display.h"
#include "Display/Objects.h"
#include "Hardware/FPGA.h"
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

#define PinRD       GPIOC, GPIO_PIN_8                   // ���� ���������� ������
#define PinWR       GPIOC, GPIO_PIN_9                   // ���� ���������� � ������
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


static uint ident = 0;      // ��� �������� ����������� ��������������� �� FPGA
static char encData[10];
static bool autoMode = false;
static bool isOverloaded = false;

uint FPGA::GovernorData::kCalib = 0;     // ��� �������� ����������� ��������������� �� FPGA
int FPGA::GovernorData::NAC = 0;         // �������� ��� �������������� ������������

uint FPGA::timeChangeSettings = 0;

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
        if (TIME_MS - timeChangeSettings < 200)
        {
            return;
        }

        if (Channel::Current()->mod.typeMeasure.IsDuration() && ModeDuration::Current().IsNdt_1ns())
        {
            ReadInterpolator();
        }
        else if(Channel::Current()->mod.typeMeasure.IsDuration() && 
            (ModeDuration::Current().IsFillFactor() || ModeDuration::Current().IsPhase()))
        {
            ReadFillFactorPhase();
        }
        else if (CURRENT_CHANNEL_IS_A && 
            (Channel::A->mod.modeFrequency.IsComparator() && Channel::A->mod.typeMeasure.IsFrequency())) 
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
              
                if((ModeFrequency::Current().IsRatioAC() || ModeFrequency::Current().IsRatioBC()) &&
                    Relation::IsEnabled())
                {
                    CycleRead(32, counterB, true);
                }

                Reset_CS; //-V2571

                LOG_WRITE("%d %d", counterA, counterB);
                ValueFPGA::Create(counterA, counterB);

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
        CycleRead(32, period, true);
        CycleRead(32, duration, true);
        Reset_CS;

        LOG_WRITE("%d %d", period, duration);
        ValueFPGA::Create(period, duration);

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

        LOG_WRITE("%d %d %d", timer, cal1, cal2);
        ValueFPGA::Create(timer, cal1, cal2);

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

        LOG_WRITE("%d %d %d %d %d", counter, interpol1, cal1, interpol2, cal2);
        ValueFPGA::Create(counter, interpol1, cal1, interpol2, cal2);

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
            MathFPGA::Auto::NA++;
        }
        else if(CURRENT_CHANNEL_IS_B) //-V2516
        {
            MathFPGA::Auto::NB++;
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
            MathFPGA::Auto::NA--;
        }
        else if(CURRENT_CHANNEL_IS_B) //-V2516
        {
            MathFPGA::Auto::NB--;
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

    if (Read_WR != 0)           // \todo � ���������, ���� ���������� �� �������� ���, ��� ���� � ���� ������� ��� ��������� � ����, �� ���������� ���� �������� ������ //-V2571
    {                           // ���� ���� �� �����, �������. ���������� ����� ������ ���� ���� ��� ���������� � 0
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
    while (Read_WR != 0)             // \todo ����������. ��������, �� �������� � ��������� ������ ������ ������� ����� ���������� //-V2571
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

    SetInvalidData();
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
        DecToBin(ValueCalibrator(), encData);
    }
    else
    {
        int negative = 1024;

        if (CURRENT_CHANNEL_IS_A)
        {
            if (MathFPGA::Auto::NA < 0)
            {
                DecToBin(negative + MathFPGA::Auto::NA, encData);
            }
            else
            {
                DecToBin(MathFPGA::Auto::NA, encData);
            }
        }
        else if (CURRENT_CHANNEL_IS_B) //-V2516
        {
            if (MathFPGA::Auto::NB < 0)
            {
                DecToBin(negative + MathFPGA::Auto::NB, encData);
            }
            else
            {
                DecToBin(MathFPGA::Auto::NB, encData);
            }
        }
    }
}


bool FPGA::IsOverloaded()
{
    return isOverloaded;
}


void FPGA::SetInvalidData()
{
    timeChangeSettings = TIME_MS;

    ValueFPGA::SetInvalidData();
}


void FPGA::DecToBin(int dec, char *bin)
{
    int x = dec;
    for (int i = 0; i < 10; i++)
    {
        if (x % 2 != 0) { bin[i] = 1; } //-V2563
        else { bin[i] = 0; } //-V2563
        x = x / 2;
    }
}


String FPGA::BinToString(pString bin, int num)
{
    char buffer[20];

    for (int i = 0; i < num; i++)
    {
        if (bin[i] == 0) //-V2563
        {
            buffer[i] = '0';
        }
        else
        {
            buffer[i] = '1';
        }
    }

    buffer[num] = '\0';

    return String(buffer);
}
