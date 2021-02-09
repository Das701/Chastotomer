#include "defines.h"
#include "Menu/Pages/Channels/Channels.h"
#include "SCPI/SCPI.h"


static pchar FuncCurrent(pchar);
static pchar FuncFrequency(pchar);
static pchar FuncPeriod(pchar);
static pchar FuncDuration(pchar);
static pchar FuncCounter(pchar);


const StructSCPI SCPI::measure[] =
{
    SCPI_LEAF(":CURRENT?",  FuncCurrent),
    SCPI_LEAF(":FREQUENCY", FuncFrequency),
    SCPI_LEAF(":PERIOD",    FuncPeriod),
    SCPI_LEAF(":DURATION",  FuncDuration),
    SCPI_LEAF(":CNT",       FuncCounter),
    ""
};


static const pchar types[] =
{
    " FREQUENCY",
    " PERIOD",
    " DURATION",
    " CNT",
    ""
};


static const pchar modesFrequency[] =
{
    " DIRECT",
    " 1/T",
    " A/B",
    " A/C",
    " B/A",
    " B/C",
    " C/A",
    " C/B",
    " TACHOMETER",
    " COMPARATOR",
    ""
};


static const pchar modesPeriod[] =
{
    " DIRECT",
    " 1/F",
    ""
};


static const pchar modesDuration[] =
{
    " DIRECT",
    " NDT/1NS",
    " INTERVAL",
    " FILLFACTOR",
    " PHASE",
    ""
};


static const pchar modesCounter[] =
{
    " A(tB)",
    " A(TB)",
    " B(tA)",
    " B(TA)",
    " C(tA)",
    " C(tB)",
    " C(TA)",
    " C(TB)",
    " HAND",
    ""
};


static pchar FuncCurrent(pchar buffer)
{
    SCPI_PROLOG(buffer);

    ModesChannel &modes = Channel::Current()->mod;

    int type = modes.typeMeasure.value;

    String answer("%s:", types[type]);

    switch (type)
    {
    case TypeMeasure::Frequency:    answer.Append(modesFrequency[modes.modeFrequency] + 1); break;
    case TypeMeasure::Period:       answer.Append(modesPeriod[modes.modePeriod] + 1);       break;
    case TypeMeasure::Duration:     answer.Append(modesDuration[modes.modeDuration] + 1);   break;
    case TypeMeasure::CountPulse:   answer.Append(modesCounter[modes.modeCountPulse] + 1);  break;
    }

    SCPI::SendAnswer(answer.c_str());

    SCPI_EPILOG(buffer);

    return nullptr;
}


static void SetModeMeasure(TypeMeasure::E type, int i, Enumeration &mode)
{
    Channel *channel = Channel::Current();

    if (channel->ExistMeasure(type, i))
    {
        if (channel->mod.typeMeasure.value != type)
        {
            channel->mod.typeMeasure.sw->SetValue((uint8)type);
        }

        if (mode.value != i)
        {
            mode.sw->SetValue((uint8)i);
        }
    }
    else
    {
        SCPI::Answer::ThisModeCannotBeSetForTheCurrentChannel();
    }
}


static pchar ProcessSetModeMeasure(pchar buffer, const pchar choice[], TypeMeasure::E type)
{
    Channel *channel = Channel::Current();

    Enumeration *modes[] =
    {
        &channel->mod.modeFrequency,
        &channel->mod.modePeriod,
        &channel->mod.modeDuration,
        &channel->mod.modeCountPulse
    };

    pchar end = nullptr;

    SCPI_PROCESS_ARRAY(choice, SetModeMeasure(type, i, *modes[type]));
}


static pchar FuncFrequency(pchar buffer)
{
    return ProcessSetModeMeasure(buffer, modesFrequency, TypeMeasure::Frequency);
}


static pchar FuncPeriod(pchar buffer)
{
    return ProcessSetModeMeasure(buffer, modesPeriod, TypeMeasure::Period);
}


static pchar FuncDuration(pchar buffer)
{
    return ProcessSetModeMeasure(buffer, modesDuration, TypeMeasure::Duration);
}


static pchar FuncCounter(pchar buffer)
{
    return ProcessSetModeMeasure(buffer, modesCounter, TypeMeasure::CountPulse);
}


/*
pchar end = nullptr;

for (int i = 0; i < modesFrequency[i][0] != 0; i++)
{
    end = SCPI::BeginWith(buffer, modesFrequency[i]);

    if (end)
    {
        if (SCPI::IsLineEnding(&end))
        {
            SCPI::SendBadSymbols();

            Function();

            return end;
        }
    }
}

return nullptr;
*/
