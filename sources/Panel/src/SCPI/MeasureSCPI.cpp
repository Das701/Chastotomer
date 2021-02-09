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

    String answer(types[Channel::Current()->mod.typeMeasure.value]);

    SCPI::SendAnswer(answer);

    SCPI_EPILOG(buffer);

    return nullptr;
}


static pchar FuncFrequency(pchar buffer)
{
    SCPI_REQUEST(SCPI::AnswerInput(modesFrequency, Channel::Current()->mod.typeMeasure.value));

    return nullptr;
}


static pchar FuncPeriod(pchar buffer)
{
    return nullptr;
}


static pchar FuncDuration(pchar buffer)
{
    return nullptr;
}


static pchar FuncCounter(pchar buffer)
{
    return nullptr;
}
