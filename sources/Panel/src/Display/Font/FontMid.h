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
//#define		DISPLAY_WIDTH		320		// LSD screen width


/*  Functions  _________________________________________________________________*/

struct FontMid
{
    
static uint32_t MidSymbol_print(uint8_t symbol, int x, int y, Color color);
//static uint32_t MidSymbolPad_print(uint8_t symbol, volatile uint8_t *display, 
//						  uint32_t foreground, uint32_t background);
//
//static void MidStringMono_print(char *text, int x, int y, Color color);
static void MidStringProp_print(char *text, int x, int y, Color color);

//static void MidStringMonoPad_print(char *text, uint8_t *display, 
//							uint32_t foreground, uint32_t background);
//static void MidStringPropPad_print(char *text, uint8_t *display, 
//							uint32_t foreground, uint32_t background);
};



#endif	// FONTMID_H
