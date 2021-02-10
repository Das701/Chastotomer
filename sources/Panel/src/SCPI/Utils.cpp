#include "defines.h"
#include "SCPI/Utils.h"


bool SCPI::SU::IsLineEnding(pchar *buffer)
{
    bool result = (**buffer == 0x0D);

    if (result)
    {
        *(*buffer)++; //-V532
    }

    return result;
}
