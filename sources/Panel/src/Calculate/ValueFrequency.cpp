#include "defines.h"
#include "Log.h"
#include "Calculate/MathFPGA.h"
#include "Calculate/ValueFrequency.h"
#include "Display/Display.h"
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


ValueFrequency_Tachometer::ValueFrequency_Tachometer(uint counter) : ValueFrequency()
{
    counter /= 2;

    MathFPGA::Data::SetDigits(String("%d", counter));

    MathFPGA::Data::SetUnits(String(""));
}


ValueFrequency_Comparator::ValueFrequency_Comparator(uint counter, int interpol1, int cal1, int interpol2, int cal2) : ValueFrequency()
{
    /*
*   A = (N - counter) / N - dx / N;
*   A = (N - conter - dx) / N
*   dx = (interpol1 / cal1 - interpol2 / cal2) / 2
*/

    if (cal1 != 0 && cal2 != 0)
    {
        if ((interpol1 & (1U << 15)) != 0)
        {
            interpol1 -= 65536;
        }

        if ((interpol2 & (1U << 15)) != 0)
        {
            interpol2 -= 65536;
        }

        uint N = 5000000U;

        if (Channel::Current()->mod.timeComparator.Is_10s())
        {
            N *= 10;
        }

        ValueComparator k1 = ValueComparator(interpol1) / (uint)cal1;

        ValueComparator k2 = ValueComparator(interpol2) / (uint)cal2;

        ValueComparator dx = (k1 - k2) / 2;

        ValueComparator A((int)N - (int)counter);
        A.Sub(dx);
        A.Div(N);

        A.Mul(1000000);     // Это приводим к своей выводимой степени

        if (!Channel::Current()->mod.timeComparator.Is_1s())
        {
            A.Mul(10);
        }

        A.SetSign(1);

        if (MathFPGA::Comparator::values.AppendValue(A.ToDouble()))
        {
            Display::Refresh();
        }

        MathFPGA::Data::SetDigits(A.ToString());

        if (Channel::Current()->mod.timeComparator.Is_1s())
        {
            MathFPGA::Data::SetUnits(String("E-6"));
        }
        else
        {
            MathFPGA::Data::SetUnits(String("E-7"));
        }
    }
}


ValueFrequency_Ratio::ValueFrequency_Ratio(uint counter1, uint counter2)
{
    LOG_WRITE("%d %d", counter1, counter2);

    mainUnits.Set(TypeConversionString::None, "");

    const ModeFrequency &mode = ModeFrequency::Current();

    counter1 /= 2;

    ValueSTRICT valueA(counter1);
    ValueSTRICT valueB((int64)0);

    if (mode.IsRatioAB() || mode.IsRatioBA())
    {

    }
    else if ((mode.IsRatioAC() || mode.IsRatioBC()) && Relation::IsEnabled())
    {
        valueA.DivUINT(counter2);
        valueA.DivUINT(32);
    }
    else if (mode.IsRatioCA() || mode.IsRatioCB())
    {
        valueA.MulUINT(100);
    }

    switch (mode.value)
    {
    case ModeFrequency::RatioAB:
        valueA.DivINT(NumberPeriods::Current().ToAbs());
        break;
    }

    SetValue(valueA, counter1);
}


char *ValueFrequency_Ratio::GetSuffixUnit(int order) const
{
    return GetSuffixUnitRelated(order);
}
