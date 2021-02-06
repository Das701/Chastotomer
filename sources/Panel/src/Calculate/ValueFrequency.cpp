#include "defines.h"
#include "Log.h"
#include "Calculate/ValueFPGA.h"
#include "Display/Display.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Utils/StringUtils.h"
#include "Utils/ValueSTRICT.h"
#include <cmath>
#include <cstdio>
#include <cstring>


ValueFrequency_Comparator::Stack ValueFrequency_Comparator::values(400);


ValueFrequency::ValueFrequency() :
    ValueFPGA()
{
    mainUnits.Set("Hz");
}


ValueFrequency_Frequency::ValueFrequency_Frequency(uint counter) : ValueFrequency()
{
    counter /= 2;

    if (counter == 0)
    {
        CalculateZero();

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

    ValueSTRICT strict((int64)counter);

    strict.MulUINT(multipliers[time]);

    strict.DivUINT(dividers[time]);

    if (CURRENT_CHANNEL_IS_C || CURRENT_CHANNEL_IS_D)
    {
        strict.MulUINT(64);
    }

    SetValue(strict, counter);
}


void ValueFrequency_Frequency::CalculateZero()
{
    ValueFrequency_Frequency zero(2);

    String zeroString(ValueFPGA::GiveStringValue());

    char *end = &zeroString[zeroString.Size()];
    char *start = &zeroString[0];

    for (char *symbol = end - 1; symbol >= start; symbol--)
    {
        if (SU::IsDigit(*symbol))
        {
            *symbol = '0';
            break;
        }
    }

    SetValue(zeroString.c_str());
}


ValueFrequency_T_1::ValueFrequency_T_1(uint counter) : ValueFrequency()
{
    counter /= 2;

    if (counter == 0)
    {
        SetInfiniteValue();
        return;
    }

    ValueSTRICT strict(Channel::Current()->mod.numberPeriods.ToAbs());

    strict.MulINT(Channel::Current()->mod.timeLabels.ToZeros());

    strict.DivUINT(counter);

    SetValue(strict, counter);
}


ValueFrequency_Tachometer::ValueFrequency_Tachometer(uint counter) : ValueFrequency()
{
    SetValue("%d", counter / 2);
}


ValueFrequency_Comparator::ValueFrequency_Comparator(uint counter, int interpol1, int cal1, int interpol2, int cal2) :
    ValueFrequency()
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

        ValueSTRICT k1(interpol1);
        k1.DivINT(cal1);

        ValueSTRICT k2(interpol2);
        k2.DivINT(cal2);

        ValueSTRICT dx = k1;
        dx.Sub(k2);
        dx.DivINT(2);

        ValueSTRICT A((int)N - (int)counter);
        A.Sub(dx);
        A.MulINT(1000000);     // Это приводим к своей выводимой степени
        A.DivUINT(N);

        if (!Channel::Current()->mod.timeComparator.Is_1s())
        {
            A.MulINT(10);
        }

        if (values.AppendValue(A.ToDouble()))
        {
            Display::Refresh();
        }

        if (Channel::Current()->mod.timeComparator.Is_1s())
        {
            SetValue("%.4f E-6", A.ToDouble());
        }
        else
        {
            SetValue("%.4f E-7", A.ToDouble());
        }
    }
}


ValueFrequency_Ratio::ValueFrequency_Ratio(uint counter1, uint counter2)
{
    mainUnits.Set("");

    const ModeFrequency &mode = ModeFrequency::Current();

    counter1 /= 2;

    ValueSTRICT valueA(counter1);

    if (mode.IsRatioAB() || mode.IsRatioBA())
    {
        valueA.DivINT(NumberPeriods::Current().ToAbs());
    }
    else if ((mode.IsRatioAC() || mode.IsRatioBC()) && Relation::IsEnabled())
    {
        if (counter2 == 0)
        {
            SetInfiniteValue();
            return;
        }

        valueA.DivUINT(counter2);
        valueA.DivUINT(32);
    }
    else if (mode.IsRatioCA() || mode.IsRatioCB())
    {
        valueA.MulUINT(64);
        valueA.DivINT(NumberPeriods::Current().ToAbs());

        counter1 *= 64;         // Это нужно, чтобы при переводе в текстовое значение учесть знаки, которые добавляются
                                // после умножения на 64
    }

    SetValue(valueA, counter1);
}


char *ValueFrequency_Ratio::GetSuffixUnit(int order) const
{
    return GetSuffixUnitRelated(order);
}


bool ValueFrequency_Comparator::Stack::AppendValue(double val)
{
    Push(val);

    return true;
}


double ValueFrequency_Comparator::Stack::GetFromEnd(int fromEnd)
{
    if (fromEnd < 0 || fromEnd >(Size() - 1))
    {
        return -1.0;
    }

    return (*this)[Size() - 1 - fromEnd];
}
