#include "defines.h"
#include "FreqMeter.h"
#include "Menu/MenuItems.h"
#include "Hardware/HAL.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageChannelB.h"
#include "Menu/Pages/PageChannelC.h"
#include "Menu/Pages/PageChannelD.h"
#include "Menu/Hint.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//void FreqMeter::LoadChannel()
//{
//    char command[4] = { 0, 0, 0, 0 };
//    char* argument;
//    
//    switch ()
//    {
//    case 
//        char argument1[6] = { 0, 0, 0, 0, 0, 0 };
//        argument = argument1;
//        break;
//    case 
//        char argument2[6] = { 1, 0, 0, 0, 0, 0 };
//        argument = argument2;
//        break;
//    case 
//        char argument3[6] = { 0, 1, 0, 0, 0, 0 };
//        argument = argument3;
//        break;
//    }
//    PLIS::WriteCommand(command, argument);
//}

void FreqMeter::LoadModeMeasureFrequency()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadModeMeasurePeriod()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadModeMeasureDuration()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadModeMeasureCountPulse()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadPeriodTimeLabels()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadTimeMeasure()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadNumerPeriodsMeasure()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadInputCouple()
{
    char command[4] = { 1, 1, 0, 0 };
    char* argument;
    if(PageChannelA::couple.value == 1)
    {
        char argument1[6] = { 0, 0, 0, 0, 0, 0 };
        argument = argument1;
    }
    else
    {
        char argument2[6] = { 1, 0, 0, 0, 0, 0 };
        argument = argument2;
    }
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadImpedance()
{
    char command[4] = { 1, 0, 0, 0 };
    char* argument;
    uint8 value = PageChannelA::impedance.value;
    if(value == InputImpedance::_1MOmh)
    {
        char argument1[6] = { 0, 0, 0, 0, 0, 0 };
        argument = argument1;
    }
    else
    {
        char argument2[6] = { 1, 0, 0, 0, 0, 0 };
        argument = argument2;
    }
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadModeFilter()
{
    char command[4] = { 1, 0, 1, 0 };
    char* argument;
    if(PageChannelA::modeFilter.value == 1)
    {
        char argument1[6] = { 0, 0, 0, 0, 0, 0 };
        argument = argument1;
    }
    else
    {
        char argument2[6] = { 1, 0, 0, 0, 0, 0 };
        argument = argument2;
    }
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadModeFront()
{
    char command[4] = { 0, 0, 1, 0 };
    char* argument;
    if(PageChannelA::modeFront.value == 1)
    {
        char argument1[6] = { 0, 0, 0, 0, 0, 0 };
        argument = argument1;
    }
    else
    {
        char argument2[6] = { 1, 0, 0, 0, 0, 0 };
        argument = argument2;
    }
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadDivider()
{
    char command[4] = { 0, 1, 0, 0 };
    char* argument;
    if(PageChannelA::divider.value == 1)
    {
        char argument1[6] = { 0, 0, 0, 0, 0, 0 };
        argument = argument1;
    }
    else
    {
        char argument2[6] = { 1, 0, 0, 0, 0, 0 };
        argument = argument2;
    }
    PLIS::WriteCommand(command, argument);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::LoadTypeSynch()
{
    char command[4] = { 1, 1, 0, 1 };
    char* argument;
    if(PageChannelA::typeSynch.value == 1)
    {
        char argument1[6] = { 0, 0, 0, 0, 0, 0 };
        argument = argument1;
    }
    else
    {
        char argument2[6] = { 1, 0, 0, 0, 0, 0 };
        argument = argument2;
    }
    PLIS::WriteCommand(command, argument);
}
