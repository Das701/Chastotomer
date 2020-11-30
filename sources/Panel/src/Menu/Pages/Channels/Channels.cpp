#include "defines.h"
#include "Settings.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/Pages/Channels/Channels.h"


using namespace Primitives;


extern PageModes pageModesA;
extern PageModes pageModesB;
extern PageModes pageModesC;
extern PageModes pageModesD;


extern Page pageChannelA;
extern Page pageChannelB;
extern Page pageChannelC;
extern Page pageChannelD;


Channel *Channel::current = nullptr;

Channel *Channel::A = nullptr;
Channel *Channel::B = nullptr;
Channel *Channel::C = nullptr;
Channel *Channel::D = nullptr;


PeriodTimeLabels Channel::timeLabels(PeriodTimeLabels::T_8);
NumberPeriods    Channel::numberPeriods(NumberPeriods::_1);
TimeMeasure      Channel::timeMeasure(TimeMeasure::_1ms);


SettingsChannel::SettingsChannel() :
    couple(InputCouple::AC),
    impedance(InputImpedance::_1MOmh),
    modeFilter(ModeFilter::Off),
    modeFront(ModeFront::Front),
    divider(Divider::_1),
    typeSynch(TypeSynch::Manual)
{

}


Channel::Channel(int num, Page *pSettings, PageModes *pModes) :
    pageSettings(pSettings),
    mod(pModes),
    set(),
    number(num)
{
}


void Channel::Create()
{
    if (A == nullptr)
    {
        A = new Channel(0, &pageChannelA, &pageModesA);
    }
    if (B == nullptr)
    {
        B = new Channel(1, &pageChannelB, &pageModesB);
    }
    if (C == nullptr)
    {
        C = new Channel(2, &pageChannelC, &pageModesC);
    }
    if (D == nullptr)
    {
        D = new Channel(3, &pageChannelD, &pageModesD);
    }

    if (Current() == nullptr)
    {
        SetCurrent(A);
    }
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
    if (mod->typeMeasure.IsFrequency())
    {
        if (ModeFrequency::Current().IsFrequency() || ModeFrequency::Current().IsRatioAC() || ModeFrequency::Current().IsRatioBC())
        {
            return true;
        }
    }

    if (mod->typeMeasure.IsPeriod() && ModePeriod::Current().IsF_1())
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
    TypeMeasure *type = mod->GetTypeMeasure();

    int mode = mod->GetModeMeasure();

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
    switch (Channel::A->mod->typeMeasure.value)
    {
    case TypeMeasure::Frequency:    mod->items[1] = mod->switchModeFrequency;     break;
    case TypeMeasure::Period:       mod->items[1] = mod->switchModePeriod;        break;
    case TypeMeasure::Duration:     mod->items[1] = mod->switchModeDuration;      break;
    case TypeMeasure::CountPulse:   mod->items[1] = mod->switchModeCountPulse;    break;
    }
}


void Channel::OnChanged_TypeMeasure()
{
    switch (mod->typeMeasure.value)
    {
    case TypeMeasure::Frequency:    if (mod->switchModeFrequency != nullptr)  { mod->switchModeFrequency->FuncOnPress();  }   break;
    case TypeMeasure::CountPulse:   if (mod->switchModeCountPulse != nullptr) { mod->switchModeCountPulse->FuncOnPress(); }   break;
    case TypeMeasure::Period:       if (mod->switchModePeriod != nullptr)     { mod->switchModePeriod->FuncOnPress();     }   break;
    case TypeMeasure::Duration:     if (mod->switchModeDuration != nullptr)   { mod->switchModeDuration->FuncOnPress();   }   break;
    }
}


void Channel::SetCurrent(int num)
{
    static Channel * const channels[Count] = { A, B, C, D };

    current = channels[num];
}
