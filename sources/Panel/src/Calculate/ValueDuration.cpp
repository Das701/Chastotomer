#include "defines.h"
#include "Calculate/MathFPGA.h"
#include "Calculate/ValueDuration.h"
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


ValueDuration_Phase_FillFactor::ValueDuration_Phase_FillFactor(uint period, uint duration) : ValueDuration()
{
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

    if (ModeDuration::Current().IsPhase())
    {
        if (Math::InBound((float)val.ToDouble(), 0.0F, 360.0F))
        {
            MathFPGA::Data::SetDigits(String("%10.3f", val.ToDouble()));
        }
        else
        {
            MathFPGA::Data::SetDigits(String(" "));
        }

        MathFPGA::Data::SetUnits(String(" $"));
    }
    else
    {
        char buffer[30];
        std::sprintf(buffer, "%10.10f", val.ToDouble());
        SU::LeaveFewDigits(buffer, 30, powDataA - 1);
        MathFPGA::Data::SetDigits(String(buffer));

        String result(" E-0");
        result[3] = (char)(zeroes | 0x30);
        zeroes = 0;
        MathFPGA::Data::SetUnits(result);
    }
}
