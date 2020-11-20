/*!*****************************************************************************
  @file		FontMid.h
  @author	VicKo
  @brief	Font Middle-size 24 (+ 6 down) x 15 (+ 1 right) pixel
  @date		2020.07
  @note		
  ******************************************************************************/

#ifndef FONTMID_H
#define FONTMID_H
#pragma once

#define		NDEBUG

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include "Display/Colors.h"
#include "defines.h"

#define		FONTMID_STARTINDEX		32	// number of rows with the same pixels
#define		FONTMID_SYMBWIDTH		18	// symbol width for monospace font
#define		FONTMID_SYMBHEIGHT		30	// symbol height
#define		FONTMID_SPACEWIDTH		10	// character 'space' width
#define		FONTMID_GAPWIDTH		4	// gap between characters
#define		DISPLAY_WIDTH		480		// LSD screen width



struct FontMid
{
    static void Write(char *text, int x, int y);
    static uint32_t WriteSymbol(uint8_t symbol, int x, int y);
};



#endif	// FONTMID_H
