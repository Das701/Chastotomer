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

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "Display/Colors.h"
#include "defines.h"

#define		BIGSTART_INDEX		64		// number of rows with the same pixels
#define		BIGCHAR_WIDTH		40		// big character width
#define		BIGSPACE_WIDTH		24		// character 'space' width
#define		BIGGAP_WIDTH		8		// gap between characters
#define		DISPLAY_WIDTH		480		// LSD screen width


/*  Functions  _________________________________________________________________*/
struct FontBig
{
    static void BigString_print(char *text, int x, int y, Color color, bool monospace);
    static void BigStringPad_print(char *text, uint8 *disp, uint32_t palet, bool monospace);
    static uint32_t BigChar_print(uint8_t symb, int x, int y, Color color);
    static uint32_t BigCharPad_print(uint8_t symb, volatile uint8_t *disp, uint32_t palet);
};

#endif
