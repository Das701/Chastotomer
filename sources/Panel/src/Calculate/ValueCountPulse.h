#pragma once
#include "Calculate/ValueFPGA.h"


struct ValueCountPulse : public ValueFPGA
{
    ValueCountPulse(uint counter);

protected:

    virtual char *GetSuffixUnit(int order) const;
};

