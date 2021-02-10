#include "defines.h"
#include "Utils/Stack.h"
#include "SCPI/Utils.h"


bool SCPI::SU::IsLineEnding(pchar *buffer)
{
    bool result = (**buffer == 0x0D);

    if (result)
    {
        *(*buffer)++;
    }

    return result;
}


pchar SCPI::SU::BeginWith(pchar buffer, pchar word)
{
    while (*word)
    {
        if (*buffer == '\0')
        {
            return nullptr;
        }

        if (*word == *buffer)
        {
            ++word;
            ++buffer;
        }
        else
        {
            break;
        }
    }

    return (*word == '\0') ? buffer : nullptr;
}


bool SCPI::SU::ReadIntegerValue(pchar buffer, int *outValue, pchar *outEnd)
{
    ConverterToInteger converter;

    int result = 0;

    while (*buffer >= '0' && *buffer <= '9')
    {
        converter.Push(*buffer);
        buffer++;
    }


}
