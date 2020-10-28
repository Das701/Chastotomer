#pragma once

#define __CC_ARM

#ifndef __IO
#define __IO
#endif

#include <stdint.h>

#undef __ASM
#define __ASM()


struct SysTickStruct
{
    unsigned short CTRL;
};

#define SysTick    ((SysTickStruct *)0)

#define NVIC_SetPriority(x, y)


extern struct structSCB
{
    unsigned VTOR;
} SCB;


#define CoreDebug_DEMCR_TRCENA_Msk         1
#define DWT_CTRL_CYCCNTENA_Msk             1       /*!< DWT CTRL: CYCCNTENA Mask */


typedef struct
{
  uint32_t CTRL;                   /*!< Offset: 0x000 (R/W)  Control Register */
  uint32_t CYCCNT;                 /*!< Offset: 0x004 (R/W)  Cycle Count Register */
  uint32_t CPICNT;                 /*!< Offset: 0x008 (R/W)  CPI Count Register */
  uint32_t EXCCNT;                 /*!< Offset: 0x00C (R/W)  Exception Overhead Count Register */
  uint32_t SLEEPCNT;               /*!< Offset: 0x010 (R/W)  Sleep Count Register */
  uint32_t LSUCNT;                 /*!< Offset: 0x014 (R/W)  LSU Count Register */
  uint32_t FOLDCNT;                /*!< Offset: 0x018 (R/W)  Folded-instruction Count Register */
  uint32_t PCSR;                   /*!< Offset: 0x01C (R/ )  Program Counter Sample Register */
  uint32_t COMP0;                  /*!< Offset: 0x020 (R/W)  Comparator Register 0 */
  uint32_t MASK0;                  /*!< Offset: 0x024 (R/W)  Mask Register 0 */
  uint32_t FUNCTION0;              /*!< Offset: 0x028 (R/W)  Function Register 0 */
  uint32_t RESERVED0[1U];
  uint32_t COMP1;                  /*!< Offset: 0x030 (R/W)  Comparator Register 1 */
  uint32_t MASK1;                  /*!< Offset: 0x034 (R/W)  Mask Register 1 */
  uint32_t FUNCTION1;              /*!< Offset: 0x038 (R/W)  Function Register 1 */
  uint32_t RESERVED1[1U];
  uint32_t COMP2;                  /*!< Offset: 0x040 (R/W)  Comparator Register 2 */
  uint32_t MASK2;                  /*!< Offset: 0x044 (R/W)  Mask Register 2 */
  uint32_t FUNCTION2;              /*!< Offset: 0x048 (R/W)  Function Register 2 */
  uint32_t RESERVED2[1U];
  uint32_t COMP3;                  /*!< Offset: 0x050 (R/W)  Comparator Register 3 */
  uint32_t MASK3;                  /*!< Offset: 0x054 (R/W)  Mask Register 3 */
  uint32_t FUNCTION3;              /*!< Offset: 0x058 (R/W)  Function Register 3 */
} DWT_Type;


#define DWT ((DWT_Type *) 0)
