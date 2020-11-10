#include "defines.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/Pages/Modes/PageModes.h"
#include "Settings.h"


using namespace Display;
using namespace Display::Primitives;


int CurrentTypeMeasure::Value()
{
    if (CURRENT_CHANNEL_IS_A)
    {
        return PageModesA::typeMeasure.value;
    }
    else if (CURRENT_CHANNEL_IS_B)
    {
        return PageModesB::typeMeasure.value;
    }
    else if (CURRENT_CHANNEL_IS_C)
    {
        return PageModesC::typeMeasure.value;
    }

    return 0;
}


Enumeration &CurrentTypeMeasure::ToEnumeration()
{
    if (CURRENT_CHANNEL_IS_A)
    {
        return PageModesA::typeMeasure;
    }
    else if (CURRENT_CHANNEL_IS_B)
    {
        return PageModesB::typeMeasure;
    }
    else if (CURRENT_CHANNEL_IS_C)
    {
        return PageModesC::typeMeasure;
    }

    return PageModesD::typeMeasure;
}


Enumeration &CurrentModeMeasure::ToEnumeration()
{
    if (CURRENT_CHANNEL_IS_A)
    {
        static Enumeration *const modes[4] =
        {
            &PageModesA::modeMeasureFrequency,
            &PageModesA::modeMeasurePeriod,
            &PageModesA::modeMeasureDuration,
            &PageModesA::modeMeasureCountPulse
        };

        return *modes[PageModesA::typeMeasure.value];
    }
    else if (CURRENT_CHANNEL_IS_B)
    {
        static Enumeration *const modesB[4] =
        {
            &PageModesB::modeMeasureFrequency,
            &PageModesB::modeMeasurePeriod,
            &PageModesB::modeMeasureDuration,
            &PageModesB::modeMeasureCountPulse
        };

        return *modesB[PageModesB::typeMeasure.value];
    }
    else if (CURRENT_CHANNEL_IS_C)
    {
        static Enumeration *const modesC[2] =
        {
            &PageModesC::modeMeasureFrequency,
            &PageModesC::modeMeasureCountPulse
        };

        return *modesC[PageModesC::typeMeasure.value];;
    }

    return PageModesD::modeMeasureFrequency;
}


void CurrentModeMeasure::Draw(int x, int y)
{
    int width = 120;

    Rectangle(width, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::WHITE);

    Text(ToEnumeration().ToText()).Write(x + 10, y + 10, Color::WHITE);
}


void CurrentModeMeasure::DrawParameters(int x, int y)
{
    static const Enumeration *const types[Channel::Count][4] =
    {
        { &PageModesA::modeMeasureFrequency, &PageModesA::modeMeasurePeriod, &PageModesA::modeMeasureDuration, &PageModesA::modeMeasureCountPulse },
        { &PageModesB::modeMeasureFrequency, &PageModesB::modeMeasurePeriod, &PageModesB::modeMeasureDuration, &PageModesB::modeMeasureCountPulse },
        { &PageModesC::modeMeasureFrequency, &PageModesC::modeMeasureCountPulse, nullptr, nullptr },
        { nullptr, nullptr, nullptr, nullptr }
    };

    static const Enumeration *const modes[3][TypeMeasureAB::Count][6] =
    {
        {
            { &PageModesA::timeMeasure,   &PageModesA::numberPeriods, &PageModesA::timeMeasure,   &PageModesA::numberPeriods, nullptr, nullptr },
            { &PageModesA::numberPeriods, &PageModesA::timeMeasure,   nullptr,                    nullptr,                    nullptr, nullptr },
            { nullptr,                    nullptr,                    nullptr,                    nullptr,                    nullptr, nullptr },
            { nullptr,                    &PageModesA::numberPeriods, nullptr,                    nullptr,                    nullptr, nullptr }
        },
        {
            { &PageModesA::timeMeasure,   &PageModesA::numberPeriods, &PageModesA::timeMeasure,   &PageModesA::numberPeriods, nullptr, nullptr },
            { &PageModesA::numberPeriods, &PageModesA::timeMeasure,   nullptr,                    nullptr,                    nullptr, nullptr },
            { nullptr,                    nullptr,                    nullptr,                    nullptr,                    nullptr, nullptr },
            { nullptr,                    &PageModesA::numberPeriods, nullptr,                    nullptr,                    nullptr, nullptr }
        },
        {
            { &PageModesC::timeMeasure,   &PageModesC::numberPeriods, &PageModesC::numberPeriods, nullptr,                    nullptr, nullptr },
            { nullptr,                    nullptr,                    &PageModesC::numberPeriods, &PageModesC::numberPeriods, nullptr, nullptr },
            { nullptr,                    nullptr,                    nullptr,                    nullptr,                    nullptr, nullptr },
            { nullptr,                    nullptr,                    nullptr,                    nullptr,                    nullptr, nullptr }
        }
    };

    const Enumeration *mode = &PageModesD::timeMeasure;

    if (!CURRENT_CHANNEL_IS_D)
    {
        mode = modes[CURRENT_CHANNEL][CurrentTypeMeasure::Value()][types[CURRENT_CHANNEL][CurrentTypeMeasure::Value()]->value];
    }

    if (mode)
    {
        int width = 60;

        Rectangle(width, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::WHITE);

        Text(mode->ToText()).Write(x + 2, y + 7, width, Color::WHITE);
    }
}


void CurrentTypeMeasure::Draw(int x, int y)
{
    int width = 100;

    Rectangle(width, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::WHITE);

    Text(ToEnumeration().ToText()).Write(x, y + 10, width, Color::WHITE);
}


bool CurrentTypeMeasure::IsFrequency()
{
    return ((CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsFrequency()) ||
        (CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsFrequency()) ||
        (CURRENT_CHANNEL_IS_C && PageModesC::typeMeasure.IsFrequency()) ||
        CURRENT_CHANNEL_IS_D);
}


bool CurrentTypeMeasure::IsPeriod()
{
    return ((CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsPeriod()) ||
        (CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsPeriod()));
}


bool CurrentTypeMeasure::IsDuration()
{
    return ((CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsDuration()) ||
        (CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsDuration()));
}


bool CurrentTypeMeasure::IsCountPulse()
{
    return ((CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsCountPulse()) ||
        (CURRENT_CHANNEL_IS_B && PageModesB::typeMeasure.IsCountPulse()) ||
        (CURRENT_CHANNEL_IS_C && PageModesC::typeMeasure.IsCountPulse()));
}


bool CurrentModeMeasureFrequency::IsTachometer()
{
    return (((PageModesA::modeMeasureFrequency.IsTachometer() && CURRENT_CHANNEL_IS_A) ||
        (PageModesB::modeMeasureFrequency.IsTachometer() && CURRENT_CHANNEL_IS_B)));
}


bool CurrentModeMeasureFrequency::IsComparator()
{
    return CURRENT_CHANNEL_IS_A && PageModesA::typeMeasure.IsFrequency() && PageModesA::modeMeasureFrequency.IsComparator();
}


bool CurrentModeMeasureFrequency::IsT_1()
{
    return ((CURRENT_CHANNEL_IS_A && PageModesA::modeMeasureFrequency.IsT_1()) ||
        (CURRENT_CHANNEL_IS_B && PageModesB::modeMeasureFrequency.IsT_1()));
}


bool CurrentModeMeasureFrequency::Is_RatioAC_or_RatioBC()
{
    return ((CURRENT_CHANNEL_IS_A && PageModesA::modeMeasureFrequency.IsRatioAC()) ||
        (CURRENT_CHANNEL_IS_B && PageModesB::modeMeasureFrequency.IsRatioBC()));
}


bool CurrentModeMeasureFrequency::Is_RatioAB_or_RatioBA()
{
    return ((CURRENT_CHANNEL_IS_A && PageModesA::modeMeasureFrequency.IsRatioAB()) ||
        (CURRENT_CHANNEL_IS_B && PageModesB::modeMeasureFrequency.IsRatioBA()));
}


bool CurrentModeMeasureFrequency::Is_RatioCA_or_RatioCB()
{
    return (CURRENT_CHANNEL_IS_C && (PageModesC::modeMeasureFrequency.IsRatioCA() || PageModesC::modeMeasureFrequency.IsRatioCB()));
}


bool CurrentModeMeasureFrequency::Is_Ratio()
{
    return Is_RatioAB_or_RatioBA() || Is_RatioAC_or_RatioBC() || Is_RatioCA_or_RatioCB();
}


bool CurrentModeMeasureCountPulse::IsBig_T()
{
    return ((CURRENT_CHANNEL_IS_A && (PageModesA::modeMeasureCountPulse == ModeMeasureCountPulseA::ATB))
        || (CURRENT_CHANNEL_IS_B && (PageModesB::modeMeasureCountPulse == ModeMeasureCountPulseB::BTA))
        || (CURRENT_CHANNEL_IS_C && (PageModesC::modeMeasureCountPulse == ModeMeasureCountPulseC::CTB))
        || (CURRENT_CHANNEL_IS_C && (PageModesC::modeMeasureCountPulse == ModeMeasureCountPulseC::CTA)));
}


void PageModes::ResetModeCurrentMeasure()
{
    PageModesA::ResetModeCurrentMeasure();
    PageModesB::ResetModeCurrentMeasure();
    PageModesC::ResetModeCurrentMeasure();
}
