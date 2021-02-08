#include "defines.h"
#include "Display/Display.h"
#include "Menu/Pages/Channels/Channels.h"
#include "SCPI/SCPI.h"
#include "Utils/String.h"


static pchar FuncIDN(pchar);
static pchar FuncReset(pchar);
static pchar FuncChannel(pchar);
static pchar FuncKeyPress(pchar);
static pchar FuncPicture(pchar);


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",      FuncIDN),
    SCPI_LEAF("*RST",       FuncReset),
    SCPI_LEAF(":CHANNEL",   FuncChannel),
    SCPI_LEAF(":KEY:PRESS", FuncKeyPress),
    SCPI_LEAF(":PICTURE",   FuncPicture),
    SCPI_NODE(":INPUT",     SCPI::input),
    SCPI_EMPTY()
};


static pchar FuncIDN(pchar buffer)
{
    SCPI_PROLOG(buffer)

    SCPI::SendAnswer(String("OAO MNIPI, CH3-96/2, Cherem V1.2").c_str());

    SCPI_EPILOG(buffer)
}


static pchar FuncReset(pchar buffer)
{
    SCPI_PROLOG(buffer)
        
    SCPI_EPILOG(buffer)
}


static void SetCurrentChannel(int i)
{
    Channel *channels[4] = { Channel::A, Channel::B, Channel::C, Channel::D };

    Channel::SetCurrent(channels[i]);

    Channel::LoadCurrentToFPGA();

    Display::Refresh();
}


static pchar FuncChannel(pchar buffer)
{
    static const pchar names[] = { " A", " B", " C", " D", "" };

    SCPI_REQUEST(SCPI::SendAnswer(names[Channel::Current()->Number()]));
    SCPI_PROCESS_ARRAY(names, SetCurrentChannel(i));
}


static void FuncControl(int i)
{
    Keyboard::AppendControl(Control((Control::E)i, Control::Action::Press));
    Keyboard::AppendControl(Control((Control::E)i, Control::Action::Release));
}


static pchar FuncKeyPress(pchar buffer)
{
    static const pchar names[] =
    {
        " REG",
        " MODE",
        " INDICATION",
        " LEFT",
        " RIGHT",
        " CHANNEL",
        " ENTER",
        " SERVICE",
        " ",
        " ",
        " TEST",
        " AUTO",
        ""
    };

    pchar end = nullptr;

    SCPI_PROCESS_ARRAY(names, FuncControl(i));
}


static pchar FuncPicture(pchar buffer)
{
    SCPI_PROLOG(buffer);

    Display::SendToSCPI();

    SCPI_EPILOG(buffer);
}
