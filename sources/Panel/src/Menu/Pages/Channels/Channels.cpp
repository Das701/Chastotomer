#include "defines.h"
#include "Settings.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/Pages/Channels/Channels.h"


using namespace Primitives;


extern Page pageModesA;
extern Page pageModesB;
extern Page pageModesC;
extern Page pageModesD;

extern Switch *switchModeFrequencyA;
extern Switch *switchModeFrequencyB;
extern Switch *switchModeFrequencyC;

extern Switch *switchModeCountPulseA;
extern Switch *switchModeCountPulseB;
extern Switch *switchModeCountPulseC;

extern Switch *switchModePeriodA;
extern Switch *switchModePeriodB;

extern Switch *switchModeDurationA;
extern Switch *switchModeDurationB;

extern Page pageChannelA;
extern Page pageChannelB;
extern Page pageChannelC;
extern Page pageChannelD;


Channel *Channel::current = &Channel::A;

static const bool enabledMeasuresA[TypeMeasure::Count] = { true, true, true, true };
static const bool enabledModeFrequencyA[ModeFrequency::Count] = { true, true, true, true, false, false, false, false, true, true };
static const bool enabledModeCountPulseA[ModeCountPulse::Count] = { true, true, false, false, false, false, false, false, true };
Channel Channel::A(&pageChannelA, &pageModesA, switchModeFrequencyA, switchModeCountPulseA, switchModePeriodA, switchModeDurationA, enabledMeasuresA, enabledModeFrequencyA, enabledModeCountPulseA);

static const bool enabledMeasuresB[TypeMeasure::Count] = { true, true, true, true };
static const bool enabledModeFrequencyB[ModeFrequency::Count] = { true, true, false, false, true, true, false, false, true, false };
static const bool enabledModeCountPulseB[ModeCountPulse::Count] = { false, false, true, true, false, false, false, false, true };
Channel Channel::B(&pageChannelB, &pageModesB, switchModeFrequencyB, switchModeCountPulseB, switchModePeriodB, switchModeDurationB, enabledMeasuresB, enabledModeFrequencyB, enabledModeCountPulseB);

static const bool enabledMeasuresC[TypeMeasure::Count] = { true, false, false, true };
static const bool enabledModeFrequencyC[ModeFrequency::Count] = { true, false, false, false, false, false, true, true, false, false };
static const bool enabledModeCountPulseC[ModeCountPulse::Count] = { false, false, false, false, true, true, true, true, false };
Channel Channel::C(&pageChannelC, &pageModesC, switchModeFrequencyC, switchModeCountPulseC, nullptr,           nullptr,             enabledMeasuresC, enabledModeFrequencyC, enabledModeCountPulseC);

static const bool enabledMeasuresD[TypeMeasure::Count] = { true, false, false, false };
static const bool enabledModeFrequencyD[ModeFrequency::Count] = { true, false, false, false, false, false, false, false, false, false };
static const bool enabledModeCountPulseD[ModeCountPulse::Count] = { false, false, false, false, false, false, false, false, false };
Channel Channel::D(&pageChannelD, &pageModesD, nullptr,              nullptr,               nullptr,           nullptr,             enabledMeasuresD, enabledModeFrequencyD, enabledModeCountPulseD);


PeriodTimeLabels Channel::timeLabels(PeriodTimeLabels::T_8);
NumberPeriods    Channel::numberPeriods(NumberPeriods::_1);
TimeMeasure      Channel::timeMeasure(TimeMeasure::_1ms);


SettingsChannel::SettingsChannel(const bool *enabledMeasures, const bool *enabledModeFrequency) :
    couple(InputCouple::AC),
    impedance(InputImpedance::_1MOmh),
    modeFilter(ModeFilter::Off),
    modeFront(ModeFront::Front),
    divider(Divider::_1),
    typeSynch(TypeSynch::Manual),
    typeMeasure(TypeMeasure::Frequency, enabledMeasures, TypeMeasure::Count),
    modeFrequency(ModeFrequency::Frequency, enabledModeFrequency, ModeFrequency::Count),
    modePeriod(ModePeriod::Period),
    modeDuration(ModeDuration::Ndt)
{

}


Channel::Channel(Page *pSettings, Page *pModes, Switch *pModeFrequency, Switch *pModeCountPulse, Switch *pModePeriod, Switch *pModeDuration,
    const bool *enabledMeasures, const bool *enabledModeFrequency, const bool *enabledModeCountPulse) :
    pageSettings(pSettings),
    pageModes(pModes),
    set(enabledMeasures, enabledModeFrequency),
    modeCountPulse(ModeCountPulse::AtB, enabledModeCountPulse, ModeCountPulse::Count),
    switchModeFrequency(pModeFrequency),
    switchModeCountPulse(pModeCountPulse),
    switchModePeriod(pModePeriod),
    switchModeDuration(pModeDuration)
{
}


Channel &Channel::Current()
{
    return *current;
}


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable: 4062)
#endif


bool TypeMeasure::IsActiveTimeLabels(int m)
{
    if (IsFrequency())
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
    else if (IsPeriod())
    {
        ModePeriod::E mode = (ModePeriod::E)m;

        switch (mode)
        {
        case ModePeriod::Period:      return true;

        case ModePeriod::F_1:         return FreqMeter::modeTest.IsEnabled();
        }
    }
    else if (IsDuration())
    {
        ModeDuration::E mode = (ModeDuration::E)m;

        switch (mode)
        {
        case ModeDuration::Ndt:
        case ModeDuration::StartStop:
        case ModeDuration::FillFactor:
        case ModeDuration::Phase:        return true;
        }
    }

    return false;
}


bool TypeMeasure::IsActiveTimeMeasure(int m)
{
    if (IsFrequency())
    {
        ModeFrequency::E mode = (ModeFrequency::E)m;

        switch (mode)
        {
        case ModeFrequency::Frequency:
        case ModeFrequency::RatioAC:
        case ModeFrequency::RatioBC:     return true;
        }
    }
    else if (IsPeriod())
    {
        ModePeriod::E mode = (ModePeriod::E)m;

        switch (mode)
        {
        case ModePeriod::F_1:            return true;
        }
    }

    return false;
}


bool TypeMeasure::IsActiveNumberPeriods(int m)
{
    if (IsFrequency())
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
    else if (IsPeriod())
    {
        ModePeriod::E mode = (ModePeriod::E)m;

        switch (mode)
        {
        case ModePeriod::Period:         return true;
        }
    }
    else if (IsCountPulse())
    {
        ModeCountPulse::E mode = (ModeCountPulse::E)m;

        switch (mode)
        {
        case ModeCountPulse::ATB:
        case ModeCountPulse::BTA:
        case ModeCountPulse::CTA:
        case ModeCountPulse::CTB:        return true;
        }
    }

    return false;
}


#ifdef WIN32
#pragma warning(pop)
#endif


bool Channel::ConsistTimeMeasure()
{
    if (set.typeMeasure.IsFrequency())
    {
        if (ModeFrequency::Current().IsFrequency() || ModeFrequency::Current().IsRatioAC() || ModeFrequency::Current().IsRatioBC())
        {
            return true;
        }
    }

    if (set.typeMeasure.IsPeriod() && ModePeriod::Current().IsF_1())
    {
        return true;
    }

    return false;
}


static void DrawValue(Enumeration &param, int x, int y)
{
    int width = 60;
    Primitives::Rectangle(width, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::WHITE);
    Text(param.ToString()).Write(x + 2, y + 7, width, Color::WHITE);
}


void Channel::DrawParameters(int x, int y)
{
    TypeMeasure *type = pageModes->GetTypeMeasure();

    int mode = pageModes->GetModeMeasure();

    int dX = 70;

    if (type->IsActiveTimeMeasure(mode))
    {
        DrawValue(timeMeasure, x, y);
    }

    if (type->IsActiveNumberPeriods(mode))
    {
        DrawValue(numberPeriods, x + dX, y);
    }

    if (type->IsActiveTimeLabels(mode))
    {
        DrawValue(timeLabels, x + 2 * dX, y);
    }
}


void Channel::PressSetup()
{
    switch (Channel::A.set.typeMeasure.value)
    {
    case TypeMeasure::Frequency:    pageModes->items[1] = switchModeFrequency;     break;
    case TypeMeasure::Period:       pageModes->items[1] = switchModePeriod;        break;
    case TypeMeasure::Duration:     pageModes->items[1] = switchModeDuration;      break;
    case TypeMeasure::CountPulse:   pageModes->items[1] = switchModeCountPulse;    break;
    }
}


void Channel::OnChanged_TypeMeasure()
{
    switch (set.typeMeasure.value)
    {
    case TypeMeasure::Frequency:    if (switchModeFrequency != nullptr)  { switchModeFrequency->FuncOnPress();  }   break;
    case TypeMeasure::CountPulse:   if (switchModeCountPulse != nullptr) { switchModeCountPulse->FuncOnPress(); }   break;
    case TypeMeasure::Period:       if (switchModePeriod != nullptr)     { switchModePeriod->FuncOnPress();     }   break;
    case TypeMeasure::Duration:     if (switchModeDuration != nullptr)   { switchModeDuration->FuncOnPress();   }   break;
    }
}


int Channel::Number() const
{
    static const Channel *const channels[4] = { &A, &B, &C, &D };

    for (int i = 0; i < 4; i++)
    {
        if (this == channels[i])
        {
            return i;
        }
    }

    return 0;
}
