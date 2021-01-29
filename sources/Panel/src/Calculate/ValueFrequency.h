#pragma once
#include "Calculate/ValueFPGA.h"
#include "Utils/ValueSTRICT.h"


struct ValueFrequency : public ValueFPGA
{
    ValueFrequency();

    void SetValue(ValueSTRICT value, uint counter);
};


struct ValueFrequency_Frequency : public ValueFrequency
{
    ValueFrequency_Frequency(uint counter1);
};


struct ValueFrequency_T_1 : public ValueFrequency
{
    ValueFrequency_T_1(uint counter1);
};


struct ValueFrequency_Ratio : public ValueFrequency
{
    ValueFrequency_Ratio(uint counter1, uint counter2);
};


struct ValueFrequency_Tachometer : public ValueFrequency
{
    ValueFrequency_Tachometer(uint counter1);
};


struct ValueFrequency_Comparator : public ValueFrequency
{
    ValueFrequency_Comparator(uint counter1);
};
