#include "defines.h"
#include "Settings.h"
#include "SCPI/SCPI.h"
#include "Utils/String.h"


// Базовая структура для установки значения параметра
struct SetSCPI
{
    virtual ~SetSCPI() {}

    void Set(int i) const
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

    virtual void SetParameter(int, int) const {};         // Неиспользуемый второй аргумент нужен для того, чтобы случайно не вызвать напрямую
};


// Общая функция для отправки ответа на запросную форму команды
static void AnswerInput(const pchar choice[], uint8 value);

// Общая функция обработки запроса
static pchar FuncInput(pchar buffer, const pchar choice[], uint8 value, const SetSCPI &set);


static pchar FuncCoupling(pchar);
static pchar FuncImpedance(pchar);
static pchar FuncFilter(pchar);
static pchar FuncModeFront(pchar);
static pchar FuncDivider(pchar);


const StructSCPI SCPI::input[] =
{
    SCPI_LEAF(":COUPLE",    FuncCoupling),
    SCPI_LEAF(":IMPEDANCE", FuncImpedance),
    SCPI_LEAF(":FILTER",    FuncFilter),
    SCPI_LEAF(":FRONT",     FuncModeFront),
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


static pchar FuncInput(pchar buffer, const pchar choice[], uint8 value, const SetSCPI &set)
{
    SCPI_REQUEST(AnswerInput(choice, value));
    SCPI_PROCESS_ARRAY(choice, set.Set(i));
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

    return FuncInput(buffer, coupling, Channel::Current()->set.couple.value, CoupleSCPI());
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

    return FuncInput(buffer, impedance, Channel::Current()->set.impedance.value, ImpedanceSCPI());
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

    return FuncInput(buffer, filter, Channel::Current()->set.modeFilter.value, FilterSCPI());
}


static pchar FuncModeFront(pchar buffer)
{
    struct ModeFrontSCPI : public SetSCPI { virtual void SetParameter(int i, int) const { Channel::Current()->set.modeFront.Set((ModeFront::E)i); } };

    static const pchar modeFront[] =
    {
        " RISE",
        " FALL",
        ""
    };

    return FuncInput(buffer, modeFront, Channel::Current()->set.modeFront.value, ModeFrontSCPI());
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

    pchar end = SCPI::BeginWith(buffer, "?");

    if (end)
    {
        if (SCPI::IsLineEnding(&end))
        {
            SCPI::SendBadSymbols();

            AnswerInput(divider, Channel::Current()->set.divider.value);

            return end;
        }

        return nullptr;
    }

    for (int i = 0; i < divider[i][0] != 0; i++)
    {
        end = SCPI::BeginWith(buffer, divider[i]);
        SCPI_RUN_IF_END(DividerSCPI().Set(i))
    }

    return nullptr;
}
