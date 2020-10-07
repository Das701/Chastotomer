/*!*****************************************************************************
  @file		BigFont.c
  @author	VicKo
  @brief	
  @date		2020
  @note		STM401RET6, SSD1936, WF43QTIBEDBND
  ******************************************************************************/
/*
  Character code: (num,) inx, (num,) inx ... 0
	inx - index in CharRow array (BIGSTART_INDEX + ...) to position in array)
	num - number of rows (1, 2 ... BIGSTART_INDEX-1) with the same index in array
	0 	- end of character code
	assert:		http://www.keil.com/support/man/docs/armlib/armlib_chr1358938924063.htm
  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
//#include "Application.h"
#include "Display/Font/BigFont1.h"
#include "Display/Primitives.h"
#include "Display/Display.h"
using namespace Display::Primitives;

/*!*****************************************************************************
  @brief	Character rows array (32-bit wide)
  @note		Comments:	index, bitmap, spase-mark-space... (chars...)
  */
static const uint32_t	BigCharRow[] = {
	0x00000000,		// 0	00000000'00000000'00000000'00000000		32			(space)
	0x0000003e,		// 1	00000000'00000000'00000000'00111110		26-5-1		(2359)
	0x0000007c,		// 2	00000000'00000000'00000000'01111100		25-5-2		(2359)
	0x0000007e,		// 3	00000000'00000000'00000000'01111110		25-6-1		(23579)
	0x000000f8,		// 4	00000000'00000000'00000000'11111000		24-5-3		(3)
	0x000000fc,		// 5	00000000'00000000'00000000'11111100		24-6-2		(23679)
	0x000001f8,		// 6	00000000'00000000'00000001'11111000		23-6-3		(2367)
	0x000003e0,		// 7	00000000'00000000'00000011'11100000		22-5-5		(4)
	0x000003f0,		// 8	00000000'00000000'00000011'11110000		22-6-4		(37)
	0x000003f8,		// 9	00000000'00000000'00000011'11111000		22-7-3		(23)
	0x000007e0,		// 10	00000000'00000000'00000111'11100000		21-6-5		(7)
	0x000007f0,		// 11	00000000'00000000'00000111'11110000		21-7-4		(23)
	0x00000fc0,		// 12	00000000'00000000'00001111'11000000		20-6-6		(7)
	0x00000fe0,		// 13	00000000'00000000'00001111'11100000		20-7-5		(2)
	0x00000ff0,		// 14	00000000'00000000'00001111'11110000		20-8-4		(3)
	0x00001f80,		// 15	00000000'00000000'00011111'10000000		19-6-7		(7)
	0x00001fc0,		// 16	00000000'00000000'00011111'11000000		19-7-6		(2)
	0x00001fe0,		// 17	00000000'00000000'00011111'11100000		19-8-5		(3)
	0x00003f00,		// 18	00000000'00000000'00111111'00000000		18-6-8		(7)
	0x00003f80,		// 19	00000000'00000000'00111111'10000000		18-7-7		(2)
	0x00007e00,		// 20	00000000'00000000'01111110'00000000		17-6-9		(7)
	0x00007f00,		// 21	00000000'00000000'01111111'00000000		17-7-8		(2)
	0x0000fc00,		// 22	00000000'00000000'11111100'00000000		16-6-10		(7)
	0x0000fe00,		// 23	00000000'00000000'11111110'00000000		16-7-9		(2)
	0x0001f800,		// 24	00000000'00000001'11111000'00000000		15-6-11		(7)
	0x0001fc00,		// 25	00000000'00000001'11111100'00000000		15-7-10		(2)
	0x0003e000,		// 26	00000000'00000011'11100000'00000000		14-5-13		(1)
	0x0003f000,		// 27	00000000'00000011'11110000'00000000		14-6-12		(7)
	0x0003f800,		// 28	00000000'00000011'11111000'00000000		14-7-11		(2)
	0x0003ff00,		// 29	00000000'00000011'11111111'00000000		14-10-8		(3)
	0x0003ff80,		// 30	00000000'00000011'11111111'10000000		14-11-7		(3)
	0x0003ffc0,		// 31	00000000'00000011'11111111'11000000		14-12-6		(3)
	0x0003ffe0,		// 32	00000000'00000011'11111111'11100000		14-13-5		(3)
	0x0007e000,		// 33	00000000'00000111'11100000'00000000		13-6-13		(17)
	0x0007f000,		// 34	00000000'00000111'11110000'00000000		13-7-12		(2)
	0x000f8000,		// 35	00000000'00001111'10000000'00000000		12-5-15		(7)
	0x000fc000,		// 36	00000000'00001111'11000000'00000000		12-6-14		(7)
	0x000fe000,		// 37	00000000'00001111'11100000'00000000		12-7-13		(12)
	0x000ff000,		// 38	00000000'00001111'11110000'00000000		12-8-12		(38)
	0x001f0000,		// 39	00000000'00011111'00000000'00000000		11-5-16		(7)
	0x001f8000,		// 40	00000000'00011111'10000000'00000000		11-6-15		(7)
	0x001fc000,		// 41	00000000'00011111'11000000'00000000		11-7-14		(2)
	0x001fe000,		// 42	00000000'00011111'11100000'00000000		11-8-13		(1)
	0x001ff800,		// 43	00000000'00011111'11111000'00000000		11-10-11	(0235689)
	0x001ff83e,		// 44	00000000'00011111'11111000'00111110		11-10-5-5-1	(9)
	0x003e0000,		// 45	00000000'00111110'00000000'00000000		10-5-17		(7)
	0x003e03e0,		// 46	00000000'00111110'00000011'11100000		10-5-7-5-5	(4)
	0x003f0000,		// 47	00000000'00111111'00000000'00000000		10-6-16		(7)
	0x003f8000,		// 48	00000000'00111111'10000000'00000000		10-7-15		(2)
	0x003fe000,		// 49	00000000'00111111'11100000'00000000		10-9-13		(1)
	0x003ffc00,		// 50	00000000'00111111'11111100'00000000		10-12-10	(38)
	0x007c0000,		// 51	00000000'01111100'00000000'00000000		9-5-18		(7)
	0x007c03e0,		// 52	00000000'01111100'00000011'11100000		9-5-8-5-5	(4)
	0x007e0000,		// 53	00000000'01111110'00000000'00000000		9-6-17		(7)
	0x007e03e0,		// 54	00000000'01111110'00000011'11100000		9-6-7-5-5	(4)
	0x007f0000,		// 55	00000000'01111111'00000000'00000000		9-7-16		(2)
	0x007fe000,		// 56	00000000'01111111'11100000'00000000		9-10-13		(1)
	0x007FFE00,		// 57	00000000'01111111'11111110'00000000		9-14-9		(0235689)
	0x007ffe3e,		// 58	00000000'01111111'11111110'00111110		9-14-3-5-1	(9)
	0x007fff00,		// 59	00000000'01111111'11111111'00000000		9-15-8		(1)
	0x00f803e0,		// 60	00000000'11111000'00000011'11100000		8-5-9-5-5	(4)
	0x00fc0000,		// 61	00000000'11111100'00000000'00000000		8-6-18		( )
	0x00fc03e0,		// 62	00000000'11111100'00000011'11100000		8-6-8-5-5	(4)
	0x00fe0000,		// 63	00000000'11111110'00000000'00000000		8-7-17		(2)
	0x00ffe000,		// 64	00000000'11111111'11100000'00000000		8-11-13		(1)
	0x00ffff00,		// 65	00000000'11111111'11111111'00000000		8-16-8		(38)
	0x01f003e0,		// 66	00000001'11110000'00000011'11100000		7-5-10-5-5	(4)
	0x01f803e0,		// 67	00000001'11111000'00000011'11100000		7-6-9-5-5	(4)
	0x01fc0000,		// 68	00000001'11111100'00000000'00000000		7-7-18		(2)
	0x01ffe000,		// 69	00000001'11111111'11100000'00000000		7-12-13		(1)
	0x01ffff80,		// 70	00000001'11111111'11111111'10000000		7-18-7		(0235689)
	0x01ffffbe,		// 71	00000001'11111111'11111111'10111110		7-18-1-5-1	(9)
	0x03c3e000,		// 72	00000011'11000011'11100000'00000000		6-4-4-5-13	(1)
	0x03e003e0,		// 73	00000011'11100000'00000011'11100000		6-5-11-5-5	(4)
	0x03e3e000,		// 74	00000011'11100011'11100000'00000000		6-5-3-5-13	(1)
	0x03f003e0,		// 75	00000011'11110000'00000011'11100000		6-6-10-5-5	(4)
	0x03f3e000,		// 76	00000011'11110011'11100000'00000000		6-6-2-5-13	(1)
	0x03f80000,		// 77	00000011'11111000'00000000'00000000		6-7-19		(2)
	0x03fbe000,		// 78	00000011'11111011'11100000'00000000		6-7-1-5-13	(1)
	0x03ffffc0,		// 79	00000011'11111111'11111111'11000000		6-20-6		(0235689)
	0x03fffffe,		// 80	00000011'11111111'11111111'11111110		6-25-1		(9)
	0x07c003e0,		// 81	00000111'11000000'00000011'11100000		5-5-12-5-5	(4)
	0x07e003e0,		// 82	00000111'11100000'00000011'11100000		5-6-11-5-5	(4)
	0x07f00000,		// 83	00000111'11110000'00000000'00000000		5-7-20		(2)
	0x07f81fe0,		// 84	00000111'11111000'00011111'11100000		5-8-6-8-5	(38)
	0x07ffffe0,		// 85	00000111'11111111'11111111'11100000		5-22-5		(0235689)
	0x07fffffe,		// 86	00000111'11111111'11111111'11111110		5-26-1		(9)
	0x1e000000,		// 87	00001111'00000000'00000000'00000000		4-4-24		(,)
	0x0f8001f8,		// 88	00001111'10000000'00000001'11111000		4-5-14-6-3	(9)
	0x0F8003e0,		// 89	00001111'10000000'00000011'11100000		4-5-13-5-5	(4)
	0x0fc003e0,		// 90	00001111'11000000'00000011'11100000		4-6-12-5-5	(4)
	0x0fc003f0,		// 91	00001111'11000000'00000011'11110000		4-6-12-6-4	(38)
	0x0fc003f8,		// 92	00001111'11000000'00000011'11111000		4-6-12-7-3	(9)
	0x0fe00000,		// 93	00001111'11100000'00000000'00000000		4-7-21		(2)
	0x0fe007f0,		// 94	00001111'11100000'00000111'11110000		4-7-10-7-4	(38)
	0x0ff00ff0,		// 95	00001111'11110000'00001111'11110000		4-8-8-8-4	(0235689)
	0x0ff00ffe,		// 96	00001111'11110000'00001111'11111110		4-8-8-11-1	(9)
	0x1e000000,		// 97	00011110'00000000'00000000'00000000		3-4-25		(.,)
	0x1F000000,		// 98	00011111'00000000'00000000'00000000		3-5-24		(,)
	0x1f0000f8,		// 99	00011111'00000000'00000000'11111000		3-5-16-5-3	(38)
	0x1f0000fc,		// 100	00011111'00000000'00000000'11111100		3-5-16-6-2	(3)
	0x1f0003e0,		// 101	00011111'00000000'00000011'11100000		3-5-14-5-5	(4)
	0x1f800000,		// 102	00011111'10000000'00000000'00000000		3-6-23		(2)
	0x1F8001F0,		// 103	00011111'10000000'00000001'11110000		3-6-14-5-4	(5)
	0x1f8001f8,		// 104	00011111'10000000'00000001'11111000		3-6-14-6-3	(0235689)
	0x1f8001fe,		// 105	00011111'10000000'00000001'11111110		3-6-14-8-1	(9)
	0x1f8003e0,		// 106	00011111'10000000'00000011'11100000		3-6-13-5-5	(4)
	0x1fc00000,		// 107	00011111'11000000'00000000'00000000		3-7-22		(2)
	0x1fc003f0,		// 108	00011111'11000000'00000011'11110000		3-7-12-6-4	(6)
	0x1fc003f8,		// 109	00011111'11000000'00000011'11111000		3-7-12-7-3	(0235689)
	0x1fc003fe,		// 110	00011111'11000000'00000011'11111110		3-7-12-9-1	(9)
	0x38000000,		// 111	00111000'00000000'00000000'00000000		2-3-27		(,)
	0x3c000000,		// 112	00111100'00000000'00000000'00000000		2-4-26		(.,)
	0x3e000000,		// 112	00111110'00000000'00000000'00000000		2-5-25		(6)
	0x3e00007c,		// 113	00111110'00000000'00000000'01111100		2-5-18-5-2	(0235689)
	0x3e00007e,		// 114	00111110'00000000'00000000'01111110		2-5-18-6-1	(9)
	0x3e0003e0,		// 115	00111110'00000000'00000011'11100000		2-5-15-5-5	(4)
	0x3f000000,		// 116	00111111'00000000'00000000'00000000		2-6-24		(.,26)
	0x3f0000fc,		// 117	00111111'00000000'00000000'11111100		2-6-16-6-2	(0235689)
	0x3f0000fe,		// 118	00111111'00000000'00000000'11111110		2-6-16-7-1	(9)
	0x3f0003e0,		// 119	00111111'00000000'00000011'11100000		2-6-14-5-5	(4)
	0x7c000000,		// 120	01111100'00000000'00000000'00000000		1-5-26		(56)
	0x7c00003e,		// 121	01111100'00000000'00000000'00111110		1-5-20-5-1	(0235689)
	0x7c1ff800,		// 122	01111100'00011111'11111000'00000000		1-5-5-10-11	(56)
	0x7c7ffe00,		// 123	01111100'01111111'11111110'00000000		1-5-3-14-9	(56)
	0x7dffff80,		// 124	01111101'11111111'11111111'10000000		1-5-1-18-7	(56)
	0x7e000000,		// 125	01111110'00000000'00000000'00000000		1-6-25		(26)
	0x7e00007c,		// 126	01111110'00000000'00000000'01111100		1-6-18-5-2	(6)
	0x7e00007e,		// 127	01111110'00000000'00000000'01111110		1-6-18-6-1	(0235689)
	0x7e0000fc,		// 128	01111110'00000000'00000000'11111100		1-6-17-6-2	(5)
	0x7e0003e0,		// 129	01111110'00000000'00000011'11100000		1-6-15-5-5	(4)
	0x7f0000fc,		// 130	01111111'00000000'00000000'11111100		1-7-16-6-2	(6)
	0x7f0001f8,		// 131	01111111'00000000'00000001'11111000		1-7-15-6-3	(5)
	0x7f8001f8,		// 132	01111111'10000000'00000001'11111000		1-8-14-6-3	(56)
	0x7fc003f8,		// 133	01111111'11000000'00000011'11111000		1-9-12-7-3	(56)
	0x7ff00ff0,		// 134	01111111'11110000'00001111'11110000		1-11-8-8-4	(56)
	0x7fffffc0,		// 135	01111111'11111111'11111111'11000000		1-25-6		(56)
	0x7fffffe0,		// 136	01111111'11111111'11111111'11100000		1-26-5		(56)
	0x7ffffff8,		// 137	01111111'11111111'11111111'11111000		1-28-3		(5)
	0x7ffffffe,		// 138	01111111'11111111'11111111'11111110		1-30-1		(247)
};

/*!*****************************************************************************
  @brief	Character rows indexes
  @note		First index have the number BIGSTART_INDEX
  */
typedef enum {
	bcrx_32 = BIGSTART_INDEX,
	bcrx_26_5_1,
	bcrx_25_5_2,
	bcrx_25_6_1,
	bcrx_24_5_3,
	bcrx_24_6_2,
	bcrx_23_6_3,
	bcrx_22_5_5,
	bcrx_22_6_4,
	bcrx_22_7_3,
	bcrx_21_6_5,
	bcrx_21_7_4,
	bcrx_20_6_6,
	bcrx_20_7_5,
	bcrx_20_8_4,
	bcrx_19_6_7,
	bcrx_19_7_6,
	bcrx_19_8_5,
	bcrx_18_6_8,
	bcrx_18_7_7,
	bcrx_17_6_9,
	bcrx_17_7_8,
	bcrx_16_6_10,
	bcrx_16_7_9,
	bcrx_15_6_11,
	bcrx_15_7_10,
	bcrx_14_5_13,
	bcrx_14_6_12,
	bcrx_14_7_11,
	bcrx_14_10_8,
	bcrx_14_11_7,
	bcrx_14_12_6,
	bcrx_14_13_5,
	bcrx_13_6_13,
	bcrx_13_7_12,
	bcrx_12_5_15,
	bcrx_12_6_14,
	bcrx_12_7_13,
	bcrx_12_8_12,
	bcrx_11_5_16,
	bcrx_11_6_15,
	bcrx_11_7_14,
	bcrx_11_8_13,
	bcrx_11_10_11,
	bcrx_11_10_5_5_1,
	bcrx_10_5_17,
	bcrx_10_5_7_5_5,
	bcrx_10_6_16,
	bcrx_10_7_15,
	bcrx_10_9_13,
	bcrx_10_12_10,
	bcrx_9_5_18,
	bcrx_9_5_8_5_5,
	bcrx_9_6_17,
	bcrx_9_6_7_5_5,
	bcrx_9_7_16,
	bcrx_9_10_13,
	bcrx_9_14_9,
	bcrx_9_14_3_5_1,
	bcrx_9_15_8,
	bcrx_8_5_9_5_5,
	bcrx_8_6_18,
	bcrx_8_6_8_5_5,
	bcrx_8_7_17,
	bcrx_8_11_13,
	bcrx_8_16_8,
	bcrx_7_5_10_5_5,
	bcrx_7_6_9_5_5,
	bcrx_7_7_18,
	bcrx_7_12_13,
	bcrx_7_18_7,
	bcrx_7_18_1_5_1,
	bcrx_6_4_4_5_13,
	bcrx_6_5_11_5_5,
	bcrx_6_5_3_5_13,
	bcrx_6_6_10_5_5,
	bcrx_6_6_2_5_13,
	bcrx_6_7_19,
	bcrx_6_7_1_5_13,
	bcrx_6_20_6,
	bcrx_6_25_1,
	bcrx_5_5_12_5_5,
	bcrx_5_6_11_5_5,
	bcrx_5_7_20,
	bcrx_5_8_6_8_5,
	bcrx_5_22_5,
	bcrx_5_26_1,
	bcrx_4_4_24,
	bcrx_4_5_14_6_3,
	bcrx_4_5_13_5_5,
	bcrx_4_6_12_5_5,
	bcrx_4_6_12_6_4,
	bcrx_4_6_12_7_3,
	bcrx_4_7_21,
	bcrx_4_7_10_7_4,
	bcrx_4_8_8_8_4,
	bcrx_4_8_8_11_1,
	bcrx_3_4_25,
	bcrx_3_5_24,
	bcrx_3_5_16_5_3,
	bcrx_3_5_16_6_2,
	bcrx_3_5_14_5_5,
	bcrx_3_6_23,
	bcrx_3_6_14_5_4,
	bcrx_3_6_14_6_3,
	bcrx_3_6_14_8_1,
	bcrx_3_6_13_5_5,
	bcrx_3_7_22,
	bcrx_3_7_12_6_4,
	bcrx_3_7_12_7_3,
	bcrx_3_7_12_9_1,
	bcrx_2_3_27,
	bcrx_2_4_26,
	bcrx_2_5_25,
	bcrx_2_5_18_5_2,
	bcrx_2_5_18_6_1,
	bcrx_2_5_15_5_5,
	bcrx_2_6_24,
	bcrx_2_6_16_6_2,
	bcrx_2_6_16_7_1,
	bcrx_2_6_14_5_5,
	bcrx_1_5_26,
	bcrx_1_5_20_5_1,
	bcrx_1_5_5_10_11,
	bcrx_1_5_3_14_9,
	bcrx_1_5_1_18_7,
	bcrx_1_6_25,
	bcrx_1_6_18_5_2,
	bcrx_1_6_18_6_1,
	bcrx_1_6_17_6_2,
	bcrx_1_6_15_5_5,
	bcrx_1_7_16_6_2,
	bcrx_1_7_15_6_3,
	bcrx_1_8_14_6_3,
	bcrx_1_9_12_7_3,
	bcrx_1_11_8_8_4,
	bcrx_1_25_6,
	bcrx_1_26_5,
	bcrx_1_28_3,
	bcrx_1_30_1
} BigCharRowIndx_t;


/*!*****************************************************************************
  @brief	Big Characters '...' arrays of rows structure
  @note		in each BigChar: (number_of, ) row_index, ... 0
  */
static const struct bcrx {
	uint8_t BigSpace[3];				// 'space' (default, dummy)
	uint8_t BigComma[11];				// ','
	uint8_t BigDot[7];					// '.'
	uint8_t Big0[25];					// '0'
	uint8_t Big1[17];					// '1'
	uint8_t Big2[45];					// '2'
	uint8_t Big3[46];					// '3'
	uint8_t Big4[32];					// '4'
	uint8_t Big5[31];					// '5'
	uint8_t Big6[38];					// '6'
	uint8_t Big7[46];					// '7'
	uint8_t Big8[44];					// '8'
	uint8_t Big9[38];					// '9'
} BigCharIndx = {
	{									// 'space' (default, dummy)
	48,	bcrx_32, 0
	},
	{									// ','
	39,	bcrx_32,
		bcrx_3_4_25,
	3,	bcrx_2_6_24,
		bcrx_3_5_24,
		bcrx_4_4_24,
		bcrx_3_4_25,
		bcrx_2_4_26,
		bcrx_2_3_27, 0
	},
	{									// '.'
	42,	bcrx_32,
		bcrx_3_4_25,
	4,	bcrx_2_6_24,
		bcrx_3_4_25, 0
	},
	{									// '0'
		bcrx_11_10_11,
		bcrx_9_14_9,
		bcrx_7_18_7,
		bcrx_6_20_6,
		bcrx_5_22_5,
		bcrx_4_8_8_8_4,
		bcrx_3_7_12_7_3,
		bcrx_3_6_14_6_3,
		bcrx_2_6_16_6_2,
		bcrx_2_5_18_5_2,
		bcrx_1_6_18_6_1,
	26,	bcrx_1_5_20_5_1,
		bcrx_1_6_18_6_1,
		bcrx_2_5_18_5_2,
		bcrx_2_6_16_6_2,
		bcrx_3_6_14_6_3,
		bcrx_3_7_12_7_3,
		bcrx_4_8_8_8_4,
		bcrx_5_22_5,
		bcrx_6_20_6,
		bcrx_7_18_7,
		bcrx_9_14_9,
		bcrx_11_10_11, 0
	},
	{									// '1'
		bcrx_14_5_13,
		bcrx_13_6_13,
		bcrx_12_7_13,
		bcrx_11_8_13,
		bcrx_10_9_13,
		bcrx_9_10_13,
		bcrx_8_11_13,
		bcrx_7_12_13,
		bcrx_6_7_1_5_13,
		bcrx_6_6_2_5_13,
		bcrx_6_5_3_5_13,
		bcrx_6_4_4_5_13,
	31,	bcrx_14_5_13,
	5,	bcrx_9_15_8, 0
	},
	{									// '2'
		bcrx_11_10_11,
		bcrx_9_14_9,
		bcrx_7_18_7,
		bcrx_6_20_6,
		bcrx_5_22_5,
		bcrx_4_8_8_8_4,
		bcrx_3_7_12_7_3,
		bcrx_3_6_14_6_3,
		bcrx_2_6_16_6_2,
		bcrx_2_5_18_5_2,
		bcrx_1_6_18_6_1,
	3,	bcrx_1_5_20_5_1,
		bcrx_26_5_1,
		bcrx_25_6_1,
		bcrx_25_5_2,
		bcrx_24_6_2,
		bcrx_23_6_3,
		bcrx_22_7_3,
		bcrx_21_7_4,
		bcrx_20_7_5,
		bcrx_19_7_6,
		bcrx_18_7_7,
		bcrx_17_7_8,
		bcrx_16_7_9,
		bcrx_15_7_10,
		bcrx_14_7_11,
		bcrx_13_7_12,
		bcrx_12_7_13,
		bcrx_11_7_14,
		bcrx_10_7_15,
		bcrx_9_7_16,
		bcrx_8_7_17,
		bcrx_7_7_18,
		bcrx_6_7_19,
		bcrx_5_7_20,
		bcrx_4_7_21,
		bcrx_3_7_22,
		bcrx_3_6_23,
	2,	bcrx_2_6_24,
		bcrx_1_6_25,
	5,	bcrx_1_30_1, 0
	},
	{									// '3'
		bcrx_12_8_12,
		bcrx_10_12_10,
		bcrx_8_16_8,
		bcrx_7_18_7,
		bcrx_6_20_6,
		bcrx_5_8_6_8_5,
		bcrx_4_7_10_7_4,
		bcrx_4_6_12_6_4,
		bcrx_3_6_14_6_3,
		bcrx_3_5_16_5_3,
		bcrx_3_5_16_6_2,
	4,	bcrx_25_5_2,
		bcrx_24_6_2,
		bcrx_24_5_3,
		bcrx_23_6_3,
		bcrx_22_6_4,
		bcrx_21_7_4,
		bcrx_19_8_5,
		bcrx_14_12_6,
		bcrx_14_11_7,
		bcrx_14_10_8,
		bcrx_14_12_6,
		bcrx_14_13_5,
		bcrx_20_8_4,
		bcrx_22_7_3,
		bcrx_23_6_3,
		bcrx_24_6_2,
		bcrx_25_5_2,
		bcrx_25_6_1,
	3,	bcrx_26_5_1,
	2,	bcrx_1_5_20_5_1,
		bcrx_1_6_18_6_1,
		bcrx_2_5_18_5_2,
		bcrx_2_6_16_6_2,
		bcrx_3_6_14_6_3,
		bcrx_3_7_12_7_3,
		bcrx_4_8_8_8_4,
		bcrx_5_22_5,
		bcrx_6_20_6,
		bcrx_7_18_7,
		bcrx_9_14_9,
		bcrx_11_10_11, 0
	},
	{									// '4'
	3,	bcrx_10_5_7_5_5,
		bcrx_9_6_7_5_5,
	2,	bcrx_9_5_8_5_5,
		bcrx_8_6_8_5_5,
	2,	bcrx_8_5_9_5_5,
		bcrx_7_6_9_5_5,
	2,	bcrx_7_5_10_5_5,
		bcrx_6_6_10_5_5,
	2,	bcrx_6_5_11_5_5,
		bcrx_5_6_11_5_5,
	2,	bcrx_5_5_12_5_5,
		bcrx_4_6_12_5_5,
	2,	bcrx_4_5_13_5_5,
		bcrx_3_6_13_5_5,
	2,	bcrx_3_5_14_5_5,
		bcrx_2_6_14_5_5,
	2,	bcrx_2_5_15_5_5,
		bcrx_1_6_15_5_5,
	5,	bcrx_1_30_1,
	15,	bcrx_22_5_5, 0
	},
	{									// '5'
	5,	bcrx_1_28_3,
	11,	bcrx_1_5_26,
		bcrx_1_5_5_10_11,
		bcrx_1_5_3_14_9,
		bcrx_1_5_1_18_7,
		bcrx_1_25_6,
		bcrx_1_26_5,
		bcrx_1_11_8_8_4,
		bcrx_1_9_12_7_3,
		bcrx_1_7_15_6_3,
		bcrx_1_6_17_6_2,
		bcrx_25_5_2,
		bcrx_25_6_1,
	8,	bcrx_26_5_1,
	2,	bcrx_1_5_20_5_1,
		bcrx_1_6_18_6_1,
		bcrx_2_5_18_5_2,
		bcrx_2_6_16_6_2,
		bcrx_3_6_14_6_3,
		bcrx_3_7_12_7_3,
		bcrx_4_8_8_8_4,
		bcrx_5_22_5,
		bcrx_6_20_6,
		bcrx_7_18_7,
		bcrx_9_14_9,
		bcrx_11_10_11, 0
	},
	{									// '6'
		bcrx_11_10_11,
		bcrx_9_14_9,
		bcrx_7_18_7,
		bcrx_6_20_6,
		bcrx_5_22_5,
		bcrx_4_8_8_8_4,
		bcrx_3_7_12_6_4,
		bcrx_3_6_14_5_4,
		bcrx_2_6_24,
		bcrx_2_5_25,
		bcrx_1_6_25,
	5,	bcrx_1_5_26,
		bcrx_1_5_5_10_11,
		bcrx_1_5_3_14_9,
		bcrx_1_5_1_18_7,
		bcrx_1_25_6,
		bcrx_1_26_5,
		bcrx_1_11_8_8_4,
		bcrx_1_9_12_7_3,
		bcrx_1_8_14_6_3,
		bcrx_1_7_16_6_2,
		bcrx_1_6_18_5_2,
		bcrx_1_6_18_6_1,
	10,	bcrx_1_5_20_5_1,
		bcrx_1_6_18_6_1,
		bcrx_2_5_18_5_2,
		bcrx_2_6_16_6_2,
		bcrx_3_6_14_6_3,
		bcrx_3_7_12_7_3,
		bcrx_4_8_8_8_4,
		bcrx_5_22_5,
		bcrx_6_20_6,
		bcrx_7_18_7,
		bcrx_9_14_9,
		bcrx_11_10_11, 0
	},
	{									// '7'
	5,	bcrx_1_30_1,
	2,	bcrx_25_6_1,
	2,	bcrx_24_6_2,
	2,	bcrx_23_6_3,
	2,	bcrx_22_6_4,
	2,	bcrx_21_6_5,
	2,	bcrx_20_6_6,
	2,	bcrx_19_6_7,
	2,	bcrx_18_6_8,
	2,	bcrx_17_6_9,
	2,	bcrx_16_6_10,
	2,	bcrx_15_6_11,
	2,	bcrx_14_6_12,
	2,	bcrx_13_6_13,
	2,	bcrx_12_6_14,
		bcrx_12_5_15,
	2,	bcrx_11_6_15,
	2,	bcrx_11_5_16,
	2,	bcrx_10_6_16,
	2,	bcrx_10_5_17,
	2,	bcrx_9_6_17,
	4,	bcrx_9_5_18, 0
	},
	{									// '8'
		bcrx_12_8_12,
		bcrx_10_12_10,
		bcrx_8_16_8,
		bcrx_7_18_7,
		bcrx_6_20_6,
		bcrx_5_8_6_8_5,
		bcrx_4_7_10_7_4,
		bcrx_4_6_12_6_4,
		bcrx_3_6_14_6_3,
		bcrx_3_5_16_5_3,
		bcrx_2_6_16_6_2,
	4,	bcrx_2_5_18_5_2,
		bcrx_2_6_16_6_2,
		bcrx_3_5_16_5_3,
		bcrx_3_6_14_6_3,
		bcrx_4_6_12_6_4,
		bcrx_4_7_10_7_4,
		bcrx_5_8_6_8_5,
		bcrx_6_20_6,
		bcrx_7_18_7,
		bcrx_8_16_8,
		bcrx_6_20_6,
		bcrx_5_22_5,
		bcrx_4_8_8_8_4,
		bcrx_3_7_12_7_3,
		bcrx_3_6_14_6_3,
		bcrx_2_6_16_6_2,
		bcrx_2_5_18_5_2,
		bcrx_1_6_18_6_1,
	5,	bcrx_1_5_20_5_1,
		bcrx_1_6_18_6_1,
		bcrx_2_5_18_5_2,
		bcrx_2_6_16_6_2,
		bcrx_3_6_14_6_3,
		bcrx_3_7_12_7_3,
		bcrx_4_8_8_8_4,
		bcrx_5_22_5,
		bcrx_6_20_6,
		bcrx_7_18_7,
		bcrx_9_14_9,
		bcrx_11_10_11, 0
	},
	{									// '9'
		bcrx_11_10_11,
		bcrx_9_14_9,
		bcrx_7_18_7,
		bcrx_6_20_6,
		bcrx_5_22_5,
		bcrx_4_8_8_8_4,
		bcrx_3_7_12_7_3,
		bcrx_3_6_14_6_3,
		bcrx_2_6_16_6_2,
		bcrx_2_5_18_5_2,
		bcrx_1_6_18_6_1,
	10,	bcrx_1_5_20_5_1,
		bcrx_1_6_18_6_1,
		bcrx_2_5_18_6_1,
		bcrx_2_6_16_7_1,
		bcrx_3_6_14_8_1,
		bcrx_3_7_12_9_1,
		bcrx_4_8_8_11_1,
		bcrx_5_26_1,
		bcrx_6_25_1,
		bcrx_7_18_1_5_1,
		bcrx_9_14_3_5_1,
		bcrx_11_10_5_5_1,
	5,	bcrx_26_5_1,
		bcrx_25_6_1,
		bcrx_25_5_2,
		bcrx_24_6_2,
		bcrx_4_5_14_6_3,
		bcrx_4_6_12_7_3,
		bcrx_4_8_8_8_4,
		bcrx_5_22_5,
		bcrx_6_20_6,
		bcrx_7_18_7,
		bcrx_9_14_9,
		bcrx_11_10_11, 0
	},
};

/*!*****************************************************************************
  @brief	Character (0x20...0x3f) offsets array
  @note		only digits and dot
  */
static const uint16_t BigCharOffs[] = {
	offsetof(struct bcrx, BigSpace),		// 0x20	sp
	offsetof(struct bcrx, BigSpace),		// 0x21 !
	offsetof(struct bcrx, BigSpace),		// 0x22 "
	offsetof(struct bcrx, BigSpace),		// 0x23 #
	offsetof(struct bcrx, BigSpace),		// 0x24 $
	offsetof(struct bcrx, BigSpace),		// 0x25 %
	offsetof(struct bcrx, BigSpace),		// 0x26 &
	offsetof(struct bcrx, BigSpace),		// 0x27 '
	offsetof(struct bcrx, BigSpace),		// 0x28 (
	offsetof(struct bcrx, BigSpace),		// 0x29 )
	offsetof(struct bcrx, BigSpace),		// 0x2a *
	offsetof(struct bcrx, BigSpace),		// 0x2b +
	offsetof(struct bcrx, BigComma),		// 0x2c ,
	offsetof(struct bcrx, BigSpace),		// 0x2d -
	offsetof(struct bcrx, BigDot),			// 0x2e .
	offsetof(struct bcrx, BigSpace),		// 0x2f /
	offsetof(struct bcrx, Big0),			// 0x30 0 
	offsetof(struct bcrx, Big1),			// 0x31 1 
	offsetof(struct bcrx, Big2),			// 0x32 2 
	offsetof(struct bcrx, Big3),			// 0x33 3 
	offsetof(struct bcrx, Big4),			// 0x34 4 
	offsetof(struct bcrx, Big5),			// 0x35 5 
	offsetof(struct bcrx, Big6),			// 0x36 6 
	offsetof(struct bcrx, Big7),			// 0x37 7 
	offsetof(struct bcrx, Big8),			// 0x38 8 
	offsetof(struct bcrx, Big9),			// 0x39 9
	offsetof(struct bcrx, BigSpace),		// 0x3a :
	offsetof(struct bcrx, BigSpace),		// 0x3b ;
	offsetof(struct bcrx, BigSpace),		// 0x3c <
	offsetof(struct bcrx, BigSpace),		// 0x3d =
	offsetof(struct bcrx, BigSpace),		// 0x3e >
	offsetof(struct bcrx, BigSpace),		// 0x3f ?
};


/*!*****************************************************************************
  @brief	Big Text String Monospace print without Background Padding
  @param	text - pointer to text string
  @param	display - pointer to top left corner in LCD buffer memory
  @param	foreground - palette for foreground (lower byte)
  @return
  */
//void FontBig::BigStringMono_print(char *text, int x, int y, Color color)
//{
//	while (*text) {
//		BigSymbol_print(*text, x, y, color);
//		x += BIGCHAR_WIDTH;
//		text++;
//	}
//}

/*!*****************************************************************************
  @brief	Big Text String Proportional space print without Background Padding
  @param	text - pointer to text string
  @param	display - pointer to top left corner in LCD buffer memory
  @param	foreground - palette for foreground (lower byte)
  @return
  */
void FontBig::BigStringProp_print(char *text, int x, int y, Color color)
{
	while (*text) {
		uint32_t space = BigSymbol_print(*text, x, y, color);
		x += space;
		text++;
	}
}


/*!*****************************************************************************
  @brief	Big Text String Monospace print with Background Padding
  @param	text - pointer to text string
  @param	display - pointer to top left corner in LCD buffer memory
  @param	foreground - palette for foreground (lower byte)
  @param	background - palette for background (lower byte)
  @return
  */
//void FontBig::BigStringMonoPad_print(char *text, uint8_t *display, 
//							uint32_t foreground, uint32_t background)
//{
//	while (*text) {
//		BigSymbolPad_print(*text, display, foreground, background);
//		display += BIGCHAR_WIDTH;
//		text++;
//	}
//}


/*!*****************************************************************************
  @brief	Big Text String Proportional space print with Background Padding
  @param	text - pointer to text string
  @param	display - pointer to top left corner in LCD buffer memory
  @param	foreground - palette for foreground (lower byte)
  @param	background - palette for background (lower byte)
  @return
  */
//void FontBig::BigStringPropPad_print(char *text, uint8_t *display, 
//							uint32_t foreground, uint32_t background)
//{
//	while (*text) {
//		uint32_t space = BigSymbolPad_print(*text, display, foreground, background);
//		display += space;
//		text++;
//	}
//}




/*!*****************************************************************************
  @brief	Big Symbol print without background padding
  @param	symbol - character
  @param	display - pointer to top left corner in LCD buffer memory
  @param	foreground - palette for foreground (lower byte)
  @return	width of symbol in pixels (for proportional string printing)
  */
uint32_t FontBig::BigSymbol_print(uint8_t symbol, int x, int y, Color color)
{
//	assert(0x30 <= symbol && symbol <= 0x39);
    bool falsecheck = false;
    uint32_t ezspace = ' ';
    if(0x28 <= symbol && symbol <= 0x39)
    {
        falsecheck = false;
    }
    else
    {
        falsecheck = true;
    }
    if (falsecheck == true) return ezspace;
	uint16_t offs = BigCharOffs[symbol - ' '];				// offset in BigCharIndx
	const uint8_t *index = (uint8_t *)&BigCharIndx + offs;	// first row index

	uint32_t width = 0;
	if (symbol == ' ') width = BIGSPACE_WIDTH;			// space width

	uint32_t same = 0;									// number of the same rows
	uint32_t row;										// row of pixels
	uint32_t code;
    uint8_t *endMem = (uint8_t *)(0x807FFFF);
    
	while (code = *index++) {
        if (index == endMem) break;
		if (code >= BIGSTART_INDEX) {
			if (!same)  same = 1;						// new row -> one row
			row = BigCharRow[code - BIGSTART_INDEX];
		}
		else {
			same = code;								// multi-rows
			continue;
		}
 
		for ( ; same ; same-- ) {
			uint32_t rowshift = row;					// row for shift
			for (uint32_t i = 0; i < 32; i++) {
				uint32_t pixel = rowshift & 0x80000000;
				if (pixel)  Point().Draw(x + i, y, color);
				if (pixel && i > width)  width = i;
				rowshift <<= 1;
			}
			y++;
		}
	}
	return width + BIGGAP_WIDTH;
}


/*!*****************************************************************************
  @brief	Big Symbol print with background padding
  @param	symbol - character
  @param	display - pointer to top left corner in LCD buffer memory
  @param	foreground - palette for foreground (lower byte)
  @param	background - palette for background (lower byte)
  @return	width of symbol in pixels (for proportional string printing)
  */
//uint32_t FontBig::BigSymbolPad_print(uint8_t symbol, volatile uint8_t *display, 
//						  uint32_t foreground, uint32_t background)
//{
//	assert(0x20 <= symbol && symbol <= 0x3F);
//	uint16_t offs = BigCharOffs[symbol - ' '];				// offset in BigCharIndx
//	const uint8_t *index = (uint8_t *)&BigCharIndx + offs;	// first row index
//
//	uint32_t width = 0;
//	if (symbol == ' ') width = BIGSPACE_WIDTH;			// space width
//
//	uint32_t same = 0;									// number of the same rows
//	uint32_t row;										// row of pixels
//	uint32_t code;
//
//	while (code = *index++) {
//		if (code >= BIGSTART_INDEX) {
//			if (!same)  same = 1;						// new row -> one row
//			row = BigCharRow[code - BIGSTART_INDEX];
//		}
//		else {
//			same = code;								// multi-rows
//			continue;
//		}
//		
//		for ( ; same; same-- ) {
//			uint32_t rowshift = row;					// row for shift
//			for (uint32_t i = 0; i < 32; i++) {
//				uint32_t pixel = rowshift & 0x80000000;
//				*(display + i) =  pixel ? foreground : background;
//				if (pixel && i > width)  width = i;
//				rowshift <<= 1;
//			}
//			display += DISPLAY_WIDTH;
//		}
//	}
//	return width + BIGGAP_WIDTH;
//}
