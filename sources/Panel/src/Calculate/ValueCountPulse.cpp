#include "defines.h"
#include "Calculate/ValueFPGA.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Menu/Pages/Modes/Modes.h"


ValueCountPulse::ValueCountPulse(uint counter) : ValueFPGA()
{
    mainUnits.Set("");

    counter /= 2;

    ValueSTRICT val(counter);

    if (CURRENT_CHANNEL_IS_C)
    {
        val.MulUINT(64);
    }

    if (ModeCountPulse::Current().IsFromPeriod())
    {
        val.DivINT(NumberPeriods::Current().ToAbs());
    }

    SetValue("%d", (uint)val.ToDouble());
}
