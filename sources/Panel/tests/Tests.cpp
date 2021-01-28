#include "defines.h"
#include "Tests.h"


bool Tests::Run()
{
    if (!TestValuesFPGA())
    {
        return false;
    }

    return true;
}
