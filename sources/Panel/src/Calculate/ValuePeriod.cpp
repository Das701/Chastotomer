#include "defines.h"
#include "Calculate/MathFPGA.h"
#include "Calculate/ValuePeriod.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Utils/ValueSTRICT.h"


ValuePeriod::ValuePeriod() : ValueFPGA()
{
    mainUnits.Set(TypeConversionString::None, "s");
}


ValuePeriod_Period::ValuePeriod_Period(uint counter) : ValuePeriod()
{
    counter /= 2;

    ValueSTRICT strict(counter);

    strict.DivINT(Channel::Current()->mod.numberPeriods.ToAbs());
    strict.DivINT(Channel::Current()->mod.timeLabels.ToZeros());

    SetValue(strict, counter);
}


ValuePeriod_F_1::ValuePeriod_F_1(uint counter) : ValuePeriod()
{
    counter /= 2;

    if (counter == 100001)
    {
        counter = counter;
    }

    if (counter == 0)
    {
        value.Set(TypeConversionString::None, MathFPGA::Data::UGO_DivNULL);
        return;
    }

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

    ValueSTRICT strict(dividers[time]);

    double d = strict.ToDouble();

    strict.DivUINT(counter);
    strict.DivUINT(multipliers[time]);

    SetValue(strict, counter, 1);
}
