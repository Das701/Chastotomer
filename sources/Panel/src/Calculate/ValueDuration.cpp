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
    float v = (float)(100.0 * timer) / (float)(cal2 - cal1);

    MathFPGA::Data::SetDigits(String("%10.2f", v));

    MathFPGA::Data::SetUnits(String(" ns"));
}


ValueDuration_Phase_FillFactor::ValueDuration_Phase_FillFactor(uint counter, uint duration) : ValueDuration()
{
    mainUnits.Set(TypeConversionString::None, "");

    uint period = counter;

    if (period == 0 && duration == 0)
        period = 1;


    ValueSTRICT val((int64)duration);
    val.DivUINT(period);

    int powDataA = 1;
    int zeroes = 0;

    do
    {
        powDataA++;
        period /= 10;

    } while (period > 0);

    if (ModeDuration::Current().IsPhase())
    {
        val.MulUINT(360);

        if (val.ToDouble() == 360.0)
        {
            val = ValueSTRICT((int64)0);
        }
    }
    else
    {
        if (val.Sign() < 0)
        {
            val.SetSign(1);
        }

        if (val.ToDouble() != 0.0)
        {
            while (val.ToUnits(Order::Milli) < 1000)
            {
                val.MulUINT(10);
                zeroes++;
            }
        }
    }

    SetValue(val, counter);
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
