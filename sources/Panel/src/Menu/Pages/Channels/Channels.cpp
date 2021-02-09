#include "defines.h"
#include "Settings.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/Hint.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstring>


using namespace Primitives;


extern PageModes pageModesA;
extern PageModes pageModesB;
extern PageModes pageModesC;
extern PageModes pageModesD;

extern Switch *switchModeFrequencyA;
extern Switch *switchModeFrequencyB;
extern Switch *switchModeFrequencyC;
extern Switch *switchModeFrequencyD;

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


Channel *Channel::current = nullptr;

Channel *Channel::A = nullptr;
Channel *Channel::B = nullptr;
Channel *Channel::C = nullptr;
Channel *Channel::D = nullptr;

static const bool enabledMeasuresA[TypeMeasure::Count] = { true, true, true, true };
static const bool enabledModeFrequencyA[ModeFrequency::Count] =
                                                     { true, true, true, true, false, false, false, false, true, true };
static const bool enabledModeCountPulseA[ModeCountPulse::Count] =
                                                         { true, true, false, false, false, false, false, false, true };

static const bool enabledMeasuresB[TypeMeasure::Count] = { true, true, true, true };
static const bool enabledModeFrequencyB[ModeFrequency::Count] =
                                                    { true, true, false, false, true, true, false, false, true, false };
static const bool enabledModeCountPulseB[ModeCountPulse::Count] =
                                                         { false, false, true, true, false, false, false, false, true };

static const bool enabledMeasuresC[TypeMeasure::Count] = { true, false, false, true };
static const bool enabledModeFrequencyC[ModeFrequency::Count] =
                                                  { true, false, false, false, false, false, true, true, false, false };
static const bool enabledModeCountPulseC[ModeCountPulse::Count] =
                                                          { false, false, false, false, true, true, true, true, false };

static const bool enabledMeasuresD[TypeMeasure::Count] = { true, false, false, false };
static const bool enabledModeFrequencyD[ModeFrequency::Count] =
                                                { true, false, false, false, false, false, false, false, false, false };
static const bool enabledModeCountPulseD[ModeCountPulse::Count] =
                                                      { false, false, false, false, false, false, false, false, false };


bool Channel::ExistMeasure(TypeMeasure::E type, int mode)
{
    if (type == TypeMeasure::Frequency)
    {
        switch (Number())
        {
        case 0: return enabledModeFrequencyA[mode];
        case 1: return enabledModeFrequencyB[mode];
        case 2: return enabledModeFrequencyC[mode];
        case 3: return enabledModeFrequencyD[mode];
        }
    }
    else if (type == TypeMeasure::CountPulse)
    {
        switch (Number())
        {
        case 0: return enabledModeCountPulseA[mode];
        case 1: return enabledModeCountPulseB[mode];
        case 2: return enabledModeCountPulseC[mode];
        case 3: return enabledModeCountPulseD[mode];
        }
    }

    return Number() > 2;
}


PeriodTimeLabels ModesChannel::timeLabels(PeriodTimeLabels::T_8);
NumberPeriods    ModesChannel::numberPeriods(NumberPeriods::_1);
TimeMeasure      ModesChannel::timeMeasure(TimeMeasure::_1ms);
TimeComparator   ModesChannel::timeComparator(TimeComparator::_1s);


SettingsChannel::SettingsChannel() :
    couple(InputCouple::AC),
    impedance(InputImpedance::_1MOmh),
    modeFilter(ModeFilter::Off),
    modeFront(ModeFront::Front),
    divider(Divider::_1),
    typeSynch(TypeSynch::Manual)
{

}


ModesChannel::ModesChannel(Switch *pModeFrequency, Switch *pModeCountPulse, Switch *pModePeriod, Switch *pModeDuration,
    const bool *enabledMeasures, const bool *enabledModeFrequency, const bool *enabledModeCountPulse) :
    typeMeasure(TypeMeasure::Frequency, enabledMeasures, TypeMeasure::Count),
    modeFrequency(ModeFrequency::Frequency, enabledModeFrequency, ModeFrequency::Count),
    modePeriod(ModePeriod::Period),
    modeDuration(ModeDuration::Ndt),
    modeCountPulse(ModeCountPulse::AtB, enabledModeCountPulse, ModeCountPulse::Count),
    switchModeFrequency(pModeFrequency),
    switchModeCountPulse(pModeCountPulse),
    switchModePeriod(pModePeriod),
    switchModeDuration(pModeDuration)
{

}


Channel::Channel(int num, Page *pSettings, PageModes *pModes,
    Switch *pModeFrequency, Switch *pModeCountPulse, Switch *pModePeriod, Switch *pModeDuration,
    const bool *enabledMeasures, const bool *enabledModeFrequency, const bool *enabledModeCountPulse) :
    pageSettings(pSettings),
    pageModes(pModes),
    set(),
    mod(pModeFrequency, pModeCountPulse, pModePeriod, pModeDuration, enabledMeasures, enabledModeFrequency,
        enabledModeCountPulse),
    number(num)
{
}


void Channel::Create()
{
    if (A == nullptr)
    {
        A = new Channel(0, &pageChannelA, &pageModesA, switchModeFrequencyA, switchModeCountPulseA, switchModePeriodA,
            switchModeDurationA, enabledMeasuresA, enabledModeFrequencyA, enabledModeCountPulseA);
    }
    if (B == nullptr)
    {
        B = new Channel(1, &pageChannelB, &pageModesB, switchModeFrequencyB, switchModeCountPulseB, switchModePeriodB,
            switchModeDurationB, enabledMeasuresB, enabledModeFrequencyB, enabledModeCountPulseB);
    }
    if (C == nullptr)
    {
        C = new Channel(2, &pageChannelC, &pageModesC, switchModeFrequencyC, switchModeCountPulseC, nullptr, nullptr,
            enabledMeasuresC, enabledModeFrequencyC, enabledModeCountPulseC);
    }
    if (D == nullptr)
    {
        D = new Channel(3, &pageChannelD, &pageModesD, switchModeFrequencyD, nullptr, nullptr, nullptr,
            enabledMeasuresD, enabledModeFrequencyD, enabledModeCountPulseD);
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
    bool result = false;

    if (IsFrequency())
    {
        ModeFrequency::E mode = (ModeFrequency::E)m;

        switch (mode) //-V2522
        {
        case ModeFrequency::T_1:
        case ModeFrequency::RatioCA:
        case ModeFrequency::RatioCB:    result = true;
            break;

        case ModeFrequency::Frequency:  result = CURRENT_CHANNEL_IS_A_OR_B ? FreqMeter::modeTest.IsEnabled() : false;
            break;

        case ModeFrequency::Tachometer: result = FreqMeter::modeTest.IsEnabled();
            break;
        }
    }
    else if (IsPeriod())
    {
        ModePeriod::E mode = (ModePeriod::E)m;

        switch (mode) //-V2522
        {
        case ModePeriod::Period:      result = true;                                break;

        case ModePeriod::F_1:         result = FreqMeter::modeTest.IsEnabled();     break;
        }
    }
    else if (IsDuration()) //-V2516
    {
        ModeDuration::E mode = (ModeDuration::E)m;

        switch (mode)
        {
        case ModeDuration::Ndt:
        case ModeDuration::StartStop:
        case ModeDuration::FillFactor:
        case ModeDuration::Phase:       result = true; break;

        case ModeDuration::Ndt_1ns:
        case ModeDuration::Count:       break;
        }
    }

    return result;
}


bool TypeMeasure::IsActiveTimeComparator(int mode)
{
    return IsFrequency() && (mode == ModeFrequency::Comparator);
}


bool TypeMeasure::IsActiveTimeMeasure(int m)
{
    if (IsFrequency())
    {
        ModeFrequency::E mode = (ModeFrequency::E)m;

        switch (mode) //-V2520 //-V2522
        {
        case ModeFrequency::Frequency:
        case ModeFrequency::RatioAC:
        case ModeFrequency::RatioBC:     return true;
        }
    }
    else if (IsPeriod()) //-V2516
    {
        ModePeriod::E mode = (ModePeriod::E)m;

        switch (mode) //-V2520 //-V2522
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

        switch (mode) //-V2520 //-V2522
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

        switch (mode) //-V2520 //-V2522
        {
        case ModePeriod::Period:         return true;
        }
    }
    else if (IsCountPulse()) //-V2516
    {
        ModeCountPulse::E mode = (ModeCountPulse::E)m;

        switch (mode) //-V2520 //-V2522
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


bool ModesChannel::ConsistTimeMeasure()
{
    if (typeMeasure.IsFrequency())
    {
        if (ModeFrequency::Current().IsFrequency() ||
            ModeFrequency::Current().IsRatioAC() ||
            ModeFrequency::Current().IsRatioBC())
        {
            return true;
        }
    }

    if (typeMeasure.IsPeriod() && ModePeriod::Current().IsF_1())
    {
        return true;
    }

    return false;
}


static void DrawValue(const Enumeration &param, int x, int y)
{
    int width = 60;
    Primitives::Rectangle(width, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::WHITE);
    Text(param.ToString()).Write(x + 2, y + 7, width, Color::WHITE);
}


void Channel::DrawMode(int x, int y)
{
    mod.typeMeasure.Draw(x, y);

    CurrentModeMeasure::Draw(x + 110, y);

    x += 260;

    TypeMeasure *type = pageModes->GetTypeMeasure();

    int mode = pageModes->GetModeMeasure();

    int dX = 70;

    if (type->IsActiveTimeMeasure(mode))
    {
        DrawValue(ModesChannel::timeMeasure, x, y);
    }

    if (type->IsActiveNumberPeriods(mode))
    {
        DrawValue(ModesChannel::numberPeriods, x + dX, y);
    }

    if (type->IsActiveTimeLabels(mode))
    {
        DrawValue(ModesChannel::timeLabels, x + 2 * dX, y);
    }

    if (type->IsActiveTimeComparator(mode))
    {
        DrawValue(ModesChannel::timeComparator, x, y);
    }
}


void Channel::DrawSettings(int x, int y)
{
    if (Display::InDrawingPart(y, 20))
    {
        Rectangle(460, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::GREEN_20);

        Text(GetSettings()).Write(x + 10, y + 6, Color::WHITE);
    }
}


void Channel::PressSetup()
{
    switch (Channel::A->mod.typeMeasure.value)
    {
    case TypeMeasure::Frequency:    pageModes->items[1] = mod.switchModeFrequency;     break;
    case TypeMeasure::Period:       pageModes->items[1] = mod.switchModePeriod;        break;
    case TypeMeasure::Duration:     pageModes->items[1] = mod.switchModeDuration;      break;
    case TypeMeasure::CountPulse:   pageModes->items[1] = mod.switchModeCountPulse;    break;
    }
}


void Channel::OnChanged_TypeMeasure()
{
    switch (mod.typeMeasure.value)
    {
    case TypeMeasure::Frequency:   if (mod.switchModeFrequency != nullptr)  { mod.switchModeFrequency->FuncOnPress();  }
        break;
    case TypeMeasure::CountPulse:  if (mod.switchModeCountPulse != nullptr) { mod.switchModeCountPulse->FuncOnPress(); }
        break;
    case TypeMeasure::Period:      if (mod.switchModePeriod != nullptr)     { mod.switchModePeriod->FuncOnPress();     }
        break;
    case TypeMeasure::Duration:    if (mod.switchModeDuration != nullptr)   { mod.switchModeDuration->FuncOnPress();   }
        break;
    }
}


void Channel::SetCurrentNext()
{
    int num = Channel::Current()->Number();

    Math::CircleIncrease<uint8>((uint8 *)&num, 0, Channel::Count - 1);

    static Channel *const channels[Count] = { A, B, C, D };

    current = channels[num];
}


void Channel::LoadCurrentToFPGA(bool openSettings, bool openMode)
{
    Current()->OnChanged_TypeMeasure();
    Current()->LoadToFPGA();
    if (openSettings) { Menu::SetOpenedPage(Current()->pageSettings); }
    else if (openMode) { Menu::SetOpenedPage(Current()->pageModes); }
    Hint::Hide();
}


String Channel::GetSettings()
{
    // Добавляет в конец строки settings строку ugo через разделительный символ
#define ADD_UGO(ugo)            \
    std::strcat(settings, " "); \
    std::strcat(settings, ugo);

    static const char *const names[Channel::Count] = { "Канал A:", "Канал B:", "Канал C:", "Канал D:" };

    char settings[100] = { 0 };

    std::strcpy(settings, names[NUMBER_CURRENT_CHANNEL]);

    ADD_UGO(current->set.couple.UGO());
    ADD_UGO(current->set.impedance.UGO());
    ADD_UGO(current->set.modeFilter.UGO());
    ADD_UGO(current->set.modeFront.UGO());
    ADD_UGO(current->set.divider.UGO());
    ADD_UGO(current->set.typeSynch.UGO());

    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        ADD_UGO(SU::Int2String(LEVEL_SYNCH(CURRENT_CHANNEL) * current->set.divider.ToAbs()).c_str());
        if (current->set.typeSynch.IsManual())
        {
            std::strcat(settings, "мВ");
        }
        else
        {
            switch (ModesChannel::timeLabels.value)
            {
            case PeriodTimeLabels::T_3:     std::strcat(settings, "x10-3");     break;
            case PeriodTimeLabels::T_4:     std::strcat(settings, "x10-4");     break;
            case PeriodTimeLabels::T_5:     std::strcat(settings, "x10-5");     break;
            case PeriodTimeLabels::T_6:     std::strcat(settings, "x10-6");     break;
            case PeriodTimeLabels::T_7:     std::strcat(settings, "x10-7");     break;
            case PeriodTimeLabels::T_8:     std::strcat(settings, "x10-8");     break;
            }
        }
    }

    return String(settings);
}
