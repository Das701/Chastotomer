#pragma once
#include "Calculate/ValueFPGA.h"


struct ValueDuration : public ValueFPGA
{
    ValueDuration(uint counter1, uint counter2 = 0, uint counter3 = 0, uint counter4 = 0);
};
