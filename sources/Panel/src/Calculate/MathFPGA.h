#pragma once
#include "Calculate/ValuesFPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Stack.h"
#include "Utils/String.h"
#include "Utils/ValueSTRICT.h"


struct MathFPGA
{
    struct Auto
    {
        friend struct FPGA;

        static int Mid();
        static int Min();
        static int Max();

        static String Give();

        static void Refresh();

        static int NA; //-V707
        static int NB; //-V707

    private:
        static uint fpgaMin;
        static uint fpgaMid;
        static uint fpgaMax;
    };
};
