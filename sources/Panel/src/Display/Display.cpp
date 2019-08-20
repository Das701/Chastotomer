#include "defines.h"
#include "stm32f4xx_hal.h"
#include "Display.h"
#include "Hardware/HAL.h"
#include "Menu/Menu.h"
#include <cstring>
#include <cstdlib>


#define COLOR_BLACK 0x00
#define COLOR_WHITE 0xFF

#define CS_OPEN         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define CS_CLOSE        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)

#define SET_DC_COMMAND  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET)
#define SET_DC_DATA     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET)

#define SET_RES_LOW     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
#define SET_RES_HI      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)

// http://microsin.net/adminstuff/hardware/ssd1322-oled-controller.html

#define COM_ENABLE_GRAY_SCALE_TABLE 0x00    /* Enable Gray Scale Table */
#define COM_COL_ADDRESS             0x15    /* Set Column Address */
#define COM_WRITE_RAM  	            0x5C    /* Write RAM Command */
#define COM_ROW_ADDRESS             0x75    /* Set Row Address */
#define COM_REMAP_AND_DUAL          0xA0    /* Set Re-map and Dual COM Line mode */
#define COM_DISPLAY_START_LINE      0xA1    /* Set Display Start Line */
#define COM_DISPLAY_OFFSET          0xA2    /* Set Display Offset */
#define COM_DISPLAY_MODE_GS15       0xA5    /* ƒисплей переводитс€ в режим, в котором все пиксели имеют максимальную €ркость GS15 */
#define COM_DISPLAY_MODE_NORMAL     0xA6    /* Ќормальна€ работа (состо€ние после сброса). */
#define COM_EXIT_PARTIAL_DISPLAY    0xA9    /* Exit Partial Display */
#define COM_FUNC_SELECT             0xAB    /* Function Selection */    
                                            // A[0] = 0b, выбрать внешнее питание дл€ VDD (внутренний регул€тор запрещен).
                                            // A[0] = 1b, разрешить работу внутреннего регул€тора дл€ VDD(состо€ние по умолчанию после сброса).
#define COM_SLEEP_MODE_ON           0xAE    /* Set Sleep mode ON */
#define COM_SLEEP_MODE_OFF          0xAF    /* Set Sleep mode OFF */
#define COM_PHASE_LENGTH            0xB1    /* Set Phase Length */
#define COM_FRONT_CLOCK_DIV         0xB3    /* Set Front Clock Divider / Oscillator Frequency */
#define COM_DISPLAY_ENHANCEMENT_A   0xB4    /* Display Enhancement A */
                                            // A[1:0] = 00b: разрешить внешнее VSL
                                            // A[1:0] = 10b : внутреннее VSL(после сброса)
                                            // B[7:3] = 11111b : улучшает качество GS диспле€
                                            // B[7:3] = 10110b : обычный режим(после сброса)
#define COM_GPIO                    0xB5    /* Set GPIO */
#define COM_SECOND_PRECHARGE_T      0xB6    /* Set Second Precharge Period */
                                            // A[3:0] устанавливает длительность второго периода предзар€да. 
#define COM_GRAY_SCALE_TABLE        0xB8    /* Set Gray Scale Table */
#define COM_DEFAULT_GRAY_SCALE_TABLE 0xB9   /* Select Default Linear Gray Scale table */
#define COM_PRECHARGE_VOLTAGE       0xBB    /* Set Pre-charge voltage */
#define COM_VCOMH                   0xBE    /* Set VCOMH */
#define COM_CONTRAST                0xC1    /* Set Contrast Current */
#define COM_MASTER_CONTRAST         0xC7    /* Master Contrast Current Control */
#define COM_MUX_RATIO               0xCA    /* Set MUX Ratio */
#define COM_DISPLAY_ENHANCEMENT_B   0xD1    /* Display Enhancement B */
                                            // A[5:4] = 00b: зарезервированное состо€ние
                                            // A[5:4] = 10b : нормальное состо€ние(после сброса).
#define COM_LOCK                    0xFD    /* Set Command Lock */
                                            // A[2] определ€ет защиту интерфейса от доступа со стороны микроконтроллера(значение после сброса 12h).


/// Ѕуфер с изображение
static uint8 buf[256][64];

static void Delay(uint ms);

static void SendData(uint8 data);
static void SendCommand(uint8 command);
static void SendCommand(uint8 command, uint8 data);
static void SendCommand(uint8 command, uint8 data0, uint8 data1);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void Display::Init()
//void Display::Init()
//{
//    SET_RES_HI;
//    Delay(500);
//    SET_RES_LOW;
//    SendCommand(COM_LOCK, 0x12);
//    //SendCommand(COM_DISPLAY_ENHANCEMENT_A, 0xA0, 0xFD); Ёто не надо. ѕо умолчанию внутренее VSL и обычный режим
//    SendCommand(COM_DISPLAY_MODE_GS15);
//}

void Display::Init()
{
    // U8G_ESC_DLY(10),              /* delay 10 ms */
    Delay(10);
    
    // U8G_ESC_CS(0),                 /* disable chip */
    CS_CLOSE;

    // U8G_ESC_ADR(0),               /* instruction mode */
    SET_DC_COMMAND;

    // U8G_ESC_RST(1),               /* do reset low pulse with (1*16)+2 milliseconds */
    SET_RES_LOW;
    Delay(100);
    SET_RES_HI;
    Delay(300);

    //U8G_ESC_CS(1),                /* enable chip */
    
    //U8G_ESC_DLY(100),             /* delay 100 ms */
    //U8G_ESC_DLY(100),             /* delay 100 ms */
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0fd,					/* lock command */
    //U8G_ESC_ADR(1),               	/* data mode */
    //0x012,					/* unlock */
    SendCommand(COM_LOCK, 0x12);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0ae,                               	 /* display off, sleep mode */
    SendCommand(COM_SLEEP_MODE_ON);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0b3,
    //U8G_ESC_ADR(1),               	/* data mode */
    //0x091,                    /* set display clock divide ratio/oscillator frequency (set clock as 80 frames/sec) */
    SendCommand(COM_FRONT_CLOCK_DIV, 0x91);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0ca, 					/* multiplex ratio */
    //U8G_ESC_ADR(1),               	/* data mode */
    //0x03f,                    		/* 1/64 Duty (0x0F~0x3F) */
    SendCommand(COM_MUX_RATIO, 0x3f);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0a2,
    //U8G_ESC_ADR(1),               	/* data mode */
    //0x000,                     		/* display offset, shift mapping ram counter */
    SendCommand(COM_DISPLAY_OFFSET, 0x00);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0a1,
    //U8G_ESC_ADR(1),               	/* data mode */
    //0x000,                     		/* display start line */
    SendCommand(COM_DISPLAY_START_LINE, 0x00);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0a0, 					/* Set Re-Map / Dual COM Line Mode */
    //U8G_ESC_ADR(1),               	/* data mode */
    //0x014, 					/* was 0x014 */
    //0x011, 					/* was 0x011 */
    SendCommand(COM_REMAP_AND_DUAL, 0x14, 0x11);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0ab,
    //U8G_ESC_ADR(1),               	/* data mode */
    //0x001,                     		/* Enable Internal VDD Regulator */
    SendCommand(COM_FUNC_SELECT, 0x01);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0b4, 					/* Display Enhancement A */
    //U8G_ESC_ADR(1),               	/* data mode */
    //0x0a0,
    //0x005 | 0x0fd,
    SendCommand(COM_DISPLAY_ENHANCEMENT_A, 0xa2, 0x05 | 0xfd);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0c1,					/* contrast */
    //U8G_ESC_ADR(1),               	/* data mode */
    //0x09f,
    SendCommand(COM_CONTRAST, 0x9f);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0c7,					/* Set Scale Factor of Segment Output Current Control */
    //U8G_ESC_ADR(1),               	/* data mode */
    //0x00f,
    SendCommand(COM_MASTER_CONTRAST, 0x0f);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0b9,                               	 /* linear gray scale */
    SendCommand(COM_DEFAULT_GRAY_SCALE_TABLE);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0b1,					/* Phase 1 (Reset) & Phase 2 (Pre-Charge) Period Adjustment */
    //U8G_ESC_ADR(1),               	/* data mode */
    //0x0e2,
    SendCommand(COM_PHASE_LENGTH, 0xe2);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0d1, 					/* Display Enhancement B */
    //U8G_ESC_ADR(1),               	/* data mode */
    //0x082 | 0x020,
    //0x020,
    SendCommand(COM_DISPLAY_ENHANCEMENT_B, 0x82 | 0x20, 0x20);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0bb,					/* precharge  voltage */
    //U8G_ESC_ADR(1),               	/* data mode */
    //0x01f,
    SendCommand(COM_PRECHARGE_VOLTAGE, 0x1f);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0b6,					/* precharge period */
    //U8G_ESC_ADR(1),               	/* data mode */
    //0x008,
    SendCommand(COM_SECOND_PRECHARGE_T, 0x08);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0be,					/* vcomh */
    //U8G_ESC_ADR(1),               	/* data mode */
    //0x007,
    SendCommand(COM_VCOMH, 0x07);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0a6,                               	 /* normal display */
    SendCommand(COM_DISPLAY_MODE_NORMAL);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0a9,                               	 /* exit partial display */
    SendCommand(COM_EXIT_PARTIAL_DISPLAY);
    
    //U8G_ESC_ADR(0),               	/* instruction mode */
    //0x0af,                               	 /* display on */
    SendCommand(COM_SLEEP_MODE_OFF);

    SendCommand(COM_DISPLAY_MODE_GS15);
    
    //U8G_ESC_CS(0),             /* disable chip */
    //U8G_ESC_END                /* end of sequence */
}

void Display::InitBlack()
{
    Delay(10);

    CS_CLOSE;

    SET_DC_COMMAND;

    SET_RES_LOW;
    Delay(100);
    SET_RES_HI;
    Delay(200);
    
    CS_OPEN;

    SendCommand(COM_LOCK, 0x12);
    SendCommand(COM_SLEEP_MODE_ON);
    SendCommand(COM_FRONT_CLOCK_DIV, 0x91);
    SendCommand(COM_MUX_RATIO, 0x3F);
    SendCommand(COM_DISPLAY_OFFSET, 0x00);
    SendCommand(COM_DISPLAY_START_LINE, 0x00);
    SendCommand(COM_REMAP_AND_DUAL, 0x14, 0x11);
    SendCommand(COM_FUNC_SELECT, 0x01);
    SendCommand(COM_DISPLAY_ENHANCEMENT_A, 0xA2, 0x05 | 0xFD);
    SendCommand(COM_CONTRAST, 0x9f);
    SendCommand(COM_MASTER_CONTRAST, 0x0F);
    SendCommand(COM_DEFAULT_GRAY_SCALE_TABLE);
    SendCommand(COM_PHASE_LENGTH, 0xE2);
    SendCommand(COM_DISPLAY_ENHANCEMENT_B, 0x82 | 0x20, 0x20);
    SendCommand(COM_PRECHARGE_VOLTAGE, 0x1F);
    SendCommand(COM_SECOND_PRECHARGE_T, 0x08);
    SendCommand(COM_VCOMH, 0x00);
    SendCommand(COM_DISPLAY_MODE_NORMAL);
    SendCommand(COM_DISPLAY_MODE_GS15);
    SendCommand(COM_EXIT_PARTIAL_DISPLAY);
    SendCommand(COM_SLEEP_MODE_OFF);
}

static void Init2()
{
    CS_CLOSE;

    Delay(500);
    SET_RES_HI;
    Delay(500);
    SET_RES_LOW;
    Delay(500);
    SET_RES_HI;
    Delay(500);
    SendCommand(COM_LOCK, 0x12);                        // unlock
    SendCommand(COM_FRONT_CLOCK_DIV, 0x91);             // to 135 fps
    SendCommand(COM_MUX_RATIO, 0x3F);
    SendCommand(COM_DISPLAY_OFFSET, 0x00);
    SendCommand(COM_DISPLAY_START_LINE, 0x00);
    SendCommand(COM_REMAP_AND_DUAL, 0x14, 0x11);
    SendCommand(COM_GPIO, 0x00);                        //disable IO intput
    SendCommand(COM_FUNC_SELECT, 0x00);
    SendCommand(COM_DISPLAY_ENHANCEMENT_A, 0xA0, 0xFD); //enable VSL extern
    SendCommand(COM_CONTRAST, 0xFF);
    SendCommand(COM_MASTER_CONTRAST, 0x0F);
    SendCommand(COM_GRAY_SCALE_TABLE);
    SendData(0);
    SendData(0);
    SendData(0);
    SendData(0);
    SendData(0);
    SendData(0);
    SendData(0);
    SendData(0);
    SendData(180);
    SendData(180);
    SendData(180);
    SendData(180);
    SendData(180);
    SendData(180);
    SendData(180);
    SendData(180);
    SendCommand(COM_ENABLE_GRAY_SCALE_TABLE);
    SendCommand(COM_PHASE_LENGTH, 0xE8);
    SendCommand(COM_DISPLAY_ENHANCEMENT_B, 0x82, 0x20);
    SendCommand(COM_PRECHARGE_VOLTAGE, 0x1F);
    SendCommand(COM_SECOND_PRECHARGE_T, 0x08);
    SendCommand(COM_VCOMH, 0x07);
    SendCommand(COM_EXIT_PARTIAL_DISPLAY);
    SendCommand(COM_DISPLAY_MODE_NORMAL);
    Delay(200);                 //stabilize VDD
    SendCommand(COM_SLEEP_MODE_OFF);
    Delay(200);                 //stabilize VDD

    SendCommand(COM_DISPLAY_MODE_GS15);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendCommand(uint8 command)
{
    Delay(1);
    CS_OPEN;
    Delay(1);
    SET_DC_COMMAND;
    Delay(1);
    HAL::SPI::Send(&command, 1);
    Delay(1);
    CS_CLOSE;
    Delay(1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendData(uint8 data)
{
    Delay(1);
    CS_OPEN;
    Delay(1);
    SET_DC_DATA;
    Delay(1);
    HAL::SPI::Send(&data, 1);
    Delay(1);
    CS_CLOSE;
    Delay(1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendData(uint8 *data, uint16 num)
{
    CS_OPEN;
    SET_DC_DATA;
    HAL::SPI::Send(data, num);
    CS_CLOSE;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendCommand(uint8 command, uint8 data)
{
    SendCommand(command);
    SendData(data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendCommand(uint8 command, uint8 data0, uint8 data1)
{
    SendCommand(command);
    SendData(data0);
    SendData(data1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawPoint(int x, int y, uint8 color = COLOR_WHITE)
{
    if (x < 256 && x >= 0 && y < 64 && y  >= 0)
    {     
        buf[x][y] = color;      
    }        
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawHLine(int x, int y, int length, uint8 color)
{
    for(int i = 0; i < length; i++)
    {
        DrawPoint(x, y, color);
        x++;        
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawVLine(int x, int y, int length, uint8 color)
{
    for(int i = 0; i < length; i++)
    {
        DrawPoint(x, y, color);
        y++;        
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawRectangle(int x, int y, int width, int height, uint8 color)
{
    DrawHLine(x, y, height, color);
    DrawVLine(x, y, width, color);
    DrawHLine(x, y + width, height, color);
    DrawVLine(x + height, y, width, color);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FillRectangle(int x, int y, int width, int height, uint8 color)
{
    for(int y0 = y; y0 < y + height; y0++)
    {
        DrawHLine(x, y0, width, color);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BeginScreen()
{
    FillRectangle(0, 0, 64, 256, COLOR_BLACK);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Delay(uint ms)
{
    uint timestamp = HAL_GetTick();
    while(HAL_GetTick() - timestamp < ms)
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EndScreen()
{
    SendCommand(COM_COL_ADDRESS, 0x00, 0xFF);

    SendCommand(COM_ROW_ADDRESS, 0x00, 0xFF);

    SendCommand(COM_WRITE_RAM);

    Delay(50);

    uint8 buffer1[128];

    for(uint y = 0; y < 64; y++)
    {
        uint8 totcount = 0;
        for(uint x = 0; x < 256; x += 2) 
        {
            uint8 low = (uint8)(buf[x][y] & 0x0F);
            uint8 high = (uint8)(buf[x + 1][y] & 0xF0);
            buffer1[totcount] = (uint8)(low | high);
            totcount++;
        }

        SendData(buffer1, 128);
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Update()
{
    DrawRectangle(std::rand() % 256, std::rand() % 64, std::rand() % 256, std::rand() % 256, (uint8)((std::rand() % 2) ? COLOR_WHITE : COLOR_BLACK));
   
    EndScreen();
}
