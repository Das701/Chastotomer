#include "defines.h"
#include "Calculate/MathFPGA.h"
#include "Calculate/ValuesFPGA.h"
#include "Display/Display.h"
#include "Display/Objects.h"
#include "Hardware/FPGA.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Utils/StringUtils.h"
#include <cstdio>
#include <cstring>


char *ValueFPGA::UGO_DivNULL = "=X/0";
char *ValueFPGA::UGO_OVERLAPPED = "оепеонкмемхе";

ValueFPGA *ValueFPGA::valueFPGA = nullptr;
String ValueFPGA::value;



void ValueFPGA::Create(uint value1, uint value2, uint value3, uint value4, uint value5)
{
    TypeMeasure &type = Channel::Current()->mod.typeMeasure;

    if (valueFPGA == nullptr)
    {
        delete valueFPGA;
        valueFPGA = nullptr;
    }

    if (type.IsFrequency())
    {
        switch (Channel::Current()->mod.modeFrequency)
        {
        case ModeFrequency::Frequency:  valueFPGA = new ValueFrequency_Frequency(value1);           break;
        case ModeFrequency::T_1:        valueFPGA = new ValueFrequency_T_1(value1);                 break;
        case ModeFrequency::Tachometer: valueFPGA = new ValueFrequency_Tachometer(value1);          break;
        case ModeFrequency::Comparator:
            valueFPGA = new ValueFrequency_Comparator(value1, (int)value2, (int)value3, (int)value4, (int)value5);
            break;
        case ModeFrequency::RatioAB:
        case ModeFrequency::RatioAC:
        case ModeFrequency::RatioBA:
        case ModeFrequency::RatioBC:
        case ModeFrequency::RatioCA:
        case ModeFrequency::RatioCB:    valueFPGA = new ValueFrequency_Ratio(value1, value2);       break;
        }
    }
    else if (type.IsPeriod())
    {
        switch (Channel::Current()->mod.modePeriod)
        {
        case ModePeriod::Period:        valueFPGA = new ValuePeriod_Period(value1);     break;
        case ModePeriod::F_1:           valueFPGA = new ValuePeriod_F_1(value1);        break;
        }
    }
    else if (type.IsDuration())
    {
        switch (Channel::Current()->mod.modeDuration)
        {
        case ModeDuration::Ndt_1ns:     valueFPGA = new ValueDuration_Ndt_1ns(value1, value2, value3);
            break;;
        case ModeDuration::FillFactor:
        case ModeDuration::Phase:       valueFPGA = new ValueDuration_Phase_FillFactor(value1, value2);
            break;;
        case ModeDuration::Ndt:
        case ModeDuration::StartStop:           valueFPGA = new ValueDuration_Ndt_StartStop(value1);
            break;
        }
    }
    else if (type.IsCountPulse())
    {
        valueFPGA = new ValueCountPulse(value1);
    }
}


int ValueFPGA::NumDigitsInNumber(uint number) const
{
    if (number == 0)
    {
        return 1;
    }

    int result = 0;

    while (number > 0)
    {
        result++;
        number /= 10;
    }

    return result;
}


char *ValueFPGA::GetSuffixUnit(int order) const
{
    if (order >= 15)       { return "P"; }
    else if (order >= 12)  { return "T"; }
    else if (order >= 9)   { return "G"; }
    else if (order >= 6)   { return "M"; }
    else if (order >= 3)   { return "k"; }
    else if (order >= 0)   { return "";  }
    else if (order >= -3)  { return "m"; }
    else if (order >= -6)  { return "u"; }
    else if (order >= -9)  { return "n"; }
    else if (order >= -12) { return "p"; }
    
    return "ph";
}


char *ValueFPGA::GetSuffixUnitRelated(int order)
{
    if (order >= 15) { return "E+15"; }
    else if (order >= 12) { return "E+12"; }
    else if (order >= 9) { return "E+9"; }
    else if (order >= 6) { return "E+6"; }
    else if (order >= 3) { return "E+3"; }
    else if (order >= 0) { return "E+0"; }
    else if (order >= -3) { return "E-3"; }
    else if (order >= -6) { return "E-6"; }
    else if (order >= -9) { return "E-9"; }
    else if (order >= -12) { return "E-12"; }

    return "E-15";
}


String ValueFPGA::GiveDigits()
{
    return "";
}


String ValueFPGA::GiveUnits()
{
    return "";
}


char *ValueFPGA::GiveStringValue()
{
    return value.c_str();
}


void ValueFPGA::SetValue(char *, ...)
{
    if (FPGA::IsOverloaded())
    {
        value.Set(UGO_OVERLAPPED);
        return;
    }

    Display::zoneData->Refresh();
}


void ValueFPGA::SetValue(ValueSTRICT strict, uint counter)
{
    if (FPGA::IsOverloaded())
    {
        value.Set(UGO_OVERLAPPED);
        return;
    }

    int order = 0;

    int numDigitsInCounter = NumDigitsInNumber(counter);

    if (numDigitsInCounter < 1)
    {
        numDigitsInCounter = 1;
    }

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

        if (NumDigitsInNumber((uint)strict.ToDouble()) > numDigitsInCounter)
        {
            strict.DivUINT(1000);
            order += 3;
        }
    }

    char buffer[30];
    std::sprintf(buffer, "%10.10f", strict.ToDouble());

    if (strict.IsZero())
    {
        SU::LeaveFewDigits(buffer, 29, numDigitsInCounter);
    }
    else
    {
        SU::LeaveFewSignedDigits(buffer, 29, numDigitsInCounter);
    }

    std::sprintf(buffer, GetSuffixUnit(order));

    std::sprintf(buffer, mainUnits.c_str());

    SetValue(buffer);
}


void ValueFPGA::SetInfiniteValue()
{
    SetValue(UGO_DivNULL);
}


void ValueFPGA::SetIntegerValue(uint counter)
{
    SetValue("%d", counter);
}


void ValueFPGA::SetInvalidData()
{
    value.Set("----------");

    Display::zoneData->Refresh();
}
