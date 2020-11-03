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


static int dcycleZeros = 0;


static char procData[40] = { 0 };
static char identInfo[10] = { 0 };
static char autoData[20];
static char minAutoData[7];
static char maxAutoData[7];

static char procDataInterpol[30] = { 0 };
static char spec[10] = { 0 };

static char procDataDcycle[30] = { 0 };

static int NA = 0; //-V707
static int NB = 0; //-V707

static char encData[10];
static char ident[4];
static char timer1[27];
static int decTimer1;
static int decCAL1;
static int decCAL2;

static char binFx[32];
static char binTizm[16];
static char binNkal[16];
static float decFx;
static float decTizm;
static float decNkal;

static char CAL1[24];
static char CAL2[24];

static int decPeriod;
static int decDuration;
static char period[32];
static char duration[32];

static float interpol;

static float dutyCycle;

static bool autoMode = false;

static char calibBin[10];
static int calibNumber = 0;
static int NAC = 0;


static void CalculationDcycle() 
{     
    decPeriod = 0;
    decDuration = 0;
    int base1 = 1; 
    int base2 = 1; 
    int len = 32;  
    for (int i = len - 1; i >= 0; i--) 
    { 
        if (period[i] == 1) 
        {
            decPeriod += base1;
        }            
        base1 = base1 * 2; 
    }
    for (int i = len - 1; i >= 0; i--) 
    { 
        if (duration[i] == 1) 
        {
            decDuration += base2;
        }            
        base2 = base2 * 2; 
    }   
    dutyCycle = (float)decDuration / (float)decPeriod;

    if(ModeMeasureDuration::Current().Is_Phase())
    {
        dutyCycle = dutyCycle*360;
    }
    else
    {
        if(dutyCycle < 0)
        {
            dutyCycle = dutyCycle*(-1);
        }
        if(dutyCycle != 0.0F) //-V2550 //-V550
        {
            while(dutyCycle < 1)
            {
                dutyCycle = dutyCycle * 10;
                dcycleZeros++;
            }
        }
    }
}


static void CalculationInterpolate() 
{     
    decTimer1 = 0;
    decCAL1 = 0;
    decCAL2 = 0; 
    int base1 = 1; 
    int base2 = 1; 
    int base3 = 1; 
    int len = 24;   

    for (int i = len - 1; i >= 0; i--) 
    { 
        if (timer1[i] == 1) 
        {
            decTimer1 += base1;
        }            
        base1 = base1 * 2; 
    }

    for (int i = len - 1; i >= 0; i--) 
    { 
        if (CAL1[i] == 1) 
        {
            decCAL1 += base2;
        }            
        base2 = base2 * 2; 
    } 

    for (int i = len - 1; i >= 0; i--) 
    { 
        if (CAL2[i] == 1) 
        {
            decCAL2 += base3;
        }            
        base3 = base3 * 2; 
    }

    interpol = (float)(100 * decTimer1) / (float)(decCAL2 - decCAL1);
}

static void CalculationComparator() 
{     
    decFx = 0;
    decTizm = 0;
    decNkal = 0; 
    int base1 = 1; 
    int base2 = 1; 
    int base3 = 1; 
    int len1 = 32;
    int len2 = 16;

    for (int i = len1 - 1; i >= 0; i--) 
    { 
        if (binFx[i] == 1) 
        {
            decFx += (float)base1;
        }            
        base1 = base1 * 2; 
    }

    for (int i = len2 - 1; i >= 0; i--) 
    { 
        if (binTizm[i] == 1) 
        {
            decTizm += (float)base2;
        }            
        base2 = base2 * 2; 
    } 

    if (binTizm[0] == 1) 
    {
        decTizm = decTizm - 65536;
    }

    for (int i = len2 - 1; i >= 0; i--) 
    { 
        if (binNkal[i] == 1) 
        {
            decNkal += (float)base3;
        }            
        base3 = base3 * 2; 
    }   
}


void DecToBin(int dec, char* bin) 
{ 
    int x = dec;
    for(int i = 0; i < 10; i++)
    {
        if(x % 2 != 0)
        {
            bin[i] = 1;
        }
        else
        {
            bin[i] = 0;
        }
        x = x / 2;
    }   
}


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
    if(autoMode == true)
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
                READ_PIN_B14(MathFPGA::minAuto[i]);
            }

            for (int i = 0; i < 10; i++)
            {
                READ_PIN_B14(MathFPGA::midAuto[i]);
            }

            for (int i = 0; i < 10; i++)
            {
                READ_PIN_B14(MathFPGA::maxAuto[i]);
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
                    READ_PIN_B14(CAL1[i]);
                }

                for (int i = 0; i < 24; i++)
                {
                    READ_PIN_B14(CAL2[i]);
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
                    READ_PIN_B14(period[i]);
                }

                for (int i = 0; i < 32; i++)
                {
                    READ_PIN_B14(duration[i]);
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
                    READ_PIN_B14(binFx[i]);
                }

                for (int i = 0; i < 16; i++)
                {
                    READ_PIN_B14(binTizm[i]);
                }

                for (int i = 0; i < 16; i++)
                {
                    READ_PIN_B14(binNkal[i]);
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
                    READ_PIN_B14(MathFPGA::dataA[i]);
                }

                if(CurrentModeMeasureFrequency::Is_AC_or_BC() && PageModesA::RelationCheck())
                {
                    for (int i = 0; i < 32; i++)
                    {
                        READ_PIN_B14(MathFPGA::dataB[i]);
                    }
                }

                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
                HAL_TIM::DelayUS(8);
            }
        }
    }
}


char* FPGA::GiveData()
{
    if(CurrentTypeMeasure::IsCountPulse())
    {
        MathFPGA::BinToDec();
        MathFPGA::decDataA = MathFPGA::decDataA / 2;

        if(CURRENT_CHANNEL_IS_C)
        {
            MathFPGA::decDataA = MathFPGA::decDataA * 100;
        }

        if(CurrentModeMeasureCountPulse::IsBig_T())
        {
            MathFPGA::decDataA /= (float)PageModesA::numberPeriods.ToAbs();
        }

        std::sprintf(procData, "%10.0f", MathFPGA::decDataA);

        return procData;
    }
    else
    {
        if(CurrentModeMeasureFrequency::IsTachometer())
        {
            MathFPGA::BinToDec();
            MathFPGA::decDataA = MathFPGA::decDataA / 2;
            std::sprintf(procData, "%10.0f", MathFPGA::decDataA);

            return procData;
        }
        else if (CurrentModeMeasureFrequency::IsComparator()) 
        {
            CalculationComparator();
            float top = 200.0F;
            float n = 5000000.0F;
            float dx = ((decTizm * 100) / decNkal);
            float k = (n - decFx) / n;
            MathFPGA::decDataA = k - (dx / top) / n;
            MathFPGA::decDataA = MathFPGA::decDataA * 1000000;
            std::sprintf(procData, "%10.3f", MathFPGA::decDataA);

            return procData;
        }
        else if(ModeMeasureDuration::Current().Is_Ndt_1ns() && PageModesA::InterpolateCheck())
        {
            CalculationInterpolate();
            std::sprintf(procDataInterpol, "%10.2f", interpol);
            return procDataInterpol;
        }
        else if((ModeMeasureDuration::Current().Is_Dcycle() || ModeMeasureDuration::Current().Is_Phase()) && PageModesA::DCycleCheck())
        {
            CalculationDcycle();

            if (ModeMeasureDuration::Current().Is_Phase())
            {
                std::sprintf(procDataDcycle, "%10.3f", dutyCycle);
            }
            else
            {
                std::sprintf(procDataDcycle, "%10.7f", dutyCycle);
            }

            return procDataDcycle;
        }
        else
        {
            MathFPGA::BinToDec();
            MathFPGA::Calculate();

            int pow = 0;

            while (MathFPGA::emptyZeros >= 10)
            {
                pow++;
                MathFPGA::emptyZeros /= 10;
            }

            if (pow < 10)
            {
                char format[10];
                std::strcpy(format, "%10.0f");
                format[4] = (char)(pow | 0x30);
                std::sprintf(procData, format, MathFPGA::decDataA);
            }
            else
            {
                char format[10];
                std::strcpy(format, "%10.10f");
                format[5] = (char)((pow - 10) | 0x30);
                std::sprintf(procData, format, MathFPGA::decDataA);
            }

            MathFPGA::emptyZeros = 1;
        }

        return procData;
    }
}

char* FPGA::GiveSpec() //-V2008
{
        if(PageModesA::InterpolateCheck() && ModeMeasureDuration::Current().Is_Ndt_1ns())
        {
            std::strcpy(spec, " ns");
        }
        else if((PageModesA::typeMeasure.IsDuration() && PageModesA::modeMeasureDuration.Is_Dcycle() && CURRENT_CHANNEL_IS_A) || 
            (PageModesB::typeMeasure.IsDuration() && PageModesB::modeMeasureDuration.Is_Dcycle() && CURRENT_CHANNEL_IS_B) || 
            (PageModesA::typeMeasure.IsDuration() && PageModesA::modeMeasureDuration.Is_Phase() && CURRENT_CHANNEL_IS_A) || 
            (PageModesB::typeMeasure.IsDuration() && PageModesB::modeMeasureDuration.Is_Phase() && CURRENT_CHANNEL_IS_B))
        {
                if(ModeMeasureDuration::Current().Is_Phase())
                {
                    std::strcpy(spec, " $");
                }
                else
                {
                    std::strcpy(spec, " E-0");
                    spec[3] = (char)(dcycleZeros | 0x30);

                    dcycleZeros = 0;
                }
        }
        else
        {
            if((CURRENT_CHANNEL_IS_A && (PageModesA::modeMeasureFrequency.IsRatioAB() || PageModesA::modeMeasureFrequency.IsRatioAC())) ||
                (CURRENT_CHANNEL_IS_B && (PageModesB::modeMeasureFrequency.IsRatioBA() || PageModesB::modeMeasureFrequency.IsRatioBC())) ||
                (CURRENT_CHANNEL_IS_C && (PageModesC::modeMeasureFrequency.IsRatioCA() || PageModesC::modeMeasureFrequency.IsRatioCB())) ||
                ((CURRENT_CHANNEL_IS_A && PageModesA::modeMeasureFrequency.IsTachometer()) || (CURRENT_CHANNEL_IS_B && PageModesB::modeMeasureFrequency.IsTachometer())) ||
                (PageModesA::typeMeasure.IsCountPulse() || PageModesB::typeMeasure.IsCountPulse() || PageModesC::typeMeasure.IsCountPulse()))
            {
                std::strcpy(spec, " ");
            }
            else
            {
                if(CurrentTypeMeasure::IsFrequency())
                {
                    if(CurrentModeMeasureFrequency::IsT_1())
                    {
                        if(MathFPGA::decDA < 1000)
                        {
                            std::strcpy(spec, " Hz");
                        }
                        else if(MathFPGA::decDA < 1000000)
                        {
                            std::strcpy(spec, " kHz");
                        }
                        else
                        {
                            std::strcpy(spec, " MHz");
                        }
                    }
                    else if(PageModesA::modeMeasureFrequency.IsComparator() && CURRENT_CHANNEL_IS_A) 
                    {
                        std::strcpy(spec, " E-6");
                    }
                    else
                    {
                        if (CURRENT_CHANNEL_IS_C)
                        {
                            if (MathFPGA::decDataC / 2 < 10000)
                            {
                                std::strcpy(spec, " MHz");
                            }
                            else
                            {
                                std::strcpy(spec, " GHz");
                            }
                        }
                        else if (CURRENT_CHANNEL_IS_D)
                        {
                            {
                                std::strcpy(spec, " GHz");
                            }
                        }
                        else
                        {
                            if (MathFPGA::decDA < 1000)
                            {
                                std::strcpy(spec, " kHz");

                            }
                            else
                            {
                                std::strcpy(spec, " MHz");
                            }
                        }
                    }
                }
                else
                {
                    if((CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsPeriod() && PageModesA::modeMeasurePeriod.IsF_1()) ||
                        (CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsPeriod() && PageModesB::modeMeasurePeriod.IsF_1()))
                    {
                        if(MathFPGA::decDA >= 1000)
                        {
                            std::strcpy(spec, " ns");
                        }
                        else if(MathFPGA::decDA <= 1)
                        {
                            std::strcpy(spec, " ms");
                        }
                        else
                        {
                            std::strcpy(spec, " us");
                        }
                    }
                    else
                    {
                        PeriodTimeLabels &current = PeriodTimeLabels::Current();

                        if(current.IsT_3() || current.IsT_4() || current.IsT_5())
                        {
                            std::strcpy(spec, " ms");
                        }
                        else
                        {
                            std::strcpy(spec, " us");
                        }
                    } } } }   
            return spec;
}

char *FPGA::GiveIdent()
{
    if (ident[0] == 0)
    {
        std::strcpy(identInfo, "0");
    }
    else
    {
        std::strcpy(identInfo, "1");
    }

    for (int i = 1; i < 4; i++)
    {
        if (ident[i] == 0)
        {
            std::strcat(identInfo, "0");
        }
        else
        {
            std::strcat(identInfo, "1");
        }
    }

    return identInfo;
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
            NA++;
        }
        else if(CURRENT_CHANNEL_IS_B)
        {
            NB++;
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
            NA--;
        }
        else if(CURRENT_CHANNEL_IS_B)
        {
            NB--;
        }
    }
}


void FPGA::SwitchAuto()
{
    if(autoMode == false)
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


char *FPGA::GiveAuto()
{
    MathFPGA::CalculationAuto();
    SU::Int2String((MathFPGA::decMinAuto - 512) * 2, minAutoData);
    SU::Int2String((MathFPGA::decMaxAuto - 512) * 2, maxAutoData);
    std::strcpy(autoData, "Макс ");
    std::strcat(autoData, maxAutoData);
    std::strcat(autoData, " Мин ");
    std::strcat(autoData, minAutoData);
 
    if (CURRENT_CHANNEL_IS_A)
    {
        LEVEL_SYNCH_A = (MathFPGA::decMidAuto - 512) * 2;
        NA = MathFPGA::decMidAuto - 512;
    }

    if (CURRENT_CHANNEL_IS_B)
    {
        LEVEL_SYNCH_B = (MathFPGA::decMidAuto - 512) * 2;
        NB = MathFPGA::decMidAuto - 512;

    }

    return autoData;
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
        DecToBin(calibNumber, encData);
        NAC = 0;
    }
    else
    {
        if(CURRENT_CHANNEL_IS_A)
        {
                if(NA < 0)
                {
                    DecToBin(negative + NA, encData);
                }
                else
                {
                    DecToBin(NA, encData);
                }
        }
        else if(CURRENT_CHANNEL_IS_B)
        {
            if(NB < 0)
            {
                DecToBin(negative + NB, encData);
            }
            else
            {
                DecToBin(NB, encData);
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
