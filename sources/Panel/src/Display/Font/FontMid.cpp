/*!*****************************************************************************
  @file		FontMid.c
  @author	VicKo
  @brief	Font Middle-size 24 (+6 down) x 15 (+1 right) pixel
  @date		2020.07
  @note		lower-case symbols 17 x 12
  ******************************************************************************/

  /*! @verbatim
	  Symbols:
	  (%), *, +, -, 0...9, A, B, E, G, H, M, V, b, d, k, m, n, s, u(µ), z

	  Subpixel palette (Right, Center, Left subpiXels):
	  000		XC0
	  001		XR1
	  010			XC1
	  011		XR2
	  100		XL1
	  101			XC2
	  110		XL2
	  111		XC3
	  Для 8 горизонтальных пикселей нужно 24 бита = 3 байта.
	  Для символа 10 * 16 пикселей нужно:
	  (3 + 3) * 16 = 96 байтов
	  Для 256 символов нужно 24'576 байтов. Но при кодировании повторяющихся строк
  можно получить двойное сжатие.


	  @endverbatim
	*/

	//#include "Application.h"
#include "FontMid.h"
#include "Display/Font/FontMid.h"
#include "Display/Primitives.h"
#include "Display/Display.h"
using namespace Primitives;

static const uint16_t	FontMidRow[] = {
	0x0000,		//	0000'0000'0000'0000		s16
	0x000E,		//	0000'0000'0000'1110		s12_3_1				( )
	0x001C,		//	0000'0000'0001'1100		s11_3_2				( )
	0x0038,		//	0000'0000'0011'1000		s10_3_3				( )
	0x003C,		//	0000'0000'0011'1100		s10_4_2				( )
	0x0070,		//	0000'0000'0111'0000		s9_3_4				(dsz)
	0x0078,		//	0000'0000'0111'1000		s9_4_3				( )
	0x00E0,		//	0000'0000'1110'0000		s8_3_5				( )
	0x00F0,		//	0000'0000'1111'0000		s8_4_4				(sz)
	0x0100,		//	0000'0001'0000'0000		s7_1_8				(AV)
	0x01C0,		//	0000'0001'1100'0000		s7_3_6				( )
	0x01E0,		//	0000'0001'1110'0000		s7_4_5				(z)
	0x01F0,		//	0000'0001'1111'0000		s7_5_4				( )
	0x01F8,		//	0000'0001'1111'1000		s7_6_3				( )
	0x01FC,		//	0000'0001'1111'1100		s7_7_2				(%)
	0x0380,		//	0000'0011'1000'0000		s6_3_7				(AV)
	0x038E,		//	0000'0011'1000'1110		s6_3_3_3_1			(%)
	0x03C0,		//	0000'0011'1100'0000		s6_4_6				(z)
	0x0700,		//	0000'0111'0000'0000		s5_3_8				( )
	0x0780,		//	0000'0111'1000'0000		s5_4_7				(z)
	0x07C0,		//	0000'0111'1100'0000		s5_5_6				(AV)
	0x07CE,		//	0000'0111'1100'1110		s5_5_2_3_1			( )
	0x0E00,		//	0000'1110'0000'0000		s4_3_9				( )
	0x0EE0,		//	0000'1110'1110'0000		s4_3_1_3_5			(AV)
	0x0F00,		//	0000'1111'0000'0000		s4_4_8				(z)
	0x0F80,		//	0000'1111'1000'0000		s4_5_7				( )
	0x0FE0,		//	0000'1111'1110'0000		s4_7_5				(AV)
	0x0FF0,		//	0000'1111'1111'0000		s4_8_4				(G)
	0x1C00,		//	0001'1100'0000'0000		s3_3_10				(%)
	0x1C70,		//	0001'1100'0111'0000		s3_3_3_3_4			(AH)
	0x1C78,		//	0001'1100'0111'1000		s3_3_3_4_3			(%)
	0x1E00,		//	0001'1110'0000'0000		s3_4_9				(z)
	0x1EF0,		//	0001'1110'1111'0000		s3_4_1_4_4			(V)
	0x1F70,		//	0001'1111'0111'0000		s3_5_1_3_4			(du)
	0x1F80,		//	0001'1111'1000'0000		s3_6_7				(s)
	0x1FE0,		//	0001'1111'1110'0000		s3_8_5				(s)
	0x1FEE,		//	0001'1111'1110'1110		s3_8_1_3_1			( )
	0x1FF0,		//	0001'1111'1111'0000		s3_9_4				(s)
	0x3838,		//	0011'1000'0011'1000		s2_3_5_3_3			(AH)
	0x3870,		//	0011'1000'0111'0000		s2_3_4_3_4			( )
	0x3B80,		//	0011'1011'1000'0000		s2_3_1_3_7			( )
	0x3C00,		//	0011'1100'0000'0000		s2_4_10				(z)
	0x3C70,		//	0011'1100'0111'0000		s2_4_3_3_4			(%)
	0x3C78,		//	0011'1100'0111'1000		s2_4_3_4_3			(AH)
	0x3F80,		//	0011'1111'1000'0000		s2_7_7				( )
	0x3FF8,		//	0011'1111'1111'1000		s2_11_3				(G)
	0x3FFC,		//	0011'1111'1111'1100		s2_12_2				(G)
	0x3FFE,		//	0011'1111'1111'1110		s2_13_1				( )
	0x700E,		//	0111'0000'0000'1110		s1_3_8_3_1			(G)
	0x701C,		//	0111'0000'0001'1100		s1_3_7_3_2			(V)
	0x701E,		//	0111'0000'0001'1110		s1_3_7_4_1			( )
	0x7070,		//	0111'0000'0111'0000		s1_3_5_3_4			( )
	0x7800,		//	0111'1000'0000'0000		s1_4_11				(z)
	0x781E,		//	0111'1000'0001'1110		s1_4_6_4_1			(G)
	0x783C,		//	0111'1000'0011'1100		s1_4_5_4_2			(AH)
	0x783E,		//	0111'1000'0011'1110		s1_4_5_5_1			( )
	0x79FC,		//	0111'1001'1111'1100		s1_4_2_7_2			(%)
	0x7F00,		//	0111'1111'0000'0000		s1_7_8				(%)
	0x7F1E,		//	0111'1111'0001'1110		s1_7_3_4_1			(%)
	0x7F3C,		//	0111'1111'0011'1100		s1_7_2_4_2			(%)
	0x7F80,		//	0111'1111'1000'0000		s1_8_7				(s)
	0x7FE0,		//	0111'1111'1110'0000		s1_10_5				(s)
	0x7FF0,		//	0111'1111'1111'0000		s1_11_4				(du)
	0x7FFC,		//	0111'1111'1111'1100		s1_13_2				(A)
	0xC38E,		//	1100'0011'1000'1110		m2_4_3_3_3_1		(%)
	0xE000,		//	1110'0000'0000'0000		m3_13				(Gbksz)
	0xE00E,		//	1110'0000'0000'1110		m3_9_3_1			(ABH)
	0xE01C,		//	1110'0000'0001'1100		m3_8_3_2			(B)
	0xE01E,		//	1110'0000'0001'1110		m3_8_4_1			(B)
	0xE01F,		//	1110'0000'0001'1111		m3_8_5				(s)
	0xE03C,		//	1110'0000'0011'1100		m3_7_4_2			(B)
	0xE03E,		//	1110'0000'0011'1110		m3_7_5_1			(B)
	0xE070,		//	1110'0000'0111'0000		m3_6_3_4			(bdknu)
	0xE07E,		//	1110'0000'0111'1110		m3_6_6_1			(G)
	0xE0E0,		//	1110'0000'1110'0000		m3_5_3_5			(k)
	0xE0F0,		//	1110'0000'1111'0000		m3_5_4_4			(s)
	0xE10E,		//	1110'0001'0000'1110		m3_4_1_4_3_1		(M)
	0xE1C0,		//	1110'0001'1100'0000		m3_4_3_6			(k)
	0xE1E0,		//	1110'0001'1110'0000		m3_4_4_5			(k)
	0xE380,		//	1110'0011'1000'0000		m3_3_3_7			(%k)
	0xE386,		//	1110'0011'1000'0110		m3_3_3_4_2_1		(%)
	0xE38E,		//	1110'0011'1000'1110		m3_3_3_3_3_1		(Mm)
	0xE3C0,		//	1110'0011'1100'0000		m3_3_4_6			(k)
	0xE700,		//	1110'0111'0000'0000		m3_2_3_8			(k)
	0xE780,		//	1110'0111'1000'0000		m3_2_4_7			(k)
	0xE7C0,		//	1110'0111'1100'0000		m3_2_5_6			(k)
	0xE7CE,		//	1110'0111'1100'1110		m3_2_5_2_3_1		(M)
	0xEE00,		//	1110'1110'0000'0000		m3_1_3_9			(k)
	0xEE70,		//	1110'1110'0111'0000		m3_1_3_2_3_4		(m)
	0xEEEE,		//	1110'1110'1110'1110		m3_1_3_1_3_1_3_1	(M)
	0xEF00,		//	1110'1111'0000'0000		m3_1_4_8			(k)
	0xEF80,		//	1110'1111'1000'0000		m3_1_5_7			(bn)
	0xEFF0,		//	1110'1111'1111'0000		m3_1_8_4			( )
	0xF000,		//	1111'0000'0000'0000		m4_12				(Gsz)
	0xF00E,		//	1111'0000'0000'1110		m4_8_3_1			(G)
	0xF01C,		//	1111'0000'0001'1100		m4_7_3_2			( )
	0xF01E,		//	1111'0000'0001'1110		m4_7_4_1			(M)
	0xF070,		//	1111'0000'0111'0000		m4_5_3_4			(s)
	0xF0F0,		//	1111'0000'1111'0000		m4_4_4_4			(bdnu)
	0xF1FC,		//	1111'0001'1111'1100		m4_3_7_2			(%)
	0xF39E,		//	1111'0011'1001'1110		m4_2_3_2_4_1		( )
	0xF7DE,		//	1111'0111'1101'1110		m4_1_5_1_4_1		(m)
	0xF83C,		//	1111'1000'0011'1100		m5_5_4_2			( )
	0xF83E,		//	1111'1000'0011'1110		m5_5_5_1			(M)
	0xFBBE,		//	1111'1011'1011'1110		m5_1_3_1_5_1		( )
	0xFC00,		//	1111'1100'0000'0000		m6_10				(k)
	0xFC7E,		//	1111'1100'0111'1110		m6_3_6_1			(M)
	0xFE00,		//	1111'1110'0000'0000		m7_9				(k)
	0xFF80,		//	1111'1111'1000'0000		m9_7				(s)
	0xFFC0,		//	1111'1111'1100'0000		m10_6				(B)
	0xFFE0,		//	1111'1111'1110'0000		m11_5				(Bbn)
	0xFFF0,		//	1111'1111'1111'0000		m12_4				(Bbdnsuz)
	0xFFF8,		//	1111'1111'1111'1000		m13_3				(B)
	0xFFFC,		//	1111'1111'1111'1100		m14_2				(Bm)
	0xFFFE,		//	1111'1111'1111'1110		m15_1				(AHm)
};


/*!*****************************************************************************
  @brief	Font Middle-Size rows index (8-bit)
  @note		Comments: bitmap, spase-mark-space... (chars...)
  */
typedef enum
{
	fmrx_s16 = FONTMID_STARTINDEX,
	fmrx_s12_3_1,					//  
	fmrx_s11_3_2,					//  
	fmrx_s10_3_3,					//  
	fmrx_s10_4_2,					//  
	fmrx_s9_3_4,					//  (%dsz)
	fmrx_s9_4_3,					//  
	fmrx_s8_3_5,					//  
	fmrx_s8_4_4,					//  (%sz)
	fmrx_s7_1_8,					//  (AV)
	fmrx_s7_3_6,					//  
	fmrx_s7_4_5,					//  (%z)
	fmrx_s7_5_4,					//  
	fmrx_s7_6_3,					//  
	fmrx_s7_7_2,					//  (%)
	fmrx_s6_3_7,					//  (AV)
	fmrx_s6_3_3_3_1,				//  (%)
	fmrx_s6_4_6,					//  (%z)
	fmrx_s5_3_8,					//  
	fmrx_s5_4_7,					//  (%z)
	fmrx_s5_5_6,					//  (0AGV)
	fmrx_s5_5_2_3_1,				//  
	fmrx_s4_3_9,					//  
	fmrx_s4_3_1_3_5,				//  (AV)
	fmrx_s4_4_8,					//  (%z)
	fmrx_s4_5_7,					//  
	fmrx_s4_7_5,					//  (AV)
	fmrx_s4_8_4,					//  (G)
	fmrx_s3_3_10,					//  (%)
	fmrx_s3_3_3_3_4,				//  (AH)
	fmrx_s3_3_3_4_3,				//  (%)
	fmrx_s3_4_9,					//  (%z)
	fmrx_s3_4_1_4_4,				//  (V)
	fmrx_s3_5_1_3_4,				//  (du)
	fmrx_s3_6_7,					//  (s)
	fmrx_s3_8_5,					//  (%)
	fmrx_s3_8_1_3_1,				//  
	fmrx_s3_9_4,					//  (0s)
	fmrx_s2_3_5_3_3,				//  (AH)
	fmrx_s2_3_4_3_4,				//  
	fmrx_s2_3_1_3_7,				//  
	fmrx_s2_4_10,					//  (z)
	fmrx_s2_4_3_3_4,				//  (%)
	fmrx_s2_4_3_4_3,				//  (AH)
	fmrx_s2_7_7,					//  
	fmrx_s2_11_3,					//  (G)
	fmrx_s2_12_2,					//  (G)
	fmrx_s2_13_1,					//  
	fmrx_s1_3_8_3_1,				//  (G)
	fmrx_s1_3_7_3_2,				//  (AV)
	fmrx_s1_3_7_4_1,				//  
	fmrx_s1_3_5_3_4,				//
	fmrx_s1_4_11,					//  (z)
	fmrx_s1_4_6_4_1,				//  (G)
	fmrx_s1_4_5_4_2,				//  (AH)
	fmrx_s1_4_5_5_1,				//  
	fmrx_s1_4_2_7_2,				//  (%)
	fmrx_s1_7_8,					//  (%)
	fmrx_s1_7_3_4_1,				//  (%)
	fmrx_s1_7_2_4_2,				//  (%)
	fmrx_s1_8_7,					//  (s)
	fmrx_s1_10_5,					//  (s)
	fmrx_s1_11_4,					//  (dsu)
	fmrx_s1_13_2,					//  (A)
	fmrx_m2_4_3_3_3_1,				//  (%)
	fmrx_m3_13,						//  (Gbksz)
	fmrx_m3_9_3_1,					//  (0ABH)
	fmrx_m3_8_3_2,					//  (B)
	fmrx_m3_8_4_1,					//  (B)
	fmrx_m3_8_5,					//  
	fmrx_m3_7_4_2,					//  (B)
	fmrx_m3_7_5_1,					//  (B)
	fmrx_m3_6_3_4,					//  (bdknu)
	fmrx_m3_6_6_1,					//  (G)
	fmrx_m3_5_3_5,					//  (k)
	fmrx_m3_5_4_4,					//  (s)
	fmrx_m3_4_1_4_3_1,				//  (M)
	fmrx_m3_4_3_6,					//  (k)
	fmrx_m3_4_4_5,					//  (k)
	fmrx_m3_3_3_7,					//  (%k)
	fmrx_m3_3_3_4_2_1,				//  (%)
	fmrx_m3_3_3_3_3_1,				//  (%Mm)
	fmrx_m3_3_4_6,					//  (k)
	fmrx_m3_2_3_8,					//  (k)
	fmrx_m3_2_4_7,					//  (k)
	fmrx_m3_2_5_6,					//  
	fmrx_m3_2_5_2_3_1,				//  (M)
	fmrx_m3_1_3_9,					//  (k)
	fmrx_m3_1_3_2_3_4,				//  (m)
	fmrx_m3_1_3_1_3_1_3_1,			//  (M)
	fmrx_m3_1_4_8,					//  (k)
	fmrx_m3_1_5_7,					//  (bn)
	fmrx_m3_1_8_4,					//  
	fmrx_m4_12,						//  (Gsz)
	fmrx_m4_8_3_1,					//  (G)
	fmrx_m4_7_3_2,					//  
	fmrx_m4_7_4_1,					//  (M)
	fmrx_m4_5_3_4,					//  (s)
	fmrx_m4_4_4_4,					//  (bdnu)
	fmrx_m4_3_7_2,					//  (%)
	fmrx_m4_2_3_2_4_1,				//  
	fmrx_m4_1_5_1_4_1,				//  (m)
	fmrx_m5_5_4_2,					//  
	fmrx_m5_5_5_1,					//  (M)
	fmrx_m5_1_3_1_5_1,				//  
	fmrx_m6_10,						//  (k)
	fmrx_m6_3_6_1,					//  (M)
	fmrx_m7_9,						//  (k)
	fmrx_m9_7,						//  (s)
	fmrx_m10_6,						//  (B)
	fmrx_m11_5,						//  (Bbns)
	fmrx_m12_4,						//  (Bbdnsuz)
	fmrx_m13_3,						//  (B)
	fmrx_m14_2,						//  (Bm)
	fmrx_m15_1,						//  (AHm)
} FontMidRowIndex_t;


/*!*****************************************************************************
  @brief	Font Middle-size arrays of rows
  @note		in each Symbol: (number_of,) row_index, ... 0
  */
static const uint8_t fontmid_space[] = {
	FONTMID_SYMBHEIGHT, fmrx_s16, 0
};

static const uint8_t fontmid_percent[] = {
		fmrx_s3_3_10,
	2,	fmrx_s1_7_8,
		fmrx_m3_3_3_7,
		fmrx_m3_3_3_4_2_1,
		fmrx_m3_3_3_3_3_1,
		fmrx_s1_7_3_4_1,
		fmrx_s1_7_2_4_2,
		fmrx_s3_3_3_4_3,
		fmrx_s8_4_4,
		fmrx_s7_4_5,
		fmrx_s6_4_6,
		fmrx_s5_4_7,
		fmrx_s4_4_8,
		fmrx_s3_4_9,
		fmrx_s2_4_3_3_4,
		fmrx_s1_4_2_7_2,
		fmrx_m4_3_7_2,
		fmrx_m3_3_3_3_3_1,
		fmrx_m2_4_3_3_3_1,
		fmrx_s6_3_3_3_1,
	2,	fmrx_s7_7_2,
		fmrx_s9_3_4,	0
};

static const uint8_t fontmid_asterisk[] = {		//  *
	2,	fmrx_s16,
	3,	fmrx_s6_3_7,
		fmrx_m3_3_3_3_3_1,
		fmrx_m4_2_3_2_4_1,
		fmrx_m5_1_3_1_5_1,
		fmrx_s2_11_3,
		fmrx_s3_9_4,
		fmrx_s4_7_5,
		fmrx_s3_9_4,
		fmrx_s2_11_3,
		fmrx_m5_1_3_1_5_1,
		fmrx_m4_2_3_2_4_1,
		fmrx_m3_3_3_3_3_1,
	3,	fmrx_s6_3_7,
	5,	fmrx_s16,	0
};

static const uint8_t fontmid_plus[] = {			//  +
	6,	fmrx_s16,
	6,	fmrx_s6_3_7,
	3,	fmrx_m15_1,
	6,	fmrx_s6_3_7,
	3,	fmrx_s16,	0
};

static const uint8_t fontmid_minus[] = {		//  -
	12,	fmrx_s16,
	3,	fmrx_m15_1,
	9,	fmrx_s16,	0
};

static const uint8_t fontmid_0[] = {			//  0
		fmrx_s5_5_6,
		fmrx_s3_9_4,
		fmrx_s2_11_3,
		fmrx_s1_4_5_4_2,
		fmrx_s1_3_7_3_2,
	14,	fmrx_m3_9_3_1,
		fmrx_s1_3_7_3_2,
		fmrx_s1_4_5_4_2,
		fmrx_s2_11_3,
		fmrx_s3_9_4,
		fmrx_s5_5_6,	0
};

static const uint8_t fontmid_1[] = {			//  1
		fmrx_s6_3_7,
		fmrx_s5_4_7,
		fmrx_s4_5_7,
		fmrx_s3_6_7,
		fmrx_s2_7_7,
		fmrx_s2_3_1_3_7,
	15,	fmrx_s6_3_7,
	3,	fmrx_s4_7_5,	0
};

static const uint8_t fontmid_2[] = {			//  2
		fmrx_s5_5_6,
		fmrx_s3_9_4,
		fmrx_s2_11_3,
		fmrx_s1_4_5_4_2,
		fmrx_s1_3_7_3_2,
	3,	fmrx_m3_9_3_1,
		fmrx_s12_3_1,
		fmrx_s11_3_2,
		fmrx_s10_4_2,
		fmrx_s9_4_3,
		fmrx_s8_4_4,
		fmrx_s7_4_5,
		fmrx_s6_4_6,
		fmrx_s5_4_7,
		fmrx_s4_4_8,
		fmrx_s3_4_9,
		fmrx_s2_4_10,
		fmrx_s1_4_11,
		fmrx_m4_12,
	3,	fmrx_m15_1,		0
};

static const uint8_t fontmid_3[] = {			//  3
		fmrx_s5_5_6,
		fmrx_s3_9_4,
		fmrx_s2_11_3,
		fmrx_s1_4_5_4_2,
		fmrx_s1_3_7_3_2,
	2,	fmrx_m3_9_3_1,
		fmrx_s12_3_1,
		fmrx_s11_3_2,
		fmrx_s10_4_2,
		fmrx_s7_6_3,
		fmrx_s7_5_4,		// --
		fmrx_s7_6_3,
		fmrx_s10_4_2,
		fmrx_s11_3_2,
	2,	fmrx_s12_3_1,
	2,	fmrx_m3_9_3_1,
		fmrx_s1_3_7_3_2,
		fmrx_s1_4_5_4_2,
		fmrx_s2_11_3,
		fmrx_s3_9_4,
		fmrx_s5_5_6,	0
};

static const uint8_t fontmid_4[] = {			//  4
	3,	fmrx_s5_3_8,
	3,	fmrx_s4_3_9,
		fmrx_s3_3_10,
	2,	fmrx_s3_3_3_3_4,
	3,	fmrx_s2_3_4_3_4,
	2,	fmrx_s1_3_5_3_4,
	2,	fmrx_m3_6_3_4,
	3,	fmrx_m15_1,
	5,	fmrx_s9_3_4,	0
};

static const uint8_t fontmid_5[] = {			//  5
	3,	fmrx_m14_2,
	5,	fmrx_m3_13,
		fmrx_m3_2_5_6,
		fmrx_m3_1_8_4,
		fmrx_m13_3,
		fmrx_m5_5_4_2,
		fmrx_m4_7_3_2,
		fmrx_m3_9_3_1,
	5,	fmrx_s12_3_1,
		fmrx_s1_3_7_3_2,
		fmrx_s1_4_5_4_2,
		fmrx_s2_11_3,
		fmrx_s3_9_4,
		fmrx_s5_5_6,	0
};

static const uint8_t fontmid_6[] = {			//  6
		fmrx_s5_5_6,
		fmrx_s3_9_4,
		fmrx_s2_11_3,
		fmrx_s1_4_5_4_2,
		fmrx_s1_3_7_3_2,
	4,	fmrx_m3_13,
		fmrx_m3_2_5_6,
		fmrx_m3_1_8_4,
		fmrx_m13_3,
		fmrx_m5_5_4_2,
		fmrx_m4_7_3_2,
	5,	fmrx_m3_9_3_1,
		fmrx_s1_3_7_3_2,
		fmrx_s1_4_5_4_2,
		fmrx_s2_11_3,
		fmrx_s3_9_4,
		fmrx_s5_5_6,	0
};

static const uint8_t fontmid_7[] = {			//  7
	3,	fmrx_m15_1,
	2,	fmrx_s12_3_1,
	2,	fmrx_s11_3_2,
	2,	fmrx_s10_3_3,
	2,	fmrx_s9_3_4,
	2,	fmrx_s8_3_5,
	2,	fmrx_s7_3_6,
	3,	fmrx_s6_3_7,
	3,	fmrx_s5_3_8,
	3,	fmrx_s4_3_9,	0
};

static const uint8_t fontmid_8[] = {			//  8
		fmrx_s5_5_6,
		fmrx_s3_9_4,
		fmrx_s2_11_3,
		fmrx_s1_4_5_4_2,
		fmrx_s1_3_7_3_2,
	3,	fmrx_m3_9_3_1,
		fmrx_s1_3_7_3_2,
		fmrx_s1_4_5_4_2,
		fmrx_s2_11_3,
		fmrx_s3_9_4,		// --
		fmrx_s2_11_3,
		fmrx_s1_4_5_4_2,
		fmrx_s1_3_7_3_2,
	4,	fmrx_m3_9_3_1,
		fmrx_s1_3_7_3_2,
		fmrx_s1_4_5_4_2,
		fmrx_s2_11_3,
		fmrx_s3_9_4,
		fmrx_s5_5_6,	0
};

static const uint8_t fontmid_9[] = {			//  9
		fmrx_s5_5_6,
		fmrx_s3_9_4,
		fmrx_s2_11_3,
		fmrx_s1_4_5_4_2,
		fmrx_s1_3_7_3_2,
	5,	fmrx_m3_9_3_1,
		fmrx_s1_3_7_4_1,
		fmrx_s1_4_5_5_1,
		fmrx_s2_13_1,
		fmrx_s3_8_1_3_1,
		fmrx_s5_5_2_3_1,
	4,	fmrx_s12_3_1,
		fmrx_s1_3_7_3_2,
		fmrx_s1_4_5_4_2,
		fmrx_s2_11_3,
		fmrx_s3_9_4,
		fmrx_s5_5_6,	0
};

static const uint8_t fontmid_A[] = {
		fmrx_s7_1_8,
	3,	fmrx_s6_3_7,
	3,	fmrx_s5_5_6,
	3,	fmrx_s4_3_1_3_5,
	3,	fmrx_s3_3_3_3_4,
	3,	fmrx_s2_3_5_3_3,
		fmrx_s1_3_7_3_2,
	2,	fmrx_s1_13_2,
		fmrx_m15_1,
	4,	fmrx_m3_9_3_1,	0
};

static const uint8_t fontmid_B[] = {
		fmrx_m10_6,
		fmrx_m12_4,
		fmrx_m13_3,
		fmrx_m3_7_4_2,
		fmrx_m3_8_3_2,
	3,	fmrx_m3_9_3_1,
		fmrx_m3_8_3_2,
		fmrx_m3_7_4_2,
		fmrx_m13_3,
		fmrx_m12_4,
		fmrx_m13_3,
		fmrx_m3_7_4_2,
		fmrx_m3_8_3_2,
	4,	fmrx_m3_9_3_1,
		fmrx_m3_8_3_2,
		fmrx_m3_7_4_2,
		fmrx_m13_3,
		fmrx_m12_4,
		fmrx_m10_6,		0
};

static const uint8_t fontmid_E[] = {			//  E
	3,	fmrx_m15_1,
	6,	fmrx_m3_13,
	3,	fmrx_m12_4,
	9,	fmrx_m3_13,
	3,	fmrx_m15_1,		0
};

static const uint8_t fontmid_G[] = {
		fmrx_s5_5_6,
		fmrx_s3_9_4,
		fmrx_s2_11_3,
		fmrx_s1_4_5_4_2,
		fmrx_s1_3_7_3_2,
		fmrx_m4_12,
	8,	fmrx_m3_13,
	3,	fmrx_m3_6_6_1,
		fmrx_m3_9_3_1,
		fmrx_m4_8_3_1,
		fmrx_s1_3_7_3_2,
		fmrx_s1_4_5_4_2,
		fmrx_s2_11_3,
		fmrx_s3_9_4,
		fmrx_s5_5_6,	0
};

static const uint8_t fontmid_H[] = {
	9,	fmrx_m3_9_3_1,
	3,	fmrx_m15_1,
	12,	fmrx_m3_9_3_1,	0
};

static const uint8_t fontmid_M[] = {
	2,	fmrx_m3_9_3_1,
	2,	fmrx_m4_7_4_1,
	2,	fmrx_m5_5_5_1,
	2,	fmrx_m6_3_6_1,
	2,	fmrx_m3_1_3_1_3_1_3_1,
	2,	fmrx_m3_2_5_2_3_1,
	2,	fmrx_m3_3_3_3_3_1,
		fmrx_m3_4_1_4_3_1,
	9,	fmrx_m3_9_3_1,	0
};

static const uint8_t fontmid_V[] = {
	4,	fmrx_m3_9_3_1,
		fmrx_m4_7_4_1,
	3,	fmrx_s1_3_7_3_2,
	3,	fmrx_s2_3_5_3_3,
	3,	fmrx_s3_3_3_3_4,
	3,	fmrx_s4_3_1_3_5,
	3,	fmrx_s5_5_6,
	3,	fmrx_s6_3_7,
		fmrx_s7_1_8,	0
};

static const uint8_t fontmid_b[] = {
	7,	fmrx_m3_13,
		fmrx_m3_1_5_7,
	2,	fmrx_m11_5,
		fmrx_m4_4_4_4,
	9,	fmrx_m3_6_3_4,
		fmrx_m4_4_4_4,
	2,	fmrx_m11_5,
		fmrx_m3_1_5_7,	0
};

static const uint8_t fontmid_d[] = {
	7,	fmrx_s9_3_4,
		fmrx_s3_5_1_3_4,
	2,	fmrx_s1_11_4,
		fmrx_m4_4_4_4,
	9,	fmrx_m3_6_3_4,
		fmrx_m4_4_4_4,
	2,	fmrx_s1_11_4,
		fmrx_s3_5_1_3_4,	0
};

static const uint8_t fontmid_k[] = {
	5,	fmrx_m3_13,
		fmrx_m3_5_3_5,
		fmrx_m3_4_4_5,
		fmrx_m3_3_4_6,
		fmrx_m3_2_4_7,
		fmrx_m3_1_4_8,
		fmrx_m7_9,
		fmrx_m6_10,
		fmrx_m7_9,
		fmrx_m3_1_3_9,
	2,	fmrx_m3_2_3_8,
	2,	fmrx_m3_3_3_7,
	2,	fmrx_m3_4_3_6,
	2,	fmrx_m3_5_3_5,
	2,	fmrx_m3_6_3_4,	0
};

static const uint8_t fontmid_m[] = {
	7,	fmrx_s16,
		fmrx_m3_1_3_2_3_4,
	2,	fmrx_m14_2,
		fmrx_m4_1_5_1_4_1,
	13,	fmrx_m3_3_3_3_3_1,	0
};

static const uint8_t fontmid_n[] = {
	7,	fmrx_s16,
		fmrx_m3_1_5_7,
	2,	fmrx_m11_5,
		fmrx_m4_4_4_4,
	13,	fmrx_m3_6_3_4,		0
};

static const uint8_t fontmid_s[] = {
	7,	fmrx_s16,
		fmrx_s3_6_7,
		fmrx_s1_10_5,
		fmrx_s1_11_4,
		fmrx_m4_5_3_4,
	2,	fmrx_m3_13,
		fmrx_m4_12,
		fmrx_s1_8_7,
		fmrx_s1_10_5,
		fmrx_s3_8_5,
		fmrx_s8_4_4,
	2,	fmrx_s9_3_4,
		fmrx_m3_5_4_4,
		fmrx_m11_5,
		fmrx_s1_10_5,
		fmrx_s3_6_7,	0
};

static const uint8_t fontmid_u[] = {
	7,	fmrx_s16,
	13,	fmrx_m3_6_3_4,
		fmrx_m4_4_4_4,
	2,	fmrx_s1_11_4,
		fmrx_s3_5_1_3_4,	0
};

static const uint8_t fontmid_z[] = {
	7,	fmrx_s16,
	3,	fmrx_m12_4,
		fmrx_s9_3_4,
		fmrx_s8_4_4,
		fmrx_s7_4_5,
		fmrx_s6_4_6,
		fmrx_s5_4_7,
		fmrx_s4_4_8,
		fmrx_s3_4_9,
		fmrx_s2_4_10,
		fmrx_s1_4_11,
		fmrx_m4_12,
		fmrx_m3_13,
	3,	fmrx_m12_4,			0
};


static const uint8_t fontmid_degree[] = {		//  °
		fmrx_s6_3_7,
		fmrx_s4_7_5,
		fmrx_s3_9_4,
		fmrx_s3_3_3_3_4,
	3,	fmrx_s2_3_5_3_3,
		fmrx_s3_3_3_3_4,
		fmrx_s3_9_4,
		fmrx_s4_7_5,
		fmrx_s6_3_7,
	13,	fmrx_s16,		0
};


/*!*****************************************************************************
  @brief	Font Middle-size Array of rowr select
  @param	symbol - character: A, B, G, H, M, V, b, d, k, m, n, s, u, z
  @return	pointer to array of rows
  @note
  */
const uint8_t *FontMidArray_select(char symbol)
{
	const uint8_t *array;
	switch (symbol)
	{
	case '%':
		array = fontmid_percent;
		break;
	case '*':
		array = fontmid_asterisk;
		break;
	case '+':
		array = fontmid_plus;
		break;
	case '-':
		array = fontmid_minus;
		break;

	case '0':
		array = fontmid_0;
		break;
	case '1':
		array = fontmid_1;
		break;
	case '2':
		array = fontmid_2;
		break;
	case '3':
		array = fontmid_3;
		break;
	case '4':
		array = fontmid_4;
		break;
	case '5':
		array = fontmid_5;
		break;
	case '6':
		array = fontmid_6;
		break;
	case '7':
		array = fontmid_7;
		break;
	case '8':
		array = fontmid_8;
		break;
	case '9':
		array = fontmid_9;
		break;

	case 'A':
		array = fontmid_A;
		break;
	case 'B':
		array = fontmid_B;
		break;
	case 'E':
		array = fontmid_E;
		break;
	case 'G':
		array = fontmid_G;
		break;
	case 'H':
		array = fontmid_H;
		break;
	case 'M':
		array = fontmid_M;
		break;
	case 'V':
		array = fontmid_V;
		break;
	case 'b':
		array = fontmid_b;
		break;
	case 'd':
		array = fontmid_d;
		break;
	case 'k':
		array = fontmid_k;
		break;
	case 'm':
		array = fontmid_m;
		break;
	case 'n':
		array = fontmid_n;
		break;
	case 's':
		array = fontmid_s;
		break;
	case 'u':
		array = fontmid_u;
		break;
	case 'z':
		array = fontmid_z;
		break;
	case '$':
		array = fontmid_degree;
		break;
	default:
		array = fontmid_space;
		break;
	}
	return array;
}


/*!*****************************************************************************
	@brief    Mid Symbol print without background padding
	@param    symbol - character
	@param    display - pointer to top left corner in LCD buffer memory
	@param    foreground - palette for foreground (lower byte)
	@return   width of symbol in pixels (for proportional string printing)
*/
uint32_t FontMid::WriteSymbol(uint8_t symbol, int x, int y)
{
	const uint8_t *index = FontMidArray_select((char)symbol);	// first row index

	uint32_t width = 0;
	if (symbol == ' ')
	{
		width = FONTMID_SPACEWIDTH;			// space width
	}

	uint32_t same = 0;									// number of the same rows
	uint32_t row;										// row of pixels
	uint32_t code;

	while (true)
	{
		code = *index++;
		if (code == 0)
		{
			break;
		}
		if (code >= FONTMID_STARTINDEX)
		{
			if (same == 0)
			{
				same = 1;						// new row -> one row
			}
			row = FontMidRow[code - FONTMID_STARTINDEX];
		}
		else
		{
			same = code;								// multi-rows
			continue;
		}

		for (; same; same--)
		{
			uint32_t rowshift = row;					// row for shift
			for (uint32_t i = 0; i < 16; i++)
			{
				uint32_t pixel = rowshift & 0x8000;
				if (pixel != 0)
				{
					Point().Draw((int)(x + i), y);
				}
				if ((pixel != 0) && i > width)
				{
					width = i;
				}
				rowshift <<= 1;
				if (rowshift == 0)
				{
					break;
				}
			}
			y++;
		}
	}
	return width + FONTMID_GAPWIDTH;
}


/*!*****************************************************************************
	@brief   Mid Text String Proportional space print without Background Padding
	@param   text - pointer to text string
	@param   display - pointer to top left corner in LCD buffer memory
	@param   foreground - palette for foreground (lower byte)
	@return
*/
void FontMid::Write(char *text, int x, int y)
{
	while (*text)
	{
		uint32_t space = WriteSymbol((uint8)*text, x, y);
		x += space;
		text++;
	}
}
