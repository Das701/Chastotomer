#include "defines.h"
#include "Calculate/MathFPGA.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstdio>


ValueDuration::ValueDuration() : ValueFPGA()
{
    mainUnits.Set(TypeConversionString::None, "s");
}


ValueDuration_Ndt_1ns::ValueDuration_Ndt_1ns(uint timer, uint cal1, uint cal2) : ValueDuration()
{
    float v = (float)(100 * timer) / (float)(cal2 - cal1);

    MathFPGA::Data::SetDigits(String("%10.1f", v));

    MathFPGA::Data::SetUnits(String(" ns"));
}


ValueDuration_Phase_FillFactor::ValueDuration_Phase_FillFactor(uint period, uint duration) : ValueDuration()
{
    mainUnits.Set(TypeConversionString::None, "");

    if (period == 0)
    {
        SetInfiniteValue();
        return;
    }

    ValueSTRICT val(duration);
    val.DivUINT(period);

    if (ModeDuration::Current().IsPhase())
    {
        val.MulUINT(360);

        if (period == duration)
        {
            val.FromINT64(0);
        }
    }
    else
    {
        if (val.ToDouble() != 0.0)
        {
            while (val.ToUnits(Order::Milli) < 1000)
            {
                val.MulUINT(10);
            }
        }
    }

    SetValue(val, period);
}


ValueDuration_Ndt_StartStop::ValueDuration_Ndt_StartStop(uint counter) : ValueDuration()
{
    counter /= 2;

    ValueSTRICT val(counter);

    val.DivINT(PeriodTimeLabels::Current().ToZeros());

    SetValue(val, counter);
}


char *ValueDuration_Phase_FillFactor::GetSuffixUnit(int order) const
{
    return GetSuffixUnitRelated(order);
}
