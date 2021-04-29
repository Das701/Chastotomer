#include "defines.h"
#include "Display/Display.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/PageService.h"
#include "SCPI/SCPI.h"
#include "SCPI/StringUtils.h"
#include "Utils/String.h"
#include "Hardware/FreqMeter.h"

static pchar FuncIDN(pchar);
static pchar FuncReset(pchar);
static pchar FuncChannel(pchar);
static pchar FuncKeyPress(pchar);
static pchar FuncGovernorRotate(pchar);
static pchar FuncPicture(pchar);
static pchar FuncRefGenerator(pchar);
static pchar FuncTestSwitch(pchar);


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",             FuncIDN),
    SCPI_LEAF("*RST",              FuncReset),
    SCPI_LEAF(":CHANNEL",          FuncChannel),
    SCPI_LEAF(":KEY:PRESS",        FuncKeyPress),
    SCPI_LEAF(":REGULATOR:ROTATE", FuncGovernorRotate),
    SCPI_LEAF(":PICTURE",          FuncPicture),
    SCPI_LEAF(":REFGENERATOR",     FuncRefGenerator),
    SCPI_NODE(":DATA",             SCPI::data),
    SCPI_NODE(":INPUT",            SCPI::input),
    SCPI_NODE(":MEASURE",          SCPI::measure),
    SCPI_NODE(":SET",              SCPI::set),
    SCPI_LEAF("*TEST",             FuncTestSwitch),
    SCPI_EMPTY()
};


static pchar FuncIDN(pchar buffer)
{
    SCPI_PROLOG(buffer);

    SCPI::SendAnswer(String("OAO MNIPI, CH3-96/2, Cherem V1.2").c_str());

    SCPI_EPILOG(buffer);

    return nullptr;
}


static pchar FuncReset(pchar buffer)
{
    SCPI_PROLOG(buffer);
    PageService::UseReset();
    Display::Refresh();
    SCPI_EPILOG(buffer);
    return nullptr;
}


static void SetCurrentChannel(int i)
{
    Channel *channels[4] = { Channel::A, Channel::B, Channel::C, Channel::D };

    bool openSettings = Menu::OpenedPage() == Channel::Current()->pageSettings;
    bool openModes = Menu::OpenedPage() == Channel::Current()->pageModes;

    Channel::SetCurrent(channels[i]);

    Channel::LoadCurrentToFPGA(openSettings, openModes);

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

static void FuncGovernor(int i)
{
    if (i == 0)
    {
        Keyboard::AppendControl(Control(Control::GovLeft, Control::Action::Press));
    }
    else if(i == 1)
    {
        Keyboard::AppendControl(Control(Control::GovRight, Control::Action::Press));
    }
}

static pchar FuncGovernorRotate(pchar buffer)
{
    static const pchar names[] =
    {
        " LEFT",
        " RIGHT",
        ""
    };

    pchar end = nullptr;

    SCPI_PROCESS_ARRAY(names, FuncGovernor(i));
}


static pchar FuncPicture(pchar buffer)
{
    SCPI_PROLOG(buffer);

    Display::SendToSCPI();

    SCPI_EPILOG(buffer);

    return nullptr;
}


static pchar FuncRefGenerator(pchar buffer)
{
    static const pchar generator[] =
    {
        " INT",
        " EXT",
        ""
    };

    return SCPI::ProcessSimpleParameter(buffer, generator, PageIndication::refGenerator.sw);
}


static pchar FuncTestSwitch(pchar buffer)
{
    SCPI_PROLOG(buffer);

    FreqMeter::modeTest.Switch();
    Display::Refresh();
    SCPI_EPILOG(buffer);

    return nullptr;
}
