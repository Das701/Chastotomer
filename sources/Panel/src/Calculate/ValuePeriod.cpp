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
