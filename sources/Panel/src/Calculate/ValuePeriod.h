#pragma once
#include "Calculate/ValueFPGA.h"


struct ValuePeriod : public ValueFPGA
{
    ValuePeriod();
};


struct ValuePeriod_Period : public ValuePeriod
{
    ValuePeriod_Period(uint counter1);
};


struct ValuePeriod_F_1 : public ValuePeriod
{
    ValuePeriod_F_1(uint counter1);
};
