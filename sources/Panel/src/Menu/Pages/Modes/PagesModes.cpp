#include "defines.h"
#include "FreqMeter.h"
#include "Settings.h"
#include "Menu/Pages/Modes/PagesModes.h"


bool CurrentPageModes::ConsistTimeMeasure()
{
    if (TypeMeasure::Current().IsFrequency())
    {
        if (ModeMeasureFrequency::Current().IsFrequency() || ModeMeasureFrequency::Current().IsRatioAC() || ModeMeasureFrequency::Current().IsRatioBC())
        {
            return true;
        }
    }

    if (TypeMeasure::Current().IsPeriod() && ModeMeasurePeriod::Current().IsF_1())
    {
        return true;
    }

    return false;
}


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable: 4062)
#endif


bool PageModes::IsActiveTimeLabels(TypeMeasure *type, int m)
{
    if (type->IsFrequency())
    {
        ModeMeasureFrequency::E mode = (ModeMeasureFrequency::E)m;

        switch (mode)
        {
        case ModeMeasureFrequency::T_1:
        case ModeMeasureFrequency::RatioCA:
        case ModeMeasureFrequency::RatioCB:     return true;

        case ModeMeasureFrequency::Frequency:
        case ModeMeasureFrequency::Tachometer:  return FreqMeter::modeTest.IsEnabled();
        }
    }
    else if (type->IsPeriod())
    {
        ModeMeasurePeriod::E mode = (ModeMeasurePeriod::E)m;

        switch (mode)
        {
        case ModeMeasurePeriod::Period:      return true;

        case ModeMeasurePeriod::F_1:         return FreqMeter::modeTest.IsEnabled();
        }
    }
    else if (type->IsDuration())
    {
        ModeMeasureDuration::E mode = (ModeMeasureDuration::E)m;

        switch (mode)
        {
        case ModeMeasureDuration::Ndt:
        case ModeMeasureDuration::Ndt2:
        case ModeMeasureDuration::FillFactor:
        case ModeMeasureDuration::Phase:        return true;
        }
    }

    return false;
}


bool PageModes::IsActiveTimeMeasure(TypeMeasure *type, int m)
{
    if (type->IsFrequency())
    {
        ModeMeasureFrequency::E mode = (ModeMeasureFrequency::E)m;

        switch (mode)
        {
        case ModeMeasureFrequency::Frequency:
        case ModeMeasureFrequency::RatioAC:
        case ModeMeasureFrequency::RatioBC:     return true;
        }
    }
    else if (type->IsPeriod())
    {
        ModeMeasurePeriod::E mode = (ModeMeasurePeriod::E)m;

        switch (mode)
        {
        case ModeMeasurePeriod::F_1:            return true;
        }
    }

    return false;
}


bool PageModes::IsActiveNumberPeriods(TypeMeasure *type, int m)
{
    if (type->IsFrequency())
    {
        ModeMeasureFrequency::E mode = (ModeMeasureFrequency::E)m;

        switch (mode)
        {
        case ModeMeasureFrequency::T_1:
        case ModeMeasureFrequency::RatioAB:
        case ModeMeasureFrequency::RatioBA:
        case ModeMeasureFrequency::RatioCA:
        case ModeMeasureFrequency::RatioCB:     return true;
        }
    }
    else if (type->IsPeriod())
    {
        ModeMeasurePeriod::E mode = (ModeMeasurePeriod::E)m;

        switch (mode)
        {
        case ModeMeasurePeriod::Period:         return true;
        }
    }
    else if (type->IsCountPulse())
    {
        ModeMeasureCountPulse::E mode = (ModeMeasureCountPulse::E)m;

        switch (mode)
        {
        case ModeMeasureCountPulse::ATB:
        case ModeMeasureCountPulse::BTA:
        case ModeMeasureCountPulse::CTA:
        case ModeMeasureCountPulse::CTB:        return true;
        }
    }

    return false;
}


#ifdef WIN32
#pragma warning(pop)
#endif


void PageModes::DrawParameters(int, int)
{
    Page *pages[Channel::Count] =
    {
        PageModesA::self,
        PageModesB::self,
        PageModesC::self,
        PageModesD::self
    };

    Page &page = *pages[CURRENT_CHANNEL];


    TypeMeasure *type = page.GetTypeMeasure();

    int mode = page.GetModeMeasure();

    if (IsActiveTimeLabels(type, mode))
    {

    }

    if (IsActiveTimeMeasure(type, mode))
    {

    }

    if (IsActiveNumberPeriods(type, mode))
    {

    }
}
