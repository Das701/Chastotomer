/*!*****************************************************************************
  @file   BigFont.h
  @author VicKo
  @date   2020.05
  @brief  Big Font 48 x 30(32): . , digits
  @note        
  ******************************************************************************/

#ifndef BIGFONT_H
#define BIGFONT_H
#pragma once

#define     NDEBUG

#include "Display/Colors.h"
#include "defines.h"

#define     BIGSTART_INDEX      64      // number of rows with the same pixels
#define     BIGCHAR_WIDTH       36      // big character width
#define     BIGSPACE_WIDTH      24      // character 'space' width
#define     BIGGAP_WIDTH        4       // gap between characters
#define     DISPLAY_WIDTH       480     // LSD screen width


struct FontBig
{
    // Если (mapping == false) - не рисовать символ, а только возвращать значение
    static int Write(char *text, int x, int y, bool mapping = true, bool splitThrees = false);

    // Если (splitTress == true) - разделять тройки символов, начиная с последнего знака
    static void WriteAboutRight(char *text, int right, int y, bool splitThress = false);
    
    // Если (mapping == false) - не рисовать символ, а только возвращать значение
    static uint WriteSymbol(uint8 symbol, int x, int y, bool mapping = true);
};

#endif
