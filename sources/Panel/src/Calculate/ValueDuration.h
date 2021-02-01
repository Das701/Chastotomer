#pragma once
#include "Calculate/ValueFPGA.h"


struct ValueDuration : public ValueFPGA
{
    ValueDuration();
};


struct ValueDuration_Ndt_StartStop : public ValueDuration
{
    ValueDuration_Ndt_StartStop(uint counter);
};


struct ValueDuration_Ndt_1ns : public ValueDuration
{
    ValueDuration_Ndt_1ns(uint timer, uint cal1, uint cal2);
};


struct ValueDuration_Phase_FillFactor : public ValueDuration
{
    ValueDuration_Phase_FillFactor(uint period, uint duration);

protected:

    virtual char *GetSuffixUnit(int order) const;
};
