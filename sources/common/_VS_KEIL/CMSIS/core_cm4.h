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
