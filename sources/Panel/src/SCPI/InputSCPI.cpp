#include "defines.h"
#include "Settings.h"
#include "SCPI/SCPI.h"
#include "Utils/String.h"


struct SetSCPI
{
    void Set(int i)
    {
        if (CURRENT_CHANNEL_IS_A_OR_B)
        {
            SetParameter(i, 0);
        }
        else
        {
            SCPI::Answer::CurrentChannelHasNotParameter();
        }
    }

    virtual void SetParameter(int, int) {};         // Неиспользуемый второй аргумент нужен для того, чтобы случайно не вызвать напрямую
};


static pchar FuncCoupling(pchar);
static pchar FuncFilter(pchar);
static pchar FuncImpedance(pchar);
static pchar FuncDivider(pchar);


const StructSCPI SCPI::input[] =
{
    SCPI_LEAF(":COUPLE",  FuncCoupling),
    SCPI_LEAF(":FILTER",    FuncFilter),
    SCPI_LEAF(":IMPEDANCE", FuncImpedance),
    SCPI_LEAF(":DIVIDER",   FuncDivider),
    SCPI_EMPTY()
};


static void AnswerInput(const pchar choice[], uint8 value)
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


static pchar FuncCoupling(pchar buffer)
{
    struct CoupleSCPI : public SetSCPI { virtual void SetParameter(int i, int) { Channel::Current()->set.couple.Set((InputCouple::E)i); } };

    static const pchar coupling[] =
    {
        " AC",
        " DC",
        ""
    };

    SCPI_REQUEST(AnswerInput(coupling, Channel::Current()->set.couple.value));
    SCPI_PROCESS_ARRAY(coupling, CoupleSCPI().Set(i));
}


static pchar FuncImpedance(pchar buffer)
{
    struct ImpedanceSCPI : public SetSCPI { virtual void SetParameter(int i, int) { Channel::Current()->set.impedance.Set((InputImpedance::E)i); } };

    static const pchar impedance[] =
    {
        " 1MOHM",
        " 50OHM",
        ""
    };

    SCPI_REQUEST(AnswerInput(impedance, Channel::Current()->set.impedance.value));
    SCPI_PROCESS_ARRAY(impedance, ImpedanceSCPI().Set(i));
}


static pchar FuncFilter(pchar buffer)
{
    struct FilterSCPI : public SetSCPI { virtual void SetParameter(int i, int) { Channel::Current()->set.modeFilter.Set((ModeFilter::E)i); } };

    static const pchar filter[] =
    {
        " ON",
        " OFF",
        ""
    };

    SCPI_REQUEST(SCPI::SendAnswer(filter[Channel::Current()->set.modeFilter.value]));
    SCPI_PROCESS_ARRAY(filter, FilterSCPI().Set(i));
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
