#pragma once
#include "Calculate/ValueFPGA.h"


struct ValueCountPulse : public ValueFPGA
{
    ValueCountPulse(uint counter1, uint counter2 = 0, uint counter3 = 0, uint counter4 = 0);
};

