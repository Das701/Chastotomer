#include "defines.h"
#include "Calculate/ValueFrequency.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Utils/StringUtils.h"
#include "Utils/ValueSTRICT.h"
#include <cmath>
#include <cstdio>
#include <cstring>


ValueFrequency::ValueFrequency() :
    ValueFPGA()
{
    mainUnits.Set(TypeConversionString::None, "Hz");
}


ValueFrequency_Frequency::ValueFrequency_Frequency(uint counter) : ValueFrequency()
{
    counter /= 2;

    TimeMeasure::E time = (TimeMeasure::E)Channel::Current()->mod.timeMeasure.value;

    uint multipliers[TimeMeasure::Count] =
    {
        1000,   // 1 ms
        100,    // 10 ms
        10,     // 100 ms
        1,      // 1 s
        1,      // 10 s
        1,      // 100 s
        1       // 1000 s
    };

    uint dividers[TimeMeasure::Count] =
    {
        1,      // 1ms
        1,      // 10 ms
        1,      // 100 ms
        1,      // 1 s
        10,     // 10 s
        100,    // 100 s
        1000    // 1000 s
    };

    ValueSTRICT strict((int64)counter);

    strict.MulUINT(multipliers[time]);
    strict.DivUINT(dividers[time]);

    if (CURRENT_CHANNEL_IS_C || CURRENT_CHANNEL_IS_D)
    {
        strict.MulUINT(64);
    }

    SetValue(strict, counter);
}


ValueFrequency_T_1::ValueFrequency_T_1(uint counter) : ValueFrequency()
{
    counter /= 2;

    ValueSTRICT strict((int64)Channel::Current()->mod.numberPeriods.ToAbs());

    strict.MulUINT((uint)Channel::Current()->mod.timeLabels.ToZeros());

    strict.DivUINT(counter);

    SetValue(strict, counter);
}


void ValueFrequency::SetValue(ValueSTRICT strict, uint counter)
{
    int order = 0;

    if (strict.ToDouble() < 0.0)
    {
        while (strict.ToUnits(Order::Milli) < 1)
        {
            strict.MulUINT(1000);
            order -= 3;
        }
    }
    else
    {
        while (strict.ToDouble() >= 1000.0)
        {
            strict.DivUINT(1000);
            order += 3;
        }
    }

    if (!strict.IsZero())
    {
        while ((uint)strict.ToDouble() < 1)
        {
            strict.MulUINT(1000);
            order -= 3;
        }

        if (NumDigitsInNumber((uint)strict.ToDouble()) > NumDigitsInNumber(counter))
        {
            strict.DivUINT(1000);
            order += 3;
        }
    }

    char buffer[30];
    std::sprintf(buffer, "%10.10f", strict.ToDouble());

    if (strict.IsZero())
    {
        SU::LeaveFewDigits(buffer, 29, NumDigitsInNumber(counter));
    }
    else
    {
        SU::LeaveFewSignedDigits(buffer, 29, NumDigitsInNumber(counter));
    }

    std::strcat(buffer, " ");
    std::strcat(buffer, GetSuffixUnit(order));
    std::strcat(buffer, mainUnits.c_str());

    value.Set(TypeConversionString::None, buffer);
}
