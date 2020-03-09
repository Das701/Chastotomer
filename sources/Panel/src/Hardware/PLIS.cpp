#include "defines.h"
#include "HAL.h"
#include "stm32f4xx_hal.h"
#include "Hardware/HAL.h"
#include "Utils/String.h"
#include <cstring>
#include <string.h>
#include <math.h>

#define    DWT_CYCCNT    *(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   *(volatile unsigned long *)0xE0001000
#define    SCB_DEMCR     *(volatile unsigned long *)0xE000EDFC
char dataA [32]; 
char dataB [16];
char procDataA[6];
int decDataA = 0;

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
     
    decDataA = 0;
    
    // Initializing base value to 1, i.e 2^0 
    int base = 1; 
  
    int len = 32; 
    for (int i = len - 1; i >= 0; i--) 
    { 
        if (dataA[i] == 1) 
        {
            decDataA += base;
        }            
        base = base * 2; 
    } 
  
//    return dec_value; 
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
//        if()                  для чтения только канала А
//        {
//        for(int i = 0; i < 16; i++)
//        {
//            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
//            delay_us(2);
//            dataB[i] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
//            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
//            delay_us(2);
//        }
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        BinToDec();
        delay_us(8);
       
//        }
//        else
//        {
//            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
//            delay_us(8);
//        }
    }        
}

char* PLIS::GiveData()
{
    Int2String(decDataA,procDataA);
    return procDataA;
}

void PLIS::WriteCommand(char* command, char* argument)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
    delay_us(2);
    for(int i = 0; i < 4; i++)
    {
        if (command[i] == 1)
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
            delay_us(2);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
            delay_us(2);
        }
    }
    for(int i = 0; i < 6; i++)
    {
        if (argument[i] == 1)
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
            delay_us(2);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
            delay_us(2);
        }
    }
}
