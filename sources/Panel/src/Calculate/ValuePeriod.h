#pragma once
#include "Calculate/ValueFPGA.h"


struct ValuePeriod : public ValueFPGA
{
    ValuePeriod(uint counter1, uint counter2 = 0, uint counter3 = 0, uint counter4 = 0);
};


struct ValuePeriod_Period : public ValuePeriod
{
    ValuePeriod_Period(uint counter1);
};


struct ValuePeriod_F_1 : public ValuePeriod
{
    ValuePeriod_F_1(uint counter1);
};
