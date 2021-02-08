#include "defines.h"
#include "Settings.h"
#include "SCPI/SCPI.h"
#include "Utils/String.h"


static pchar FuncCoupling(pchar);
static pchar FuncImpedance(pchar);
static pchar FuncFilter(pchar);
static pchar FuncTriggerSlope(pchar);
static pchar FuncDivider(pchar);


const StructSCPI SCPI::input[] =
{
    SCPI_LEAF(":COUPLE",        FuncCoupling),
    SCPI_LEAF(":IMPEDANCE",     FuncImpedance),
    SCPI_LEAF(":FILTER",        FuncFilter),
    SCPI_LEAF(":TRIGGER:SLOPE", FuncTriggerSlope),
    SCPI_LEAF(":DIVIDER",       FuncDivider),
    SCPI_EMPTY()
};


void SCPI::AnswerInput(const pchar choice[], uint8 value)
{
    if (CURRENT_CHANNEL_IS_A_OR_B)
    {
        SCPI::SendAnswer(choice[value]);
    }
    else
    {
        SCPI::Answer::CurrentChannelHasNotParameter();
    }
}


pchar SCPI::ProcessSimpleParameter(pchar buffer, const pchar choice[], uint8 value, const SetSCPI &s)
{
    SCPI_REQUEST(AnswerInput(choice, value));
    SCPI_PROCESS_ARRAY(choice, s.Set(i));
}


static pchar FuncCoupling(pchar buffer)
{
    struct CoupleSCPI : public SetSCPI { virtual void SetParameter(int i, int) const { Channel::Current()->set.couple.Set((InputCouple::E)i); } };

    static const pchar coupling[] =
    {
        " AC",
        " DC",
        ""
    };

    return SCPI::ProcessSimpleParameter(buffer, coupling, Channel::Current()->set.couple.value, CoupleSCPI());
}


static pchar FuncImpedance(pchar buffer)
{
    struct ImpedanceSCPI : public SetSCPI { virtual void SetParameter(int i, int) const { Channel::Current()->set.impedance.Set((InputImpedance::E)i); } };

    static const pchar impedance[] =
    {
        " 1MOHM",
        " 50OHM",
        ""
    };

    return SCPI::ProcessSimpleParameter(buffer, impedance, Channel::Current()->set.impedance.value, ImpedanceSCPI());
}


static pchar FuncFilter(pchar buffer)
{
    struct FilterSCPI : public SetSCPI { virtual void SetParameter(int i, int) const { Channel::Current()->set.modeFilter.Set((ModeFilter::E)i); } };

    static const pchar filter[] =
    {
        " ON",
        " OFF",
        ""
    };

    return SCPI::ProcessSimpleParameter(buffer, filter, Channel::Current()->set.modeFilter.value, FilterSCPI());
}


static pchar FuncTriggerSlope(pchar buffer)
{
    struct ModeFrontSCPI : public SetSCPI { virtual void SetParameter(int i, int) const { Channel::Current()->set.modeFront.Set((ModeFront::E)i); } };

    static const pchar modeFront[] =
    {
        " RISE",
        " FALL",
        ""
    };

    return SCPI::ProcessSimpleParameter(buffer, modeFront, Channel::Current()->set.modeFront.value, ModeFrontSCPI());
}


static pchar FuncDivider(pchar buffer)
{
    struct DividerSCPI : public SetSCPI { virtual void SetParameter(int i, int) const { Channel::Current()->set.divider.Set((Divider::E)i); }; };

    static const pchar divider[] =
    {
        " 1",
        " 10",
        ""
    };

    return SCPI::ProcessSimpleParameter(buffer, divider, Channel::Current()->set.divider.value, DividerSCPI());
}
