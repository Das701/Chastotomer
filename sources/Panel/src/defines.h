#pragma once
#include "mytypes.h"

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

struct Flags_ 
{
  unsigned Key_Event:1;
  unsigned ENC_Event:1;
  unsigned Control_Event:1;
  unsigned RefrCreen:1;
  unsigned FunKey_Event:1;
  unsigned Change_Event:1;			//Флаг нажатия новой клавиши
  unsigned RefrCreen_2:1;
  unsigned DataReceive:1;
  unsigned Test:1;
  unsigned Calibrate:1;
  unsigned Auto:1;
  unsigned Hold:1;
  unsigned Processing:1;
  unsigned CNTNumb:1;
  unsigned ReceiveFlag:1;
  unsigned TransceiveFlag:1;
  unsigned SoftStart:1;
};

extern struct Flags_ Flags;

extern char ModeScreen;
extern char ModeScreenPre;

//Режимы работы
#define CALIBRATE_MODE	0
#define MEASURE_SCREEN	1
#define CHANNEL_PREST	2
