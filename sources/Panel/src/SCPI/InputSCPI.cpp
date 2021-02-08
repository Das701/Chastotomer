#include "defines.h"
#include "Settings.h"
#include "SCPI/SCPI.h"
#include "Utils/String.h"


static pchar FuncCoupling(pchar);
static pchar FuncFilter(pchar);
static pchar FuncImpedance(pchar);
static pchar FuncDivider(pchar);


const StructSCPI SCPI::input[] =
{
    SCPI_LEAF(":COUPLING",  FuncCoupling),
    SCPI_LEAF(":FILTER",    FuncFilter),
    SCPI_LEAF(":IMPEDANCE", FuncImpedance),
    SCPI_LEAF(":DIVIDER",   FuncDivider),
    SCPI_EMPTY()
};


static const pchar coupling[] =
{
    " AC",
    " DC",
    ""
};

static void AnswerCoupling()
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        SCPI::SendAnswer(coupling[Channel::Current()->set.couple.value]);
    }
    else
    {
        SCPI::Answer::CurrentChannelHasNotParameter();
    }
}

static void SetCoupling(int i)
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        Channel::Current()->set.couple.Set((InputCouple::E)i);
    }
    else
    {
        SCPI::Answer::CurrentChannelHasNotParameter();
    }
}

static pchar FuncCoupling(pchar buffer)
{
    SCPI_REQUEST(AnswerCoupling());
    SCPI_PROCESS_ARRAY(coupling, SetCoupling(i));
}


static const pchar impedance[] =
{
    " 1MOHM",
    " 50OHM",
    ""
};

static void AnswerImpedance()
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        SCPI::SendAnswer(impedance[Channel::Current()->set.impedance.value]);
    }
    else
    {
        SCPI::Answer::CurrentChannelHasNotParameter();
    }
}

static void SetImpedance(int i)
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        Channel::Current()->set.impedance.Set((InputImpedance::E)i);
    }
    else
    {
        SCPI::Answer::CurrentChannelHasNotParameter();
    }
}

static pchar FuncImpedance(pchar buffer)
{
    SCPI_REQUEST(AnswerImpedance());
    SCPI_PROCESS_ARRAY(impedance, SetImpedance(i));
}


static pchar FuncFilter(pchar buffer)
{
    static const pchar filter[] =
    {
        " ON",
        " OFF",
        ""
    };

    SCPI_REQUEST(SCPI::SendAnswer(filter[Channel::Current()->set.modeFilter.value]));
    SCPI_PROCESS_ARRAY(filter, Channel::Current()->set.modeFilter.Set((ModeFilter::E)i));
}


static pchar FuncDivider(pchar buffer)
{
    static const pchar divider[] =
    {
        " 1",
        " 10",
        ""
    };

    SCPI_REQUEST(SCPI::SendAnswer(divider[Channel::Current()->set.divider.value]));

    return buffer;
}
