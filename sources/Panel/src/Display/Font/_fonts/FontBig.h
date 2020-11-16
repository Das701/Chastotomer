/*!*****************************************************************************
  @file		BigFont.h
  @author	VicKo
  @date		2020.05
  @brief	Big Font 48 x 30(32): . , digits
  @note		
  ******************************************************************************/

#ifndef BIGFONT_H
#define BIGFONT_H
#pragma once

#define		NDEBUG

#include "Display/Colors.h"
#include "defines.h"

#define		BIGSTART_INDEX		64		// number of rows with the same pixels
#define		BIGCHAR_WIDTH		36		// big character width
#define		BIGSPACE_WIDTH		24		// character 'space' width
#define		BIGGAP_WIDTH		4		// gap between characters
#define		DISPLAY_WIDTH		480		// LSD screen width


struct FontBig
{
    static void Write(char *text, int x, int y, Color color);
    
    static uint WriteSymbol(uint8 symbol, int x, int y, Color color);
};

#endif
