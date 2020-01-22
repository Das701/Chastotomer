#include "defines.h"
#include "Hardware/HAL.h"
#include <stm32f4xx_hal.h>


#define PIN_BL_E  GPIO_PIN_0
#define PORT_BL_E GPIOB
#define BL_E      PORT_BL_E, PIN_BL_E

#define PIN_D_C  GPIO_PIN_1
#define PORT_D_C GPIOB
#define D_C      PORT_D_C, PIN_D_C

#define PIN_WR  GPIO_PIN_2
#define PORT_WR GPIOB
#define WR      PORT_WR, PIN_WR

#define PIN_RD  GPIO_PIN_10
#define PORT_RD GPIOB
#define RD      PORT_RD, PIN_RD

#define PIN_CS  GPIO_PIN_13
#define PORT_CS GPIOC
#define CS      PORT_CS, PIN_CS

#define PIN_DIP_ON  GPIO_PIN_15
#define PORT_DIP_ON GPIOC
#define DIP_ON      PORT_DIP_ON, PIN_DIP_ON

#define PIN_D0  GPIO_PIN_0
#define PORT_D0 GPIOA
#define D0      PORT_D0, PIN_D0

#define PIN_D1  GPIO_PIN_1
#define PORT_D1 GPIOA
#define D1      PORT_D1, PIN_D1

#define PIN_D2  GPIO_PIN_2
#define PORT_D2 GPIOA
#define D2      PORT_D2, PIN_D2

#define PIN_D3  GPIO_PIN_3
#define PORT_D3 GPIOA
#define D3      PORT_D3, PIN_D3

#define PIN_D4  GPIO_PIN_4
#define PORT_D4 GPIOA
#define D4      PORT_D4, PIN_D4

#define PIN_D5  GPIO_PIN_5
#define PORT_D5 GPIOA
#define D5      PORT_D5, PIN_D5

#define PIN_D6  GPIO_PIN_6
#define PORT_D6 GPIOA
#define D6      PORT_D6, PIN_D6

#define PIN_D7  GPIO_PIN_7
#define PORT_D7 GPIOA
#define D7      PORT_D7, PIN_D7


struct StructPIO
{
    StructPIO(GPIO_TypeDef *g, uint16 p) : gpio(g), pin(p) { }

    void Set()
    {
        HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);
    }

    void Reset()
    {
        HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
    }

    void Write(int state)
    {
        HAL_GPIO_WritePin(gpio, pin, (GPIO_PinState)state);
    }

private:
    GPIO_TypeDef *gpio;
    uint16 pin;
};


StructPIO pinBL_E(BL_E);
StructPIO pinD_C(D_C);
StructPIO pinWR(WR);
StructPIO pinRD(RD);
StructPIO pinCS(CS);
StructPIO pinDIP_ON(DIP_ON);

StructPIO pinD0(D0);
StructPIO pinD1(D1);
StructPIO pinD2(D2);
StructPIO pinD3(D3);
StructPIO pinD4(D4);
StructPIO pinD5(D5);
StructPIO pinD6(D6);
StructPIO pinD7(D7);


void HAL_FSMC::Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef is =
    {
        0,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLDOWN
    };

    is.Pin = PIN_D0 | PIN_D1 | PIN_D2 | PIN_D3 | PIN_D4 | PIN_D5 | PIN_D6 | PIN_D7;

    HAL_GPIO_Init(GPIOA, &is);

    is.Pin =
        GPIO_PIN_0 |   // D8
        GPIO_PIN_1 |   // D9
        GPIO_PIN_2 |   // D10
        GPIO_PIN_3 |   // D11
        GPIO_PIN_4 |   // D12
        GPIO_PIN_5 |   // D13
        GPIO_PIN_6 |   // D14
        GPIO_PIN_7 |   // D15
        PIN_CS     |   // CS
        PIN_DIP_ON;    // DIP_ON

    HAL_GPIO_Init(GPIOC, &is);

    is.Pin = PIN_BL_E  | PIN_D_C | PIN_WR | PIN_RD;
    HAL_GPIO_Init(GPIOB, &is);

    pinCS.Set();
    pinRD.Set();
    pinWR.Set();
    pinDIP_ON.Set();
    pinBL_E.Set();
}


void HAL_FSMC::WriteCommand(uint8 command)
{
    HAL_Delay(1);

    pinCS.Reset();
    pinD_C.Reset();
    pinWR.Reset();

    SetDataBus(command);

    pinWR.Set();
    pinD_C.Set();
    pinCS.Set();
}


void HAL_FSMC::WriteData(uint8 data)
{
    HAL_Delay(1);

    pinCS.Reset();
    pinD_C.Set();
    pinWR.Reset();

    SetDataBus(data);

    pinWR.Set();
    pinD_C.Reset();
    pinCS.Set();

}


void HAL_FSMC::SetDataBus(uint8 data)
{
    static StructPIO * const pins[8] = { &pinD0, &pinD1, &pinD2, &pinD3, &pinD4, &pinD5, &pinD6, &pinD7 };

    for(int i = 0; i < 8; i++)
    {
        pins[i]->Write(data & 0x01);
        data >>= 1;
    }
}
