#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/Sender.h"


int SCPI::Sender::valuesForSend = 0;
SCPI::Sender::Mode::E SCPI::Sender::mode = SCPI::Sender::Mode::Hand;


void SCPI::Sender::SendValue(pchar value)
{
    if (mode == Mode::Auto || valuesForSend > 0)
    {
        SendAnswer(value);

        if (valuesForSend > 0)
        {
            valuesForSend--;
        }
    }
}
