#include <stm32f4xx_hal.h>



void HAL_GPIO_WritePin(GPIO_TypeDef* /*GPIOx*/, uint16_t /*GPIO_Pin*/, GPIO_PinState /*PinState*/)
{

}


void  HAL_GPIO_Init(GPIO_TypeDef  * /*GPIOx*/, GPIO_InitTypeDef * /*GPIO_Init*/)
{

}

void  HAL_GPIO_DeInit(GPIO_TypeDef  * /*GPIOx*/, uint32_t /*GPIO_Pin*/)
{

}


GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* /*GPIOx*/, uint16_t /*GPIO_Pin*/)
{
    return GPIO_PIN_RESET;
}


void HAL_GPIO_EXTI_IRQHandler(uint16_t /*GPIO_Pin*/)
{

}
