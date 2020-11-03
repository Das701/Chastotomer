#pragma once

typedef unsigned short         uint16;
typedef signed short           int16;
typedef unsigned char          uint8;
typedef signed char            int8;
typedef unsigned long long int uint64;
typedef unsigned int           uint;
typedef const char * const     pString;
typedef const char *           pCHAR;

typedef void(*pFuncVII)(int, int);


#ifndef WIN32
#define nullptr 0 //-V1059
#endif

#define _bitset(bits)                               \
  ((uint8)(                                         \
  (((uint8)((uint)bits / 01)        % 010) << 0) |  \
  (((uint8)((uint)bits / 010)       % 010) << 1) |  \
  (((uint8)((uint)bits / 0100)      % 010) << 2) |  \
  (((uint8)((uint)bits / 01000)     % 010) << 3) |  \
  (((uint8)((uint)bits / 010000)    % 010) << 4) |  \
  (((uint8)((uint)bits / 0100000)   % 010) << 5) |  \
  (((uint8)((uint)bits / 01000000)  % 010) << 6) |  \
  (((uint8)((uint)bits / 010000000) % 010) << 7)))


#define BIN_U8(bits) (_bitset(0##bits))

#define BIN_U16(bits1, bits0) (BIN_U8(bits1) << 8 | BIN_U8(bits0))

#define BIN_U32(bits3, bits2, bits1, bits0) (BIN_U8(bits3) << 24 | BIN_U8(bits2) << 16 | BIN_U8(bits1) << 8 | BIN_U8(bits0))

#define HEX_FROM_2(hex1, hex0) ((uint)(0x##hex1) << 16 | (uint)0x##hex0)

#define _SET_BIT_VALUE(value, numBit, bitValue) ((value) |= ((bitValue) << (numBit)))
#define _GET_BIT(value, numBit) (((value) >> (numBit)) & 1)
#define _CLEAR_BIT(value, bit) ((value) &= (~(1 << (bit))))
#define _SET_BIT(value, bit) ((value) |= (1 << (bit)))
