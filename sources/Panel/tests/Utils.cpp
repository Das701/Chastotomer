#include "defines.h"
#include "Tests.h"
#include <cmath>
#include <cstdlib>


static double RandInt()
{
    return std::rand();
}


namespace Tests
{
    namespace Utils
    {
        double RandDouble()
        {
            return RandInt() * RandInt() / RandInt() / RandInt() * RandInt() * RandInt() / RandInt() / RandInt();
        }

        bool EqualsDouble(double d1, double d2)
        {
            return std::fabs(d1 - d2) < 1e-3;
        }
    }
}
