#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/Sender.h"


bool SCPI::Sender::needSend = false;


void SCPI::Sender::SendValue(pchar value)
{
    if (needSend)
    {
        SendAnswer(value);
        needSend = false;
    }
}
