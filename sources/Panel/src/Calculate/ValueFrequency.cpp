#include "defines.h"
#include "Calculate/ValueFrequency.h"


ValueFrequency::ValueFrequency(uint value1, uint value2, uint value3, uint value4) :
    ValueFPGA(value1, value2, value3, value4)
{

}


ValueFrequency_Frequency::ValueFrequency_Frequency(uint counter1) : ValueFrequency(counter1)
{

}
