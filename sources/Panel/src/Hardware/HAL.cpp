#include "HAL.h"
#include "stm32f4xx_hal.h"


static void SystemClock_Config(void);
static SPI_HandleTypeDef hSPI;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HAL::Init()
{
    HAL_Init();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    SystemClock_Config();
    
    SPI::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };


    __HAL_RCC_PLL_PLLM_CONFIG(16);

    __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSI);

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        ERROR_HANDLER();
    }
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLLSAI.PLLSAIN = 100;
    PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
    PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::SPI::Init()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_SPI4_CLK_ENABLE();

    hSPI.Instance               = SPI4;
    hSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hSPI.Init.Direction         = SPI_DIRECTION_1LINE;
    hSPI.Init.CLKPhase          = SPI_PHASE_1EDGE;
    hSPI.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    hSPI.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    hSPI.Init.CRCPolynomial     = 7;
    hSPI.Init.DataSize          = SPI_DATASIZE_8BIT;
    hSPI.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    hSPI.Init.NSS               = SPI_NSS_SOFT;
    hSPI.Init.TIMode            = SPI_TIMODE_DISABLE;
    hSPI.Init.Mode              = SPI_MODE_MASTER; 

    if(HAL_SPI_Init(&hSPI) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    GPIO_InitTypeDef  isGPIO;
        
    isGPIO.Pin       = GPIO_PIN_5 | GPIO_PIN_7;
    isGPIO.Mode      = GPIO_MODE_AF_PP;
    isGPIO.Pull      = GPIO_PULLUP;
    isGPIO.Speed     = GPIO_SPEED_FAST;
    isGPIO.Alternate = GPIO_AF5_SPI4;
    HAL_GPIO_Init(GPIOA, &isGPIO);

    
    isGPIO.Pin = GPIO_PIN_5;                // Управление DC дисплея
    isGPIO.Mode      = GPIO_MODE_OUTPUT_PP;
    isGPIO.Alternate = GPIO_MODE_AF_PP;
    HAL_GPIO_Init(GPIOC, &isGPIO);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::SPI::Send(uint8 *buffer, uint16 num)
{
    HAL_SPI_Transmit(&hSPI, (uint8 *)buffer, num, HAL_MAX_DELAY);
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
       /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ERROR_HANDLER(void)
{
    *((int*)((void*)0)) = 0;
}
