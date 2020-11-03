#include "defines.h"
#include "Settings.h"
#include "Hardware/FPGA.h"
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

static char dataA [32]; 
static char dataB [32];
static char procData[40] = { 0 };
static char identInfo[10] = { 0 };
static char autoData[20];
static char minAutoData[7];
static char maxAutoData[7];

static float decDataA;
static float decDataB;
static float decDataC;
static int decDA;
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

static char minAuto[10] = { 0 };
static char midAuto[10] = { 0 };
static char maxAuto[10] = { 0 };
static int decMinAuto;
static int decMidAuto;
static int decMaxAuto;

static int decPeriod;
static int decDuration;
static char period[32];
static char duration[32];

static int emptyZeros;
static int manualZeros = 1;

static float interpol;

static float dutyCycle;

static bool autoMode = false;

static char calibBin[10];
static int calibNumber = 0;
static int NAC = 0;


static void BinToDec() 
{ 
    int len = 32; 
    decDataA = 0; 
    int baseA = 1; 

    for (int i = len - 1; i >= 0; i--) 
    { 
        if (dataA[i] == 1) 
        {
            decDataA += (float)baseA;
        }            
        baseA = baseA * 2; 
    }

    if(CurrentModeMeasureFrequency::Is_AC_or_BC())
    {
        decDataB = 0;
        int baseB = 1; 

        for (int i = len - 1; i >= 0; i--) 
        { 
            if (dataB[i] == 1) 
            {
                decDataB += (float)baseB;
            }            
            baseB = baseB * 2; 
        }  
    }

    if(CURRENT_CHANNEL_IS_C)
    {
        decDataA = decDataA * 64 / 100;
    }
}

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

static void Calculation()
{
    int x = 0;
    if(CurrentTypeMeasure::IsFrequency())
    {
        if(CurrentModeMeasureFrequency::IsT_1())
        {
            int n = 1;
            manualZeros = 10;
            double test1;
            double test2;
            double test3;

            int tmet = PageModesA::periodTimeLabels.ToZeros();
            manualZeros *= PageModesA::periodTimeLabels.ToZeros() / 1000;

            n *= PageModesA::numberPeriods.ToAbs();
            manualZeros *= PageModesA::numberPeriods.ToAbs();
           
            test1 = (double)decDataA / tmet;
            test2 = test1 / n;
            test3 = 4 / test2;
            decDataA = (float)test3;
            decDA = (int)(decDataA / 2.0F);

            if(decDA < 1000)
            {
            }
            else if(decDA < 1000000)
            {
                decDataA = decDataA / 1000;
            }
            else
            {
                decDataA = decDataA / 1000000;
            }

            x = 1;
        }
        else if(CurrentModeMeasureFrequency::Is_AB_or_BA())
        {           
            x = PageModesA::numberPeriods.ToAbs();
        }
        else if(CurrentModeMeasureFrequency::Is_CA_or_CB())
        {
            decDataA = decDataA * 100;
            x = PageModesC::numberPeriods.ToAbs();
        }
        else if(CurrentModeMeasureFrequency::Is_AC_or_BC())
        {
            int sT = PageModesA::timeMeasure.ToMS();

            decDataA = decDataA / decDataB;
            decDataA = decDataA / 32;
            manualZeros = 1000000;
            manualZeros = manualZeros * sT;
            x = 1;
        }
        else
        {
            int mhz = 1000 * PageModesA::timeMeasure.ToMS();
            int khz = PageModesA::timeMeasure.ToMS();
            
            if(((decDataA / (float)khz) / 2.0F) < 1000.0F)
            {
                x = khz; 
            }
            else
            {
                x = mhz;            
            }
            decDA = (int)((decDataA / (float)khz) / 2.0F);
            if(CURRENT_CHANNEL_IS_C)
            {
                if(decDataA < 10000)
                {
                    decDataC = decDataA;               
                    khz = khz * 10;
                    x = khz;
                }
                else
                {
                    decDataC = decDataA;
                    mhz = mhz * 10;
                    x = mhz;
                }
            }
            if(CURRENT_CHANNEL_IS_D)
            {
                if(decDataA * 64.0F / (1000.0F * (float)khz) > 19000.0F)
                {
                    decDataC = 0;
                    x = khz;
                }
                else
                {
                    decDataC = (float)decDataA * 64 / 1000;
                    x = mhz;
                }
                decDA = (int)decDataC;
            }
        }
    }
    else if(CurrentTypeMeasure::IsDuration())
    {
        if(PageModesA::periodTimeLabels.IsT_7())
        {
            x = 10;
        }
        else if(PageModesA::periodTimeLabels.IsT_8() || PageModesA::periodTimeLabels.IsT_5())
        {
            x = 100;
        }
        else
        {
            x = 1;
        }
    }
    else if(CurrentTypeMeasure::IsPeriod())
    {
        if(ModeMeasurePeriod::Current().IsF_1())
        {
            int sT = PageModesA::timeMeasure.ToMS();
            
            decDA = (int)(decDataA / (2.0F * (float)sT));
            decDataA = 4 / decDataA;

            if(decDA >= 1000) //-V1051
            {
                decDataA = decDataA * 10000000 * (float)sT * (float)sT;
            }  
            else if(decDA <= 1)
            {
                decDataA = decDataA * 10 * (float)sT * (float)sT;
            }
            else
            {
                decDataA = decDataA * 10000 * (float)sT * (float)sT;
            }

            x = sT * 10;
        }
        else
        {
            int usT = 1;

            if(PageModesA::periodTimeLabels.IsT_7() || PageModesA::periodTimeLabels.IsT_4())
            {
                usT *= 10;
            }
            else if(PageModesA::periodTimeLabels.IsT_8() || PageModesA::periodTimeLabels.IsT_5())
            {
                usT *= 100;
            }
            
            x = usT * PageModesA::numberPeriods.ToAbs();
        }
    }
    if(CURRENT_CHANNEL_IS_D)
    {
        decDataA = decDataC * 2;
    }
    decDataA = (float)decDataA / (2.0F * (float)x);
    emptyZeros = x;

    if(manualZeros != 1) //-V1051
    {
        emptyZeros = manualZeros;
        manualZeros = 1;
    }
}

static void CalculationInterpole() 
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

static void CalculationAuto() 
{     
    decMinAuto = 0;
    decMidAuto = 0;
    decMaxAuto = 0; 
    int base1 = 1; 
    int base2 = 1; 
    int base3 = 1; 
    int len = 10;

    for (int i = len - 1; i >= 0; i--) 
    { 
        if (minAuto[i] == 1) 
        {
            decMinAuto += base1;
        }            
        base1 = base1 * 2; 
    }

    for (int i = len - 1; i >= 0; i--) 
    { 
        if (midAuto[i] == 1) 
        {
            decMidAuto += base2;
        }            
        base2 = base2 * 2; 
    } 

    for (int i = len - 1; i >= 0; i--) 
    { 
        if (maxAuto[i] == 1) 
        {
            decMaxAuto += base3;
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
        if (ModeMeasureDuration::Current().Is_Ndt_1ns() && PageModesA::InterpoleCheck())
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


char* FPGA::GiveData()
{
    if(CurrentTypeMeasure::IsCountPulse())
    {
        BinToDec();
        decDataA = decDataA / 2;

        if(CURRENT_CHANNEL_IS_C)
        {
            decDataA = decDataA * 100;
        }

        if(CurrentModeMeasureCountPulse::IsBig_T())
        {
            decDataA /= (float)PageModesA::numberPeriods.ToAbs();
        }

        std::sprintf(procData, "%10.0f", decDataA);

        return procData;
    }
    else
    {
        if(CurrentModeMeasureFrequency::IsTachometer())
        {
            BinToDec();
            decDataA = decDataA / 2;
            std::sprintf(procData, "%10.0f", decDataA);

            return procData;
        }
        else if (CURRENT_CHANNEL_IS_A && (PageModesA::modeMeasureFrequency.IsComparator() && PageModesA::typeMeasure.IsFrequency())) 
        {
            CalculationComparator();
            float top = 200.0F;
            float n = 5000000.0F;
            float dx = ((decTizm * 100) / decNkal);
            float k = (n - decFx) / n;
            decDataA = k - (dx / top) / n;
            decDataA = decDataA * 1000000;
            std::sprintf(procData, "%10.3f", decDataA);

            return procData;
        }
        else if(ModeMeasureDuration::Current().Is_Ndt_1ns() && PageModesA::InterpoleCheck())
        {
            CalculationInterpole();
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
            BinToDec();
            Calculation();

            int pow = 0;

            while (emptyZeros >= 10)
            {
                pow++;
                emptyZeros /= 10;
            }

            if (pow < 10)
            {
                char format[10];
                std::strcpy(format, "%10.0f");
                format[4] = (char)(pow | 0x30);
                std::sprintf(procData, format, decDataA);
            }
            else
            {
                char format[10];
                std::strcpy(format, "%10.10f");
                format[5] = (char)((pow - 10) | 0x30);
                std::sprintf(procData, format, decDataA);
            }

            emptyZeros = 1;
        }

        return procData;
    }
}

char* FPGA::GiveSpec() //-V2008
{
        if(PageModesA::InterpoleCheck() && ModeMeasureDuration::Current().Is_Ndt_1ns())
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
            if((CURRENT_CHANNEL_IS_A && (PageModesA::modeMeasureFrequency.IsAB() || PageModesA::modeMeasureFrequency.IsAC())) ||
                (CURRENT_CHANNEL_IS_B && (PageModesB::modeMeasureFrequency.IsBA() || PageModesB::modeMeasureFrequency.IsBC())) ||
                (CURRENT_CHANNEL_IS_C && (PageModesC::modeMeasureFrequency.IsCA() || PageModesC::modeMeasureFrequency.IsCB())) ||
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
                        if(decDA < 1000)
                        {
                            std::strcpy(spec, " Hz");
                        }
                        else if(decDA < 1000000)
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
                            if (decDataC / 2 < 10000)
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
                            if (decDA < 1000)
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
                        if(decDA >= 1000)
                        {
                            std::strcpy(spec, " ns");
                        }
                        else if(decDA <= 1)
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

int FPGA::MidAuto()
{
    CalculationAuto();
    return decMidAuto;
}

int FPGA::MinAuto()
{
    CalculationAuto();
    return decMinAuto;
}

int FPGA::MaxAuto()
{
    CalculationAuto();
    return decMaxAuto;
}

void FPGA::RefreshAuto()
{
    for(int i = 0; i < 10; i++)
    {
        minAuto[i] = 0;
        midAuto[i] = 0;
        maxAuto[i] = 0;
    }
}

char *FPGA::GiveAuto()
{
    CalculationAuto();
    SU::Int2String((decMinAuto - 512) * 2, minAutoData);
    SU::Int2String((decMaxAuto - 512) * 2, maxAutoData);
    std::strcpy(autoData, "Макс ");
    std::strcat(autoData, maxAutoData);
    std::strcat(autoData, " Мин ");
    std::strcat(autoData, minAutoData);
 
    if (CURRENT_CHANNEL_IS_A)
    {
        LEVEL_SYNCH_A = (decMidAuto - 512) * 2;
        NA = decMidAuto - 512;
    }

    if (CURRENT_CHANNEL_IS_B)
    {
        LEVEL_SYNCH_B = (decMidAuto - 512) * 2;
        NB = decMidAuto - 512;

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
