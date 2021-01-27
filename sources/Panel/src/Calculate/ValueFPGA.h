#pragma once
#include "Utils/String.h"


struct ValueFPGA
{
    ValueFPGA(uint counter1, uint counter2 = 0, uint counter3 = 0, uint counter4 = 0);

    String value;
    String units;
};
