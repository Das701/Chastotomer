#include "defines.h"
#include "Display/Display.h"
#include "Menu/Pages/Channels/Channels.h"
#include "SCPI/SCPI.h"
#include "Utils/String.h"


static pCHAR FuncIDN(pCHAR);
static pCHAR FuncReset(pCHAR);
static pCHAR FuncChannel(pCHAR);
static pCHAR FuncPicture(pCHAR);


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",    FuncIDN),
    SCPI_LEAF("*RST",     FuncReset),
    SCPI_LEAF(":CHANNEL", FuncChannel),
    SCPI_LEAF(":PICTURE", FuncPicture),
    SCPI_NODE(":INPUT",   SCPI::input),
    SCPI_EMPTY()
};


static pCHAR FuncIDN(pCHAR buffer)
{
    SCPI_PROLOG(buffer)

    SCPI::SendAnswer(String("OAO MNIPI, CH3-96/2, Cherem V1.2").c_str());

    SCPI_EPILOG(buffer)
}


static pCHAR FuncReset(pCHAR buffer)
{
    SCPI_PROLOG(buffer)
        
    SCPI_EPILOG(buffer)
}


static void SetCurrentChannel(int i)
{

}


static pCHAR FuncChannel(pCHAR buffer)
{
    pchar names[] = { "A", "B", "C", "D", "" };

    SCPI_REQUEST(SCPI::SendAnswer(names[Channel::Current()->Number()]));
    SCPI_PROCESS_ARRAY(names, SetCurrentChannel(i));
}


static pCHAR FuncPicture(pCHAR buffer)
{
    SCPI_PROLOG(buffer);

    Display::SendToSCPI();

    SCPI_EPILOG(buffer);
}
