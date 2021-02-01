#pragma once
#include "Calculate/ValueFPGA.h"


struct ValueDuration : public ValueFPGA
{
    ValueDuration();
};


struct ValueDuration_Ndt_1ns : public ValueDuration
{
    ValueDuration_Ndt_1ns(uint timer, uint cal1, uint cal2);
};
