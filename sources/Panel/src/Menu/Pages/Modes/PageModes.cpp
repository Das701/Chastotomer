#include "defines.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Settings.h"


using namespace Display;
using namespace Display::Primitives;


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

    static const Enumeration *const modes[3][TypeMeasure::Count][6] =
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
        mode = modes[CURRENT_CHANNEL][TypeMeasure::Current().value][types[CURRENT_CHANNEL][TypeMeasure::Current().value]->value];
    }

    if (mode)
    {
        int width = 60;

        Rectangle(width, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::WHITE);

        Text(mode->ToText()).Write(x + 2, y + 7, width, Color::WHITE);
    }
}


void TypeMeasure::Draw(int x, int y) const
{
    int width = 100;

    Rectangle(width, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::WHITE);

    Text(ToText()).Write(x, y + 10, width, Color::WHITE);
}


void PageModes::ResetModeCurrentMeasure()
{
    PageModesA::ResetModeCurrentMeasure();
    PageModesB::ResetModeCurrentMeasure();
    PageModesC::ResetModeCurrentMeasure();
}
