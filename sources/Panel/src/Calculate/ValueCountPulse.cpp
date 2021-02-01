#include "defines.h"
#include "Calculate/ValueCountPulse.h"
#include "Menu/Pages/Modes/Modes.h"


ValueCountPulse::ValueCountPulse(uint counter) : ValueFPGA()
{
    mainUnits.Set(TypeConversionString::None, "");

    counter /= 2;

    ValueSTRICT val(counter);

    if (ModeCountPulse::Current().IsFromPeriod())
    {
        val.DivINT(NumberPeriods::Current().ToAbs());
    }

    SetValue(val, counter);
}


char *ValueCountPulse::GetSuffixUnit(int order) const
{
    return GetSuffixUnitRelated(order);
}
