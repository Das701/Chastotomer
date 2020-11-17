#include "defines.h"
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


bool PageModes::IsActiveTimeLabels(TypeMeasure *type, int mode)
{
    return false;
}


bool PageModes::IsActiveTimeMeasure(TypeMeasure *type, int mode)
{
    return false;
}


bool PageModes::IsActiveNumberPeriods(TypeMeasure *type, int mode)
{
    return false;
}


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
