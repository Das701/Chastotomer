#include "defines.h"
#include "Calculate/MathFPGA.h"
#include "Calculate/ValueDuration.h"


ValueDuration::ValueDuration() : ValueFPGA()
{
    mainUnits.Set(TypeConversionString::None, "s");
}


ValueDuration_Ndt_1ns::ValueDuration_Ndt_1ns(uint timer, uint cal1, uint cal2) : ValueDuration()
{
    float v = (float)(100.0 * timer) / (float)(cal2 - cal1);

    MathFPGA::Data::SetDigits(String("%10.2f", v));

    MathFPGA::Data::SetUnits(String(" ns"));
}
