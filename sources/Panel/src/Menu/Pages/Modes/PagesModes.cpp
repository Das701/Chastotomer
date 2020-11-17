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


void CurrentPageModes::DrawParameters(int, int)
{
//    Page *pages[Channel::Count] =
//    {
//        PageModesA::self,
//        PageModesB::self,
//        PageModesC::self,
//        PageModesD::self
//    };
//
//    Page &page = *pages[CURRENT_CHANNEL];

//    TypeMeasure *types[Channel::Count] =
//    {
//        &PageModesA::typeMeasure,
//        &PageModesB::typeMeasure,
//        &PageModesC::typeMeasure,
//        &PageModesD::typeMeasure
//    };
//
//    
//
//    static const bool enabledTimeLabels[TypeMeasure::Count][ModeMeasureFrequency::Count] =
//    {
//        false
//    };
//
//    static const bool enabledTimeMeasure[TypeMeasure::Count][ModeMeasureFrequency::Count] =
//    {
//        false
//    };
//
//    static const bool enabeldNumberPeriods[TypeMeasure::Count][ModeMeasureFrequency::Count] =
//    {
//        false
//    };
}
