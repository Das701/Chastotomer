#include "defines.h"
#include "Menu/Pages/Channels/Channels.h"


Channel::E Channel::current = Channel::_A;

static const bool enabledMeasuresA[TypeMeasure::Count] = { true, true, true, true };
static const bool enabledModeFrequencyA[ModeFrequency::Count] = { true, true, true, true, false, false, false, false, true, true };
Channel Channel::A(&pageChannelA, enabledMeasuresA, enabledModeFrequencyA);

static const bool enabledMeasuresB[TypeMeasure::Count] = { true, true, true, true };
static const bool enabledModeFrequencyB[ModeFrequency::Count] = { true, true, false, false, true, true, false, false, true, false };
Channel Channel::B(&pageChannelB, enabledMeasuresB, enabledModeFrequencyB);

static const bool enabledMeasuresC[TypeMeasure::Count] = { true, false, false, true };
static const bool enabledModeFrequencyC[ModeFrequency::Count] = { true, false, false, false, false, false, true, true, false, false };
Channel Channel::C(&pageChannelC, enabledMeasuresC, enabledModeFrequencyC);

static const bool enabledMeasuresD[TypeMeasure::Count] = { true, false, false, false };
static const bool enabledModeFrequencyD[ModeFrequency::Count] = { true, false, false, false, false, false, false, false, false, false };
Channel Channel::D(&pageChannelD, enabledMeasuresD, enabledModeFrequencyD);


PeriodTimeLabels Channel::timeLabels(PeriodTimeLabels::T_8);
NumberPeriods    Channel::numberPeriods(NumberPeriods::_1);
TimeMeasure      Channel::timeMeasure(TimeMeasure::_1ms);


Channel::Channel(Page *pSettings, const bool *enabledMeasures, const bool *enabledModeFrequency) :
    settings(pSettings),
    couple(InputCouple::AC),
    impedance(InputImpedance::_1MOmh),
    modeFilter(ModeFilter::Off),
    modeFront(ModeFront::Front),
    divider(Divider::_1),
    typeSynch(TypeSynch::Manual),
    typeMeasure(TypeMeasure::Frequency, enabledMeasures, TypeMeasure::Count),
    modeMeasureFrequency(ModeFrequency::Frequency, enabledModeFrequency, ModeFrequency::Count),
    modePeriod(ModeMeasurePeriod::Period)
{
}


Channel &Channel::Current()
{
    static Channel * const channels[Count] = { &A, &B, &C, &D };

    return *channels[current];
}


Page *Channel::PageForChannel(Channel::E channel)
{
    Page *pages[Channel::Count] =
    {
        PageModesA::self,
        PageModesB::self,
        PageModesC::self,
        PageModesD::self
    };

    return pages[channel];
}


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable: 4062)
#endif


bool Channel::IsActiveTimeLabels(TypeMeasure *type, int m)
{
    if (type->IsFrequency())
    {
        ModeFrequency::E mode = (ModeFrequency::E)m;

        switch (mode)
        {
        case ModeFrequency::T_1:
        case ModeFrequency::RatioCA:
        case ModeFrequency::RatioCB:     return true;

        case ModeFrequency::Frequency:   return CURRENT_CHANNEL_IS_A_OR_B ? FreqMeter::modeTest.IsEnabled() : false;

        case ModeFrequency::Tachometer:  return FreqMeter::modeTest.IsEnabled();
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
        case ModeMeasureDuration::StartStop:
        case ModeMeasureDuration::FillFactor:
        case ModeMeasureDuration::Phase:        return true;
        }
    }

    return false;
}


bool Channel::IsActiveTimeMeasure(TypeMeasure *type, int m)
{
    if (type->IsFrequency())
    {
        ModeFrequency::E mode = (ModeFrequency::E)m;

        switch (mode)
        {
        case ModeFrequency::Frequency:
        case ModeFrequency::RatioAC:
        case ModeFrequency::RatioBC:     return true;
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


bool Channel::IsActiveNumberPeriods(TypeMeasure *type, int m)
{
    if (type->IsFrequency())
    {
        ModeFrequency::E mode = (ModeFrequency::E)m;

        switch (mode)
        {
        case ModeFrequency::T_1:
        case ModeFrequency::RatioAB:
        case ModeFrequency::RatioBA:
        case ModeFrequency::RatioCA:
        case ModeFrequency::RatioCB:     return true;
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
