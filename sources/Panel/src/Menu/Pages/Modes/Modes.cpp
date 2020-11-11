#include "Settings.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Menu/Pages/Modes/PageModes.h"


ModeMeasureCountPulse &ModeMeasureCountPulse::Current()
{
    static const bool correct[1] = { true };
    static ModeMeasureCountPulse def(ModeMeasureCountPulse::Count, correct);

    static ModeMeasureCountPulse *const modes[Channel::Count] =
    {
        &PageModesA::modeMeasureCountPulse,
        &PageModesB::modeMeasureCountPulse,
        &PageModesC::modeMeasureCountPulse,
        &def
    };

    return *modes[CURRENT_CHANNEL];
}


bool ModeMeasureCountPulse::IsFromPeriod() const
{
    return (value == ATB) || (value == BTA) || (value == CTA) || (value == CTB);
}
