#include "defines.h"
#include "Calculate/MathFPGA.h"
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

    if (counter == 0)
    {
        value.Set(TypeConversionString::None, MathFPGA::Data::UGO_DivNULL);
        return;
    }

    ValueSTRICT strict(Channel::Current()->mod.numberPeriods.ToAbs());

    strict.MulINT(Channel::Current()->mod.timeLabels.ToZeros());

    strict.DivUINT(counter);

    SetValue(strict, counter);
}
