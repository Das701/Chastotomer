#include "defines.h"
#include "stm32f4xx_hal.h"
#include "Hardware/HAL.h"
#include "Utils/String.h"
#include "Menu/Menu.h"
#include "Settings.h"
#include <cstring>
#include <string.h>
#include <math.h>
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageChannelB.h"
#include "Menu/Pages/PageModes.h"
#include "Menu/Pages/PageModesB.h"
#include "Menu/Pages/PageModesC.h"
#include "Menu/Hint.h"

#define    DWT_CYCCNT    *(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   *(volatile unsigned long *)0xE0001000
#define    SCB_DEMCR     *(volatile unsigned long *)0xE000EDFC
    
static char dataA [32]; 
//static char dataB [32];
static char procData[30] = { 0 };
static int decDataA;
static int decDataAOst;
static char procDataOst[10] = { 0 };
static int zeros;
static int zerosA;
static char procInterpol[10] = { 0 };
static char procDataInterpol[30] = { 0 };

static char procDcycle[10] = { 0 };
static char procDataDcycle[30] = { 0 };
//static int DACA = 0;
//static int DACB = 0;

static int NA = 0;
static int NB = 0;

static char encData[10];
static char ident[3];
static char timer1[27];
static int decTimer1;
static int decCAL1;
static int decCAL2;

static char CAL1[24];
static char CAL2[24];

static int decPeriod;
static int decDuration;
static char period[32];
static char duration[32];

static int emptyZeros;

static int interpol;
static int interpolOst;

static int dutyCycle;
static int dutyCycleOst;

void DWT_Init(void)
{
        //разрешаем использовать счётчик
        SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
         //обнуляем значение счётного регистра
	DWT_CYCCNT  = 0;
         //запускаем счётчик
	DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk; 
}

static __inline uint32_t delta(uint32_t t0, uint32_t t1)
{
    return (t1 - t0); 
}
void delay_us(uint32_t us)
{
      uint32_t t0 =  DWT->CYCCNT;
      uint32_t us_count_tic =  us * (SystemCoreClock/1000000);
      while (delta(t0, DWT->CYCCNT) < us_count_tic) ;
}

static void BinToDec() 
{ 
    int len = 32; 
    decDataA = 0; 
    int baseA = 1; 
    for (int i = len - 1; i >= 0; i--) 
    { 
        if (dataA[i] == 1) 
        {
            decDataA += baseA;
        }            
        baseA = baseA * 2; 
    }  
//    else if(CURRENT_CHANNEL_IS_B)
//    {
//        decDataB = 0;
//        int baseB = 1; 
//        for (int i = len - 1; i >= 0; i--) 
//        { 
//            if (dataB[i] == 1) 
//            {
//                decDataB += baseB;
//            }            
//            baseB = baseB * 2; 
//        }  
//    }
}

static void CalculationDcycle() 
{     
    decPeriod = 0;
    decDuration = 0;
    int base1 = 1; 
    int base2 = 1; 
    int len = 32;  
    int period1;
    int duration1;
    for (int i = len - 1; i >= 0; i--) 
    { 
        if (period[i] == 1) 
        {
            decPeriod += base1;
        }            
        base1 = base1 * 2; 
    }
    decPeriod = decPeriod/2;
    for (int i = len - 1; i >= 0; i--) 
    { 
        if (duration[i] == 1) 
        {
            decDuration += base2;
        }            
        base2 = base2 * 2; 
    }   
    
    decDuration = decDuration/2;
    dutyCycle = decDuration/decPeriod;
    dutyCycleOst = decDuration%decPeriod;
    while(dutyCycleOst > 99)
    {
        dutyCycleOst = dutyCycleOst / 10;
    }
    if(decPeriod > decDuration)
    {
        zeros = 0;
        period1 = decPeriod;
        duration1 = decDuration;
        while((duration1*10)/period1 == 0)
        {
            duration1 = duration1*10;
            zeros++;
        }
    }
}

static void Calculation()
{
    int datA = 0;
    int x;
    if((CURRENT_CHANNEL_IS_A && (PageModes::typeMeasure == TypeMeasure::Frequency)) ||
       (CURRENT_CHANNEL_IS_B && (PageModesB::typeMeasureB == TypeMeasureB::Frequency)) ||
       (CURRENT_CHANNEL_IS_C && (PageModesC::typeMeasureC == TypeMeasureC::Frequency)))
    {
        if((CURRENT_CHANNEL_IS_A && (PageModes::modeMeasureFrequency == ModeMeasureFrequency::T_1)) || 
            (CURRENT_CHANNEL_IS_B && (PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::T_1)))
        {
            int msF = 10;
            decDataA = decDataA*10;
            if((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_3)) ||
            (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_3)))
            {
                msF = msF*1;
            }
            else if((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_4)) ||
            (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_4)))
            {
                msF = msF*10;
            }
            else if((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_5)) ||
            (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_5)))
            {
                msF = msF*100;
            }
            else if((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_6)) ||
            (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_6)))
            {
                msF = msF*1000;
            }
            else if((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_7)) ||
            (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_7)))
            {
                msF = msF*10000;
            }
            else if((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_8)) ||
            (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_8)))
            {
                msF = msF*100000;
            }
            if((CURRENT_CHANNEL_IS_A && (PageModes::numberPeriods == NumberPeriods::_1)) ||
            (CURRENT_CHANNEL_IS_B && (PageModesB::numberPeriodsB == NumberPeriodsB::_1)))
            {
                msF = msF*1;
            }
            else if((CURRENT_CHANNEL_IS_A && (PageModes::numberPeriods == NumberPeriods::_10)) ||
            (CURRENT_CHANNEL_IS_B && (PageModesB::numberPeriodsB == NumberPeriodsB::_10)))
            {
                msF = msF*10;
            }
            else if((CURRENT_CHANNEL_IS_A && (PageModes::numberPeriods == NumberPeriods::_100)) ||
            (CURRENT_CHANNEL_IS_B && (PageModesB::numberPeriodsB == NumberPeriodsB::_100)))
            {
                msF = msF*100;
            }
            else if((CURRENT_CHANNEL_IS_A && (PageModes::numberPeriods == NumberPeriods::_1K)) ||
            (CURRENT_CHANNEL_IS_B && (PageModesB::numberPeriodsB == NumberPeriodsB::_1K)))
            {
                msF = msF*1000;
            }
            else if((CURRENT_CHANNEL_IS_A && (PageModes::numberPeriods == NumberPeriods::_10K)) ||
            (CURRENT_CHANNEL_IS_B && (PageModesB::numberPeriodsB == NumberPeriodsB::_10K)))
            {
                msF = msF*10000;
            }
            else if((CURRENT_CHANNEL_IS_A && (PageModes::numberPeriods == NumberPeriods::_100K)) ||
            (CURRENT_CHANNEL_IS_B && (PageModesB::numberPeriodsB == NumberPeriodsB::_100K)))
            {
                msF = msF*100000;
            }
            x = msF;
        }
        else
        {
        int mhz = 1000;
        int khz = 1;
        if((CURRENT_CHANNEL_IS_A && (PageModes::timeMeasure == TimeMeasure::_1ms)) ||
           (CURRENT_CHANNEL_IS_B && (PageModesB::timeMeasureB == TimeMeasureB::_1ms)) ||
           (CURRENT_CHANNEL_IS_C && (PageModesC::timeMeasureC == TimeMeasureC::_1ms)))
        {
            mhz = mhz*1;
            khz = khz*1;
        }
        else if((CURRENT_CHANNEL_IS_A && (PageModes::timeMeasure == TimeMeasure::_10ms)) ||
           (CURRENT_CHANNEL_IS_B && (PageModesB::timeMeasureB == TimeMeasureB::_10ms)) ||
           (CURRENT_CHANNEL_IS_C && (PageModesC::timeMeasureC == TimeMeasureC::_10ms)))
        {
            mhz = mhz*10;
            khz = khz*10;
        }
        else if((CURRENT_CHANNEL_IS_A && (PageModes::timeMeasure == TimeMeasure::_100ms)) ||
           (CURRENT_CHANNEL_IS_B && (PageModesB::timeMeasureB == TimeMeasureB::_100ms)) ||
           (CURRENT_CHANNEL_IS_C && (PageModesC::timeMeasureC == TimeMeasureC::_100ms)))
        {
            mhz = mhz*100;
            khz = khz*100;
        }
        else if((CURRENT_CHANNEL_IS_A && (PageModes::timeMeasure == TimeMeasure::_1s)) ||
           (CURRENT_CHANNEL_IS_B && (PageModesB::timeMeasureB == TimeMeasureB::_1s)) ||
           (CURRENT_CHANNEL_IS_C && (PageModesC::timeMeasureC == TimeMeasureC::_1s)))
        {
            mhz = mhz*1000;
            khz = khz*1000;
        }
        else if((CURRENT_CHANNEL_IS_A && (PageModes::timeMeasure == TimeMeasure::_10s)) ||
           (CURRENT_CHANNEL_IS_B && (PageModesB::timeMeasureB == TimeMeasureB::_10s)) ||
           (CURRENT_CHANNEL_IS_C && (PageModesC::timeMeasureC == TimeMeasureC::_10s)))
        {
            mhz = mhz*10000;
            khz = khz*10000;
        }
        else if((CURRENT_CHANNEL_IS_A && (PageModes::timeMeasure == TimeMeasure::_100s)) ||
           (CURRENT_CHANNEL_IS_B && (PageModesB::timeMeasureB == TimeMeasureB::_100s)) ||
           (CURRENT_CHANNEL_IS_C && (PageModesC::timeMeasureC == TimeMeasureC::_100s)))
        {
            mhz = mhz*100000;
            khz = khz*100000;
        }
        if((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_6 || PageModes::periodTimeLabels == PeriodTimeLabels::T_7 || PageModes::periodTimeLabels == PeriodTimeLabels::T_8)) ||
           (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_6 || PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_7 || PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_8)) ||
           (CURRENT_CHANNEL_IS_C && (PageModesC::periodTimeLabelsC == PeriodTimeLabelsC::T_6 || PageModesC::periodTimeLabelsC == PeriodTimeLabelsC::T_7 || PageModesC::periodTimeLabelsC == PeriodTimeLabelsC::T_8)))
        {
            x = mhz;
        }
        else if((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_3 || PageModes::periodTimeLabels == PeriodTimeLabels::T_4 || PageModes::periodTimeLabels == PeriodTimeLabels::T_5)) ||
           (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_3 || PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_4 || PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_5)) ||
           (CURRENT_CHANNEL_IS_C && (PageModesC::periodTimeLabelsC == PeriodTimeLabelsC::T_3 || PageModesC::periodTimeLabelsC == PeriodTimeLabelsC::T_4 || PageModesC::periodTimeLabelsC == PeriodTimeLabelsC::T_5)))
        {
            x = khz;            
        }
        }
    }
    else if((CURRENT_CHANNEL_IS_A && (PageModes::typeMeasure == TypeMeasure::Duration)) ||
       (CURRENT_CHANNEL_IS_B && (PageModesB::typeMeasureB == TypeMeasureB::Duration)))
    {
        int us = 1;
        if((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_6)) ||
           (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_6)))
        {
        } 
        else if((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_7)) ||
           (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_7)))
        {
            us = us*10;
        } 
        else if(((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_8)) ||
                (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_8))) || 
                ((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_5)) ||
                (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_5))))
        {
            us = us*100;
        } 
        x = us;
    }
    else if((CURRENT_CHANNEL_IS_A && (PageModes::typeMeasure == TypeMeasure::Period)) ||
       (CURRENT_CHANNEL_IS_B && (PageModesB::typeMeasureB == TypeMeasureB::Period)))
    {
        int usT = 1;
        if(((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_6)) ||
                (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_6))) || 
                ((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_3)) ||
                (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_3))))
        {
            usT = usT*1;
        }
        else if(((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_7)) ||
                (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_7))) || 
                ((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_4)) ||
                (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_4))))
        {
            usT = usT*10;
        }
        else if(((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_8)) ||
                (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_8))) || 
                ((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_5)) ||
                (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_5))))
        {
            usT = usT*100;
        }
        if((CURRENT_CHANNEL_IS_A && (PageModes::numberPeriods == NumberPeriods::_1)) ||
           (CURRENT_CHANNEL_IS_B && (PageModesB::numberPeriodsB == NumberPeriodsB::_1)))
        {
            usT = usT*1;
        }
        else if((CURRENT_CHANNEL_IS_A && (PageModes::numberPeriods == NumberPeriods::_10)) ||
           (CURRENT_CHANNEL_IS_B && (PageModesB::numberPeriodsB == NumberPeriodsB::_10)))
        {
            usT = usT*10;
        }
        else if((CURRENT_CHANNEL_IS_A && (PageModes::numberPeriods == NumberPeriods::_100)) ||
           (CURRENT_CHANNEL_IS_B && (PageModesB::numberPeriodsB == NumberPeriodsB::_100)))
        {
            usT = usT*100;
        }
        else if((CURRENT_CHANNEL_IS_A && (PageModes::numberPeriods == NumberPeriods::_1K)) ||
           (CURRENT_CHANNEL_IS_B && (PageModesB::numberPeriodsB == NumberPeriodsB::_1K)))
        {
            usT = usT*1000;
        }
        else if((CURRENT_CHANNEL_IS_A && (PageModes::numberPeriods == NumberPeriods::_10K)) ||
           (CURRENT_CHANNEL_IS_B && (PageModesB::numberPeriodsB == NumberPeriodsB::_10K)))
        {
            usT = usT*10000;
        }
        else if((CURRENT_CHANNEL_IS_A && (PageModes::numberPeriods == NumberPeriods::_100K)) ||
           (CURRENT_CHANNEL_IS_B && (PageModesB::numberPeriodsB == NumberPeriodsB::_100K)))
        {
            usT = usT*100000;
        }
        x = usT;
    }
    datA = decDataA/2;
    decDataA = decDataA/(2*x);
    decDataAOst = datA%x;
    zerosA = 0;
    if(decDataAOst != 0)
    {
        while((datA*10)/x == 0)
        {
            datA = datA*10;
            zerosA++;
        }
    }
    emptyZeros = x;
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
    interpol = (100*decTimer1)/(decCAL2 - decCAL1);
    interpolOst = (100*decTimer1)%(decCAL2 - decCAL1);
    while(interpolOst > 99)
    {
        interpolOst = interpolOst / 10;
    }
}
void DecToBin(int dec, char* bin) 
{ 
    int x = dec;
    for(int i = 0; i < 10; i++)
    {
        if(x%2 != 0)
        {
            bin[i] = 1;
        }
        else
        {
            bin[i] = 0;
        }
        x = x/2;
    }   
}


void PLIS::Init()
{
    DWT_Init();
    
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

void PLIS::Update()
{
    if(((PageModes::modeMeasureDuration == ModeMeasureDuration::Ndt_1ns && CURRENT_CHANNEL_IS_A) || 
            (PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Ndt_1ns && CURRENT_CHANNEL_IS_B))
            && PageModes::InterpoleCheck())
    {
        if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) != 0)
        {            
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
            for(int i = 0; i < 3; i++)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                delay_us(2);
                ident[i] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
                delay_us(2);
            }
            for(int i = 0; i < 24; i++)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                delay_us(2);
                timer1[i] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
                delay_us(2);
            }
            for(int i = 0; i < 24; i++)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                delay_us(2);
                CAL1[i] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
                delay_us(2);
            }
            for(int i = 0; i < 24; i++)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                delay_us(2);
                CAL2[i] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
                delay_us(2);
            }
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
            delay_us(8);
            
        }  
    }
    else if(((PageModes::modeMeasureDuration == ModeMeasureDuration::Dcycle && CURRENT_CHANNEL_IS_A) || 
        (PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Dcycle && CURRENT_CHANNEL_IS_B))
        && PageModes::DCycleCheck())
    {
        if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) != 0)
        {            
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
            for(int i = 0; i < 32; i++)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                delay_us(2);
                period[i] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
                delay_us(2);
            }
            for(int i = 0; i < 32; i++)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                delay_us(2);
                duration[i] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
                delay_us(2);
            }
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
            delay_us(8);
        }
    }
    else
    {
        if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) != 0)
        {            
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
            for(int i = 0; i < 32; i++)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                delay_us(2);
                dataA[i] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
                delay_us(2);
            }
//            if(CURRENT_CHANNEL_IS_B)
//            {
//                for(int i = 0; i < 32; i++)
//                {
//                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
//                    delay_us(2);
//                    dataB[i] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
//                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
//                    delay_us(2);
//                }
//            }
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
            delay_us(8);
        }     
    }    
}


char* PLIS::GiveData()
{
    if(CURRENT_CHANNEL_IS_C)
    {
        BinToDec();
        Int2String(decDataA*64/200, procData);
        return procData;
    }
    else
    {
    if(((PageModes::modeMeasureDuration == ModeMeasureDuration::Ndt_1ns && CURRENT_CHANNEL_IS_A) || 
        (PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Ndt_1ns && CURRENT_CHANNEL_IS_B))
        && PageModes::InterpoleCheck())
    {
        CalculationInterpole();
        Int2String(interpol, procDataInterpol);
        std::strcat(procDataInterpol, ".");
        Int2String(interpolOst, procInterpol);
        std::strcat(procDataInterpol, procInterpol);
        return procDataInterpol;
    }
    else if(((PageModes::modeMeasureDuration == ModeMeasureDuration::Dcycle && CURRENT_CHANNEL_IS_A) || 
        (PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Dcycle && CURRENT_CHANNEL_IS_B))
        && PageModes::DCycleCheck())
    {
        CalculationDcycle();
        Int2String(dutyCycle, procDataDcycle);
        std::strcat(procDataDcycle, ".");
        while(zeros != 0)
        {
            std::strcat(procDataDcycle, "0");
            zeros--;
        }
        Int2String(dutyCycleOst, procDcycle);
        std::strcat(procDataDcycle, procDcycle);
        return procDataDcycle;
    }
    else
    {
        BinToDec();
        Calculation();
        Int2String(decDataA, procData);
        if(decDataAOst != 0)
        {
            std::strcat(procData, ".");
            Int2String(decDataAOst, procDataOst);
            while(zerosA != 0)
            {
                std::strcat(procData, "0");
                zerosA--;
            }
            std::strcat(procData, procDataOst);
        }
        else
        {
            if(emptyZeros != 1)
            {
            std::strcat(procData, ".");
            }
            emptyZeros = emptyZeros/10;
            while(emptyZeros != 0)
            {
                std::strcat(procData, "0");
                emptyZeros = emptyZeros/10;
            }
        }
        if((CURRENT_CHANNEL_IS_A && (PageModes::typeMeasure == TypeMeasure::Frequency)) ||
           (CURRENT_CHANNEL_IS_B && (PageModesB::typeMeasureB == TypeMeasureB::Frequency)) ||
           (CURRENT_CHANNEL_IS_C && (PageModesC::typeMeasureC == TypeMeasureC::Frequency)))
        {
            if((CURRENT_CHANNEL_IS_A && (PageModes::modeMeasureFrequency == ModeMeasureFrequency::T_1)) || 
            (CURRENT_CHANNEL_IS_B && (PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::T_1)))
            {
                std::strcat(procData, " ms");
            }
            else
            {
                if((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_3 || PageModes::periodTimeLabels == PeriodTimeLabels::T_4 || PageModes::periodTimeLabels == PeriodTimeLabels::T_5)) ||
                (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_3 || PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_4 || PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_5)) ||
                (CURRENT_CHANNEL_IS_C && (PageModesC::periodTimeLabelsC == PeriodTimeLabelsC::T_3 || PageModesC::periodTimeLabelsC == PeriodTimeLabelsC::T_4 || PageModesC::periodTimeLabelsC == PeriodTimeLabelsC::T_5)))
                {
                    std::strcat(procData, " kHz");
                    
                }
                else if((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_6|| PageModes::periodTimeLabels == PeriodTimeLabels::T_7 || PageModes::periodTimeLabels == PeriodTimeLabels::T_8)) ||
                        (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_6 || PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_7 || PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_8)) ||
                        (CURRENT_CHANNEL_IS_C && (PageModesC::periodTimeLabelsC == PeriodTimeLabelsC::T_6 || PageModesC::periodTimeLabelsC == PeriodTimeLabelsC::T_7 || PageModesC::periodTimeLabelsC == PeriodTimeLabelsC::T_8)))
                {
                    std::strcat(procData, " MHz");
                }
            }
        }
        else
        {
            //if((PageModes::modeMeasureDuration == ModeMeasureDuration::Ndt2 && CURRENT_CHANNEL_IS_A) || 
            //   (PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Ndt2 && CURRENT_CHANNEL_IS_B))
            //{
            //    std::strcat(procData, " E-3");
            //}
            //else
            //{
                if(((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_5)) ||
                    (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_5))) || 
                    ((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_4)) ||
                    (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_4))) ||
                    ((CURRENT_CHANNEL_IS_A && (PageModes::periodTimeLabels == PeriodTimeLabels::T_3)) ||
                    (CURRENT_CHANNEL_IS_B && (PageModesB::periodTimeLabelsB == PeriodTimeLabelsB::T_3))))
                {
                    std::strcat(procData, " ms");
                }
                else
                {
                    std::strcat(procData, " us");
                }
            //}
        }
        return procData;
    }
    }
    
}

void PLIS::WriteCommand(char* command, char* argument)
{
    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == 0)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
        delay_us(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        delay_us(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
        delay_us(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
//        delay_us(2);
        for(int i = 0; i < 4; i++)
        {
            if (command[i] == 1)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
                delay_us(2);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                delay_us(2);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
                delay_us(2);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                delay_us(2);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
            }
        }
        for(int i = 0; i < 6; i++)
        {
            if (argument[i] == 1)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
                delay_us(2);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                delay_us(2);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
                delay_us(2);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                delay_us(2);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
            }
        }
        delay_us(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
        delay_us(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        delay_us(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
        delay_us(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
    }
}

void PLIS::IncreaseN()
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

void PLIS::DecreaseN()
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

void PLIS::WriteData()
{
    int negative = 1024;
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
    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == 0)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
        delay_us(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
        delay_us(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
        delay_us(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
//        delay_us(2);
        for(int i = 9; i > -1; i--)
        {
            if (encData[i] == 1)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
                delay_us(2);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                delay_us(2);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
                delay_us(2);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                delay_us(2);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
            }
        }
        delay_us(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
        delay_us(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        delay_us(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
        delay_us(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
    }
}
